/*
 *   Copyright (C) 2016  Antonio Pastor
 *
 *   This program is free software: you can redistribute it
 *   and/or modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation, either version 3 of the License,
 *   or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.
 *   If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: Mar 7, 2016
 *      Author: apastor <anpastor{at}it.uc3m.es>
 */


#include "postgres.h"
#include "fmgr.h"

#include "math.h"
#include "catalog/pg_type.h"
#include "utils/array.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif




//sfunc( internal-state, next-data-values ) ---> next-internal-state
//finalfunc( internal-state ) ---> aggregate-value

// function check_float8_array extracted from PostgreSQL source src/backend/utils/adt/float.c
static float8 * check_float8_array(ArrayType *transarray, const char *caller, int n);


PG_FUNCTION_INFO_V1(entropy_sfunc);

Datum entropy_sfunc(PG_FUNCTION_ARGS) {

	// state value can safely be modified in-place in transition functions
	ArrayType * internal = PG_GETARG_ARRAYTYPE_P(0);
//	float8 * internal_values = check_float8_array(internal, "entropy_accum", 2);
	float8 * internal_values = (float8 *) ARR_DATA_PTR(internal);
	int64	x_ij = PG_GETARG_INT64(1);
	internal_values[0] += x_ij;
	if (x_ij> 1){
		internal_values[1] += x_ij * log2(x_ij);
	}
	PG_RETURN_ARRAYTYPE_P(internal);
}


PG_FUNCTION_INFO_V1(entropy_final);

Datum entropy_final(PG_FUNCTION_ARGS) {

//	ArrayType * internal = PG_GETARG_ARRAYTYPE_P_COPY(0);
//	float8 * internal_values = check_float8_array(internal, "entropy_final", 2);
	float8 * internal_values = (float8 *) ARR_DATA_PTR(PG_GETARG_ARRAYTYPE_P(0));
	PG_RETURN_FLOAT4( log2(internal_values[0]) - (internal_values[1] / internal_values[0]) );
}


PG_FUNCTION_INFO_V1(norm_entropy_final);

Datum norm_entropy_final(PG_FUNCTION_ARGS) {

//	ArrayType * internal = PG_GETARG_ARRAYTYPE_P_COPY(0);
//	float8 * internal_values = check_float8_array(internal, "norm_entropy_final", 2);
	float8 * internal_values = (float8 *) ARR_DATA_PTR(PG_GETARG_ARRAYTYPE_P(0));
	PG_RETURN_INT32(round( 100 * (1 - (internal_values[1] / ( internal_values[0] * log2(internal_values[0]) )) ) ));
}


//sfunc( internal-state, next-data-values ) ---> next-internal-state
//finalfunc( internal-state ) ---> aggregate-value
PG_FUNCTION_INFO_V1(sumX_log2X);

Datum sumX_log2X(PG_FUNCTION_ARGS) {

	// state value can safely be modified in-place in transition functions
    float4	sum = PG_GETARG_FLOAT8(0);
    int64	x_ij = PG_GETARG_INT64(1);

    if(x_ij>1){
    	sum += (x_ij * log2(x_ij));
    }
    PG_RETURN_FLOAT8(sum);
}


// ARG[0] = X_COUNT, ARG[1] = SumXLog2X
PG_FUNCTION_INFO_V1(entropy_from_sum);

Datum entropy_from_sum(PG_FUNCTION_ARGS) {
    int64 x_count = PG_GETARG_INT64(0);
    float8 sumX_log2X = PG_GETARG_FLOAT8(1);

    if (x_count>0){
        PG_RETURN_FLOAT4( log2(x_count) - (sumX_log2X / x_count) );
    }
    else {
        PG_RETURN_NULL();
    }
}


// ARG[0] = X_COUNT, ARG[1] = SumXLog2X
PG_FUNCTION_INFO_V1(norm_entropy_from_sum);

Datum norm_entropy_from_sum(PG_FUNCTION_ARGS) {
    int64 x_count = PG_GETARG_INT64(0);
    float8	sumX_log2X = PG_GETARG_FLOAT8(1);

    if (x_count>0){
    	int32 score = round(100 * (1 - (sumX_log2X / (x_count * log2(x_count)) ) ));
    	PG_RETURN_INT32(score);
    }
    else {
    	PG_RETURN_NULL();
    }
}

static float8 * check_float8_array(ArrayType *transarray, const char *caller, int n) {
        /*
         * We expect the input to be an N-element float array; verify that. We
         * don't need to use deconstruct_array() since the array data is just
         * going to look like a C array of N float8 values.
         */
        if (ARR_NDIM(transarray) != 1 ||
                ARR_DIMS(transarray)[0] != n ||
                ARR_HASNULL(transarray) ||
                ARR_ELEMTYPE(transarray) != FLOAT8OID)
                elog(ERROR, "%s: expected %d-element float8 array", caller, n);
        return (float8 *) ARR_DATA_PTR(transarray);
}

//void main(){
//	int a = 7;
//	printf("log2 of 4 = %.2f\n", log2(a));
//}

/*
 * entropy.c
 *
 *  Created on: Mar 7, 2016
 *      Author: apastor
 */


#include "postgres.h"
#include "fmgr.h"

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include "catalog/pg_type.h"
#include "libpq/pqformat.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/sortsupport.h"

//#include <stdlib.h>
//#include <stdio.h>

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(entropy);
PG_FUNCTION_INFO_V1(sumXlog2X);
PG_FUNCTION_INFO_V1(entropyFromSum);



//sfunc( internal-state, next-data-values ) ---> next-internal-state
//ffunc( internal-state ) ---> aggregate-value

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


Datum entropy(PG_FUNCTION_ARGS) {

		ArrayType * internal = PG_GETARG_ARRAYTYPE_P(0);
		float8 * internal_values = check_float8_array(internal, "entropy", 2);

		if(PG_NARGS()>1) {
			int64	x_ij = PG_GETARG_INT64(1);
			internal_values[0] += x_ij;
			if (x_ij> 1){
				internal_values[1] += x_ij * log2(x_ij);
			}
			PG_RETURN_ARRAYTYPE_P(internal);
		} else {
			PG_RETURN_FLOAT8( log2(internal_values[0]) - (internal_values[1] / internal_values[0]) );
		}
}


//sfunc( internal-state, next-data-values ) ---> next-internal-state
//ffunc( internal-state ) ---> aggregate-value

Datum sumXlog2X(PG_FUNCTION_ARGS) {

    float4	sum = PG_GETARG_FLOAT4(0);
    int64	x_ij = PG_GETARG_INT64(1);

    if(x_ij>1){
    	sum += (x_ij * log2(x_ij));
    }
    PG_RETURN_FLOAT4(sum);
}

// ARG[0] = X_COUNT, ARG[1] = SumXLog2X

Datum entropyFromSum(PG_FUNCTION_ARGS) {
    int64	x_count = PG_GETARG_INT64(0);
    float4	sumXlog2X = PG_GETARG_FLOAT4(1);

    if (x_count>0){
    	PG_RETURN_FLOAT4( log2(x_count) - (sumXlog2X / x_count) );
    }
    else {
    	PG_RETURN_NULL();
    }
}


//void main(){
//	int a = 7;
//	printf("log2 of 4 = %.2f\n", log2(a));
//}

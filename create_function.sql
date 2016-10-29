CREATE OR REPLACE FUNCTION entropy_sfunc(float8[], int8) RETURNS float8[]
AS 'libentropy.so', 'entropy_sfunc'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION entropy_final(float8[]) RETURNS float4
AS 'libentropy.so', 'entropy_final'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION norm_entropy_final(float8[]) RETURNS int4
AS 'libentropy.so', 'norm_entropy_final'
LANGUAGE C IMMUTABLE STRICT;

DROP AGGREGATE IF EXISTS entropy(int8);

CREATE AGGREGATE entropy(int8)
(
    sfunc = entropy_sfunc,
    stype = float8[],
    finalfunc = entropy_final,
    initcond = '{0,0}'
);

DROP AGGREGATE IF EXISTS norm_entropy(int8);

CREATE AGGREGATE norm_entropy(int8)
(
    sfunc = entropy_sfunc,
    stype = float8[],
    finalfunc = norm_entropy_final,
    initcond = '{0,0}'
);

CREATE OR REPLACE FUNCTION sumX_log2X_float8(float8, int8) RETURNS float8
AS 'libentropy.so', 'sumX_log2X'
LANGUAGE C IMMUTABLE STRICT;

DROP AGGREGATE IF EXISTS sumX_log2X(int8);

CREATE AGGREGATE sumX_log2X(int8)
(
sfunc = sumX_log2X_float8,
stype = float8,
initcond = 0
);

CREATE OR REPLACE FUNCTION entropy_from_sum(int8, float8) RETURNS float4
AS 'libentropy.so', 'entropy_from_sum'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION norm_entropy_from_sum(int8, float8) RETURNS int4
AS 'libentropy.so', 'norm_entropy_from_sum'
LANGUAGE C IMMUTABLE STRICT;

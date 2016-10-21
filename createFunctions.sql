CREATE OR REPLACE FUNCTION entropy_float8(float8[], int8) RETURNS float8[]
AS 'libentropy.so', 'entropy'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION entropy_float8(float8[]) RETURNS float8
AS 'libentropy.so', 'entropy'
LANGUAGE C IMMUTABLE STRICT;

DROP AGGREGATE IF EXISTS entropy(int8);

CREATE AGGREGATE entropy(int8)
(sfunc = entropy_float8,
stype = float8[],
finalfunc = entropy_float8,
initcond = '{0,0}'
);

CREATE OR REPLACE FUNCTION sumXlog2X_float4(float4, int8) RETURNS float4
AS 'libentropy.so', 'sumXlog2X'
LANGUAGE C IMMUTABLE STRICT;

DROP AGGREGATE IF EXISTS sumXlog2X(int8);

CREATE AGGREGATE sumXlog2X(int8)
(sfunc = sumXlog2X_float4,
stype = float4,
initcond = 0
);

CREATE OR REPLACE FUNCTION entropyFromSum(int8, float4) RETURNS float4
AS 'libentropy.so', 'entropyFromSum'
LANGUAGE C IMMUTABLE STRICT;

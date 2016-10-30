CREATE TEMPORARY TABLE test1 (
    X1 int,
    X2 int,
    X3 int
);

INSERT INTO test1 VALUES (5,1,1000);
INSERT INTO test1 VALUES (0,1,1000);
INSERT INTO test1 VALUES (0,1,1000);
INSERT INTO test1 VALUES (0,1,1000);
INSERT INTO test1 VALUES (0,1,1000);

SELECT entropy(X1), entropy(X2), entropy(X3) FROM test1;

SELECT norm_entropy(X1), norm_entropy(X2), norm_entropy(X3) FROM test1;

SELECT sumX_log2X(X1), sumX_log2X(X2), sumX_log2X(X3) FROM test1;

SELECT entropy_from_sum(5,sumX_log2X(X1)), entropy_from_sum(5,sumX_log2X(X2)), entropy_from_sum(5000,sumX_log2X(X3)) FROM test1;

SELECT norm_entropy_from_sum(5,sumX_log2X(X1)), norm_entropy_from_sum(5,sumX_log2X(X2)), norm_entropy_from_sum(5000,sumX_log2X(X3)) FROM test1;


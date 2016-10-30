# entropy-postgresql
Aggregate user-defined function to compute Shannon's entropy in PostgreSQL (>=9.4).

The functions are implemented in C and compute the entropy from counts of events.
The probabilities are estimated by maximum likelihood from the counts.

## Install

1. Compile and install the entropy functions in postgres

  ```bash
  make && sudo make install
  ```
2. Create the entropy functions in the postgres databases where you want to use them

  ```bash
  psql -d <database> -f create_functions.sql
  ```

## How to use the functions

The file `test_functions.sql` contains examples using the functions to query a temporary table.

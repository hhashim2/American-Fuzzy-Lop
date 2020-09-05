# American-Fuzzy-Lop
The American Fuzzy Lop is a fuzzer which is efficient in checking code coverage. A fuzzer is a fuzz testing technique in software development which provides invalid, unexpected, and random data to a program. This program is then monitored for memory leakages, crashes, and corner test-cases.
## Program Description
This program, written in conjunction with Raja Rafey, provides a front-end for basic statistic calculations for a given collection of vectors as input. An example invocation is as follows: 

`$ printf "1 2 3\n4 5 6\n7 8 9\n" | ./src/ds mean 0 mean 1 mean 2`

`4 5 6`

This shell command feeds the input: 

`1 2 3`

`4 5 6`

`7 8 9`

which then prints the mean of the individual three columns.

## American Fuzzy Lop
AFL is a grey-box fuzzing technique which utilizes instrumentation compiled in to its target code. 

To instrument the code, the following command is run in the directory where `main.c` resides:

`afl-gcc –Wall stat.h stat.c main.c –o ds`

Seed Cases are then piped into the fuzzer program as inputs in the form of matrices in different files. The first input is the best case scenario where the program is given an n * n matrix, which the front-end converts to column vectors, and feeds into the stat calculator. The following inputs then provides incomplete matrices, which are bound to fail because the traversals of such matrices result in column vectors which are unevenly distributed as a result of transposing said matrix.

To run the fuzzer, a directory for `afl/out` and `afl/in` is created after which we can execute the command to run the fuzzer, assuming that our executable resides in /src/ds:

`afl-fuzz –i afl/in –o afl/out ds –x -ly`

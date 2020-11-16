#!/bin/bash

# run 1, 2, 4, 8 threads
# with 100, 1000, 100000 iterations
# no locks
./lab2_add             --iterations 100
./lab2_add             --iterations 1000
./lab2_add             --iterations 10000
./lab2_add             --iterations 100000
./lab2_add --threads 2 --iterations 100
./lab2_add --threads 2 --iterations 1000
./lab2_add --threads 2 --iterations 10000
./lab2_add --threads 2 --iterations 100000
./lab2_add --threads 4 --iterations 100
./lab2_add --threads 4 --iterations 1000
./lab2_add --threads 4 --iterations 10000
./lab2_add --threads 4 --iterations 100000
./lab2_add --threads 8 --iterations 100
./lab2_add --threads 8 --iterations 1000
./lab2_add --threads 8 --iterations 10000
./lab2_add --threads 8 --iterations 100000

# run 2, 4, 8, 12 threads
# with 10, 20, 40, 80, 100, 1000, 10000, 100000 iterations
# use yield
./lab2_add --threads 2  --iterations 10 	--yield
./lab2_add --threads 2  --iterations 20 	--yield
./lab2_add --threads 2  --iterations 40 	--yield
./lab2_add --threads 2  --iterations 80 	--yield
./lab2_add --threads 2  --iterations 100 	--yield
./lab2_add --threads 2  --iterations 1000 	--yield
./lab2_add --threads 2  --iterations 10000 	--yield
./lab2_add --threads 2  --iterations 100000 --yield
./lab2_add --threads 4  --iterations 10 	--yield
./lab2_add --threads 4  --iterations 20 	--yield
./lab2_add --threads 4  --iterations 40 	--yield
./lab2_add --threads 4  --iterations 80 	--yield
./lab2_add --threads 4  --iterations 100 	--yield
./lab2_add --threads 4  --iterations 1000 	--yield
./lab2_add --threads 4  --iterations 10000 	--yield
./lab2_add --threads 4  --iterations 100000 --yield
./lab2_add --threads 8  --iterations 10 	--yield
./lab2_add --threads 8  --iterations 20 	--yield
./lab2_add --threads 8  --iterations 40 	--yield
./lab2_add --threads 8  --iterations 80 	--yield
./lab2_add --threads 8  --iterations 100 	--yield
./lab2_add --threads 8  --iterations 1000 	--yield
./lab2_add --threads 8  --iterations 10000 	--yield
./lab2_add --threads 8  --iterations 100000 --yield
./lab2_add --threads 12 --iterations 10 	--yield
./lab2_add --threads 12 --iterations 20 	--yield
./lab2_add --threads 12 --iterations 40 	--yield
./lab2_add --threads 12 --iterations 80 	--yield
./lab2_add --threads 12 --iterations 100 	--yield
./lab2_add --threads 12 --iterations 1000 	--yield
./lab2_add --threads 12 --iterations 10000 	--yield
./lab2_add --threads 12 --iterations 100000 --yield

# run 100, 1000, 10000, 100000 threads
# with 2 and 8 iterations
# compare the use of yield to no yield
./lab2_add --threads 2 --iterations 100 	--yield
./lab2_add --threads 2 --iterations 1000 	--yield
./lab2_add --threads 2 --iterations 10000 	--yield
./lab2_add --threads 2 --iterations 100000 	--yield
./lab2_add --threads 8 --iterations 100 	--yield
./lab2_add --threads 8 --iterations 1000 	--yield
./lab2_add --threads 8 --iterations 10000 	--yield
./lab2_add --threads 8 --iterations 100000 	--yield
./lab2_add --threads 2 --iterations 100
./lab2_add --threads 2 --iterations 1000
./lab2_add --threads 2 --iterations 10000
./lab2_add --threads 2 --iterations 100000
./lab2_add --threads 8 --iterations 100
./lab2_add --threads 8 --iterations 1000
./lab2_add --threads 8 --iterations 10000
./lab2_add --threads 8 --iterations 100000

# run 2, 4, 8, 12 threads
# run with each lock with yield
# with 10000 iterations for CAS and Mutex
./lab2_add --threads 2  --iterations 10000 	--yield --sync m
./lab2_add --threads 2  --iterations 10000 	--yield --sync c
./lab2_add --threads 4  --iterations 10000 	--yield --sync m
./lab2_add --threads 4  --iterations 10000 	--yield --sync c
./lab2_add --threads 8  --iterations 10000 	--yield --sync m
./lab2_add --threads 8  --iterations 10000 	--yield --sync c
./lab2_add --threads 12 --iterations 10000 	--yield --sync m
./lab2_add --threads 12 --iterations 10000 	--yield --sync c
# with 1000 for spin
./lab2_add --threads 2  --iterations 1000 	--yield --sync s
./lab2_add --threads 4  --iterations 1000 	--yield --sync s
./lab2_add --threads 8  --iterations 1000 	--yield --sync s
./lab2_add --threads 12 --iterations 1000 	--yield --sync s

# run 1, 2, 4, 8, 12 threads
# with 10000 iterations
# run with each lock
./lab2_add --iterations 10000
./lab2_add --iterations 10000 --sync m
./lab2_add --iterations 10000 --sync s #or 1000
./lab2_add --iterations 10000 --sync c
./lab2_add --threads 2 --iterations 10000
./lab2_add --threads 2 --iterations 10000 --sync m
./lab2_add --threads 2 --iterations 10000 --sync s
./lab2_add --threads 2 --iterations 10000 --sync c
./lab2_add --threads 4 --iterations 10000
./lab2_add --threads 4 --iterations 10000 --sync m
./lab2_add --threads 4 --iterations 10000 --sync s
./lab2_add --threads 4 --iterations 10000 --sync c
./lab2_add --threads 8 --iterations 10000
./lab2_add --threads 8 --iterations 10000 --sync m
./lab2_add --threads 8 --iterations 10000 --sync s
./lab2_add --threads 8 --iterations 10000 --sync c
./lab2_add --threads 12 --iterations 10000
./lab2_add --threads 12 --iterations 10000 --sync m
./lab2_add --threads 12 --iterations 10000 --sync s
./lab2_add --threads 12 --iterations 10000 --sync c

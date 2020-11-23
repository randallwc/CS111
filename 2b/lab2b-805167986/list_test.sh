#!/bin/bash

# lab2b_1.png
# Mutex synchronized list operations, 
# 1,000 iterations, 
# 1,2,4,8,12,16,24 threads

./lab2_list --threads 1  --iterations 1000 --sync m
./lab2_list --threads 2  --iterations 1000 --sync m
./lab2_list --threads 4  --iterations 1000 --sync m
./lab2_list --threads 8  --iterations 1000 --sync m
./lab2_list --threads 12 --iterations 1000 --sync m
./lab2_list --threads 16 --iterations 1000 --sync m
./lab2_list --threads 24 --iterations 1000 --sync m

# Spin-lock synchronized list operations, 
# 1,000 iterations, 
# 1,2,4,8,12,16,24 threads

./lab2_list --threads 1  --iterations 1000 --sync s
./lab2_list --threads 2  --iterations 1000 --sync s
./lab2_list --threads 4  --iterations 1000 --sync s
./lab2_list --threads 8  --iterations 1000 --sync s
./lab2_list --threads 12 --iterations 1000 --sync s
./lab2_list --threads 16 --iterations 1000 --sync s
./lab2_list --threads 24 --iterations 1000 --sync s

# lab2b_2.png
# Run the list mutex test again for 
# 1,000 iterations and 
# 1, 2, 4, 8, 16, 24 threads

./lab2_list --threads 1  --iterations 1000 --sync m
./lab2_list --threads 2  --iterations 1000 --sync m
./lab2_list --threads 4  --iterations 1000 --sync m
./lab2_list --threads 8  --iterations 1000 --sync m
./lab2_list --threads 16 --iterations 1000 --sync m
./lab2_list --threads 24 --iterations 1000 --sync m

# lab2b_3.png
# Run your program with --yield=id, 
# 4 lists, 
# 1,4,8,12,16 threads, and 
# 1, 2, 4, 8, 16 iterations (and no synchronization) to see how many iterations it takes 
# to reliably fail (and make sure your Makefile expects some of these tests to fail).

./lab2_list --threads 1 	--iterations 1  --lists 4 --yield id
./lab2_list --threads 4 	--iterations 1  --lists 4 --yield id
./lab2_list --threads 8 	--iterations 1  --lists 4 --yield id
./lab2_list --threads 12 	--iterations 1  --lists 4 --yield id
./lab2_list --threads 16 	--iterations 1  --lists 4 --yield id
./lab2_list --threads 1 	--iterations 2  --lists 4 --yield id
./lab2_list --threads 4 	--iterations 2  --lists 4 --yield id
./lab2_list --threads 8 	--iterations 2  --lists 4 --yield id
./lab2_list --threads 12 	--iterations 2  --lists 4 --yield id
./lab2_list --threads 16 	--iterations 2  --lists 4 --yield id
./lab2_list --threads 1 	--iterations 4  --lists 4 --yield id
./lab2_list --threads 4 	--iterations 4  --lists 4 --yield id
./lab2_list --threads 8 	--iterations 4  --lists 4 --yield id
./lab2_list --threads 12 	--iterations 4  --lists 4 --yield id
./lab2_list --threads 16 	--iterations 4  --lists 4 --yield id
./lab2_list --threads 1 	--iterations 8  --lists 4 --yield id
./lab2_list --threads 4 	--iterations 8  --lists 4 --yield id
./lab2_list --threads 8 	--iterations 8  --lists 4 --yield id
./lab2_list --threads 12 	--iterations 8  --lists 4 --yield id
./lab2_list --threads 16 	--iterations 8  --lists 4 --yield id
./lab2_list --threads 1 	--iterations 16 --lists 4 --yield id
./lab2_list --threads 4 	--iterations 16 --lists 4 --yield id
./lab2_list --threads 8 	--iterations 16 --lists 4 --yield id
./lab2_list --threads 12 	--iterations 16 --lists 4 --yield id
./lab2_list --threads 16 	--iterations 16 --lists 4 --yield id

# Run your program with --yield=id, 
# 4 lists, 
# 1,4,8,12,16 threads, and 
# 10, 20, 40, 80 iterations, 
# --sync=s and --sync=m to confirm that updates are 
# now properly protected (i.e., all runs succeeded).

./lab2_list --threads 1 	--iterations 10 --lists 4 --yield id --sync m
./lab2_list --threads 4 	--iterations 10 --lists 4 --yield id --sync m
./lab2_list --threads 8 	--iterations 10 --lists 4 --yield id --sync m
./lab2_list --threads 12 	--iterations 10 --lists 4 --yield id --sync m
./lab2_list --threads 16 	--iterations 10 --lists 4 --yield id --sync m
./lab2_list --threads 1 	--iterations 20 --lists 4 --yield id --sync m
./lab2_list --threads 4 	--iterations 20 --lists 4 --yield id --sync m
./lab2_list --threads 8 	--iterations 20 --lists 4 --yield id --sync m
./lab2_list --threads 12 	--iterations 20 --lists 4 --yield id --sync m
./lab2_list --threads 16 	--iterations 20 --lists 4 --yield id --sync m
./lab2_list --threads 1 	--iterations 40 --lists 4 --yield id --sync m
./lab2_list --threads 4 	--iterations 40 --lists 4 --yield id --sync m
./lab2_list --threads 8 	--iterations 40 --lists 4 --yield id --sync m
./lab2_list --threads 12 	--iterations 40 --lists 4 --yield id --sync m
./lab2_list --threads 16 	--iterations 40 --lists 4 --yield id --sync m
./lab2_list --threads 1 	--iterations 80 --lists 4 --yield id --sync m
./lab2_list --threads 4 	--iterations 80 --lists 4 --yield id --sync m
./lab2_list --threads 8 	--iterations 80 --lists 4 --yield id --sync m
./lab2_list --threads 12 	--iterations 80 --lists 4 --yield id --sync m
./lab2_list --threads 16 	--iterations 80 --lists 4 --yield id --sync m
./lab2_list --threads 1 	--iterations 10 --lists 4 --yield id --sync s
./lab2_list --threads 4 	--iterations 10 --lists 4 --yield id --sync s
./lab2_list --threads 8 	--iterations 10 --lists 4 --yield id --sync s
./lab2_list --threads 12 	--iterations 10 --lists 4 --yield id --sync s
./lab2_list --threads 16 	--iterations 10 --lists 4 --yield id --sync s
./lab2_list --threads 1 	--iterations 20 --lists 4 --yield id --sync s
./lab2_list --threads 4 	--iterations 20 --lists 4 --yield id --sync s
./lab2_list --threads 8 	--iterations 20 --lists 4 --yield id --sync s
./lab2_list --threads 12 	--iterations 20 --lists 4 --yield id --sync s
./lab2_list --threads 16 	--iterations 20 --lists 4 --yield id --sync s
./lab2_list --threads 1 	--iterations 40 --lists 4 --yield id --sync s
./lab2_list --threads 4 	--iterations 40 --lists 4 --yield id --sync s
./lab2_list --threads 8 	--iterations 40 --lists 4 --yield id --sync s
./lab2_list --threads 12 	--iterations 40 --lists 4 --yield id --sync s
./lab2_list --threads 16 	--iterations 40 --lists 4 --yield id --sync s
./lab2_list --threads 1 	--iterations 80 --lists 4 --yield id --sync s
./lab2_list --threads 4 	--iterations 80 --lists 4 --yield id --sync s
./lab2_list --threads 8 	--iterations 80 --lists 4 --yield id --sync s
./lab2_list --threads 12 	--iterations 80 --lists 4 --yield id --sync s
./lab2_list --threads 16 	--iterations 80 --lists 4 --yield id --sync s

# without yields, 
# for 1000 iterations, 
# 1,2,4,8,12 threads, and 
# 1,4,8,16 lists.
# lab2b_4.png(symc=m)

./lab2_list --threads 1 	--iterations 1000 --lists 1  --sync m
./lab2_list --threads 2 	--iterations 1000 --lists 1  --sync m
./lab2_list --threads 4 	--iterations 1000 --lists 1  --sync m
./lab2_list --threads 8 	--iterations 1000 --lists 1  --sync m
./lab2_list --threads 12 	--iterations 1000 --lists 1  --sync m
./lab2_list --threads 1 	--iterations 1000 --lists 4  --sync m
./lab2_list --threads 2 	--iterations 1000 --lists 4  --sync m
./lab2_list --threads 4 	--iterations 1000 --lists 4  --sync m
./lab2_list --threads 8 	--iterations 1000 --lists 4  --sync m
./lab2_list --threads 12 	--iterations 1000 --lists 4  --sync m
./lab2_list --threads 1 	--iterations 1000 --lists 8  --sync m
./lab2_list --threads 2 	--iterations 1000 --lists 8  --sync m
./lab2_list --threads 4 	--iterations 1000 --lists 8  --sync m
./lab2_list --threads 8 	--iterations 1000 --lists 8  --sync m
./lab2_list --threads 12 	--iterations 1000 --lists 8  --sync m
./lab2_list --threads 1 	--iterations 1000 --lists 16 --sync m
./lab2_list --threads 2 	--iterations 1000 --lists 16 --sync m
./lab2_list --threads 4 	--iterations 1000 --lists 16 --sync m
./lab2_list --threads 8 	--iterations 1000 --lists 16 --sync m
./lab2_list --threads 12 	--iterations 1000 --lists 16 --sync m

# lab2b_5.png(sync=s)

./lab2_list --threads 1 	--iterations 1000 --lists 1  --sync s
./lab2_list --threads 2 	--iterations 1000 --lists 1  --sync s
./lab2_list --threads 4 	--iterations 1000 --lists 1  --sync s
./lab2_list --threads 8 	--iterations 1000 --lists 1  --sync s
./lab2_list --threads 12 	--iterations 1000 --lists 1  --sync s
./lab2_list --threads 1 	--iterations 1000 --lists 4  --sync s
./lab2_list --threads 2 	--iterations 1000 --lists 4  --sync s
./lab2_list --threads 4 	--iterations 1000 --lists 4  --sync s
./lab2_list --threads 8 	--iterations 1000 --lists 4  --sync s
./lab2_list --threads 12 	--iterations 1000 --lists 4  --sync s
./lab2_list --threads 1 	--iterations 1000 --lists 8  --sync s
./lab2_list --threads 2 	--iterations 1000 --lists 8  --sync s
./lab2_list --threads 4 	--iterations 1000 --lists 8  --sync s
./lab2_list --threads 8 	--iterations 1000 --lists 8  --sync s
./lab2_list --threads 12 	--iterations 1000 --lists 8  --sync s
./lab2_list --threads 1 	--iterations 1000 --lists 16 --sync s
./lab2_list --threads 2 	--iterations 1000 --lists 16 --sync s
./lab2_list --threads 4 	--iterations 1000 --lists 16 --sync s
./lab2_list --threads 8 	--iterations 1000 --lists 16 --sync s
./lab2_list --threads 12 	--iterations 1000 --lists 16 --sync s
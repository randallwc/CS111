#!/bin/bash

#run with 10, 100, 1000, 10000, 20000 iterations
./lab2_list --iterations 10
./lab2_list --iterations 100
./lab2_list --iterations 1000
./lab2_list --iterations 10000
./lab2_list --iterations 20000

#run with 2, 4, 8, 12 threads
#with 1, 10, 100, 1000 iterations
./lab2_list --threads 2  --iterations 1
./lab2_list --threads 2  --iterations 10
./lab2_list --threads 2  --iterations 100
./lab2_list --threads 2  --iterations 1000
./lab2_list --threads 4  --iterations 1
./lab2_list --threads 4  --iterations 10
./lab2_list --threads 4  --iterations 100
./lab2_list --threads 4  --iterations 1000
./lab2_list --threads 8  --iterations 1
./lab2_list --threads 8  --iterations 10
./lab2_list --threads 8  --iterations 100
./lab2_list --threads 8  --iterations 1000
./lab2_list --threads 12 --iterations 1
./lab2_list --threads 12 --iterations 10
./lab2_list --threads 12 --iterations 100
./lab2_list --threads 12 --iterations 1000

#yield with i, d, il, dl
#run with 2, 4, 8, 12 threads
#with 1, 2, 4, 8, 16, 32 iterations
./lab2_list --threads 2  --iterations 1 	--yield i
./lab2_list --threads 2  --iterations 2 	--yield i
./lab2_list --threads 2  --iterations 4 	--yield i
./lab2_list --threads 2  --iterations 8 	--yield i
./lab2_list --threads 2  --iterations 16 	--yield i
./lab2_list --threads 2  --iterations 32 	--yield i
./lab2_list --threads 4  --iterations 1 	--yield i
./lab2_list --threads 4  --iterations 2 	--yield i
./lab2_list --threads 4  --iterations 4 	--yield i
./lab2_list --threads 4  --iterations 8 	--yield i
./lab2_list --threads 4  --iterations 16 	--yield i
./lab2_list --threads 4  --iterations 32 	--yield i
./lab2_list --threads 8  --iterations 1 	--yield i
./lab2_list --threads 8  --iterations 2 	--yield i
./lab2_list --threads 8  --iterations 4 	--yield i
./lab2_list --threads 8  --iterations 8 	--yield i
./lab2_list --threads 8  --iterations 16 	--yield i
./lab2_list --threads 8  --iterations 32 	--yield i
./lab2_list --threads 12 --iterations 1 	--yield i
./lab2_list --threads 12 --iterations 2 	--yield i
./lab2_list --threads 12 --iterations 4 	--yield i
./lab2_list --threads 12 --iterations 8 	--yield i
./lab2_list --threads 12 --iterations 16 	--yield i
./lab2_list --threads 12 --iterations 32 	--yield i
./lab2_list --threads 2  --iterations 1 	--yield d
./lab2_list --threads 2  --iterations 2 	--yield d
./lab2_list --threads 2  --iterations 4 	--yield d
./lab2_list --threads 2  --iterations 8 	--yield d
./lab2_list --threads 2  --iterations 16 	--yield d
./lab2_list --threads 2  --iterations 32 	--yield d
./lab2_list --threads 4  --iterations 1 	--yield d
./lab2_list --threads 4  --iterations 2 	--yield d
./lab2_list --threads 4  --iterations 4 	--yield d
./lab2_list --threads 4  --iterations 8 	--yield d
./lab2_list --threads 4  --iterations 16 	--yield d
./lab2_list --threads 4  --iterations 32 	--yield d
./lab2_list --threads 8  --iterations 1 	--yield d
./lab2_list --threads 8  --iterations 2 	--yield d
./lab2_list --threads 8  --iterations 4 	--yield d
./lab2_list --threads 8  --iterations 8 	--yield d
./lab2_list --threads 8  --iterations 16 	--yield d
./lab2_list --threads 8  --iterations 32 	--yield d
./lab2_list --threads 12 --iterations 1 	--yield d
./lab2_list --threads 12 --iterations 2 	--yield d
./lab2_list --threads 12 --iterations 4 	--yield d
./lab2_list --threads 12 --iterations 8 	--yield d
./lab2_list --threads 12 --iterations 16 	--yield d
./lab2_list --threads 12 --iterations 32 	--yield d
./lab2_list --threads 2  --iterations 1 	--yield il
./lab2_list --threads 2  --iterations 2 	--yield il
./lab2_list --threads 2  --iterations 4 	--yield il
./lab2_list --threads 2  --iterations 8 	--yield il
./lab2_list --threads 2  --iterations 16 	--yield il
./lab2_list --threads 2  --iterations 32 	--yield il
./lab2_list --threads 4  --iterations 1 	--yield il
./lab2_list --threads 4  --iterations 2 	--yield il
./lab2_list --threads 4  --iterations 4 	--yield il
./lab2_list --threads 4  --iterations 8 	--yield il
./lab2_list --threads 4  --iterations 16 	--yield il
./lab2_list --threads 4  --iterations 32 	--yield il
./lab2_list --threads 8  --iterations 1 	--yield il
./lab2_list --threads 8  --iterations 2 	--yield il
./lab2_list --threads 8  --iterations 4 	--yield il
./lab2_list --threads 8  --iterations 8 	--yield il
./lab2_list --threads 8  --iterations 16 	--yield il
./lab2_list --threads 8  --iterations 32 	--yield il
./lab2_list --threads 12 --iterations 1 	--yield il
./lab2_list --threads 12 --iterations 2 	--yield il
./lab2_list --threads 12 --iterations 4 	--yield il
./lab2_list --threads 12 --iterations 8 	--yield il
./lab2_list --threads 12 --iterations 16 	--yield il
./lab2_list --threads 12 --iterations 32 	--yield il
./lab2_list --threads 2  --iterations 1 	--yield dl
./lab2_list --threads 2  --iterations 2 	--yield dl
./lab2_list --threads 2  --iterations 4 	--yield dl
./lab2_list --threads 2  --iterations 8 	--yield dl
./lab2_list --threads 2  --iterations 16 	--yield dl
./lab2_list --threads 2  --iterations 32 	--yield dl
./lab2_list --threads 4  --iterations 1 	--yield dl
./lab2_list --threads 4  --iterations 2 	--yield dl
./lab2_list --threads 4  --iterations 4 	--yield dl
./lab2_list --threads 4  --iterations 8 	--yield dl
./lab2_list --threads 4  --iterations 16 	--yield dl
./lab2_list --threads 4  --iterations 32 	--yield dl
./lab2_list --threads 8  --iterations 1 	--yield dl
./lab2_list --threads 8  --iterations 2 	--yield dl
./lab2_list --threads 8  --iterations 4 	--yield dl
./lab2_list --threads 8  --iterations 8 	--yield dl
./lab2_list --threads 8  --iterations 16 	--yield dl
./lab2_list --threads 8  --iterations 32 	--yield dl
./lab2_list --threads 12 --iterations 1 	--yield dl
./lab2_list --threads 12 --iterations 2 	--yield dl
./lab2_list --threads 12 --iterations 4 	--yield dl
./lab2_list --threads 12 --iterations 8 	--yield dl
./lab2_list --threads 12 --iterations 16 	--yield dl
./lab2_list --threads 12 --iterations 32 	--yield dl

#yield with i
#sync with mutex and spin
#run with 2, 4, 8, 12 threads
#with 1, 2, 4, 8, 16, 32 iterations
./lab2_list --threads 2  --iterations 1 	--yield i --sync m
./lab2_list --threads 2  --iterations 2 	--yield i --sync m
./lab2_list --threads 2  --iterations 4 	--yield i --sync m
./lab2_list --threads 2  --iterations 8 	--yield i --sync m
./lab2_list --threads 2  --iterations 16 	--yield i --sync m
./lab2_list --threads 2  --iterations 32 	--yield i --sync m
./lab2_list --threads 4  --iterations 1 	--yield i --sync m
./lab2_list --threads 4  --iterations 2 	--yield i --sync m
./lab2_list --threads 4  --iterations 4 	--yield i --sync m
./lab2_list --threads 4  --iterations 8 	--yield i --sync m
./lab2_list --threads 4  --iterations 16 	--yield i --sync m
./lab2_list --threads 4  --iterations 32 	--yield i --sync m
./lab2_list --threads 8  --iterations 1 	--yield i --sync m
./lab2_list --threads 8  --iterations 2 	--yield i --sync m
./lab2_list --threads 8  --iterations 4 	--yield i --sync m
./lab2_list --threads 8  --iterations 8 	--yield i --sync m
./lab2_list --threads 8  --iterations 16 	--yield i --sync m
./lab2_list --threads 8  --iterations 32 	--yield i --sync m
./lab2_list --threads 12 --iterations 1 	--yield i --sync m
./lab2_list --threads 12 --iterations 2 	--yield i --sync m
./lab2_list --threads 12 --iterations 4 	--yield i --sync m
./lab2_list --threads 12 --iterations 8 	--yield i --sync m
./lab2_list --threads 12 --iterations 16 	--yield i --sync m
./lab2_list --threads 12 --iterations 32 	--yield i --sync m

./lab2_list --threads 2  --iterations 1 	--yield i --sync s
./lab2_list --threads 2  --iterations 2 	--yield i --sync s
./lab2_list --threads 2  --iterations 4 	--yield i --sync s
./lab2_list --threads 2  --iterations 8 	--yield i --sync s
./lab2_list --threads 2  --iterations 16 	--yield i --sync s
./lab2_list --threads 2  --iterations 32 	--yield i --sync s
./lab2_list --threads 4  --iterations 1 	--yield i --sync s
./lab2_list --threads 4  --iterations 2 	--yield i --sync s
./lab2_list --threads 4  --iterations 4 	--yield i --sync s
./lab2_list --threads 4  --iterations 8 	--yield i --sync s
./lab2_list --threads 4  --iterations 16 	--yield i --sync s
./lab2_list --threads 4  --iterations 32 	--yield i --sync s
./lab2_list --threads 8  --iterations 1 	--yield i --sync s
./lab2_list --threads 8  --iterations 2 	--yield i --sync s
./lab2_list --threads 8  --iterations 4 	--yield i --sync s
./lab2_list --threads 8  --iterations 8 	--yield i --sync s
./lab2_list --threads 8  --iterations 16 	--yield i --sync s
./lab2_list --threads 8  --iterations 32 	--yield i --sync s
./lab2_list --threads 12 --iterations 1 	--yield i --sync s
./lab2_list --threads 12 --iterations 2 	--yield i --sync s
./lab2_list --threads 12 --iterations 4 	--yield i --sync s
./lab2_list --threads 12 --iterations 8 	--yield i --sync s
./lab2_list --threads 12 --iterations 16 	--yield i --sync s
./lab2_list --threads 12 --iterations 32 	--yield i --sync s

#yield with d
#sync with mutex and spin
#run with 2, 4, 8, 12 threads
#with 1, 2, 4, 8, 16, 32 iterations
./lab2_list --threads 2  --iterations 1 	--yield d --sync m
./lab2_list --threads 2  --iterations 2 	--yield d --sync m
./lab2_list --threads 2  --iterations 4 	--yield d --sync m
./lab2_list --threads 2  --iterations 8 	--yield d --sync m
./lab2_list --threads 2  --iterations 16 	--yield d --sync m
./lab2_list --threads 2  --iterations 32 	--yield d --sync m
./lab2_list --threads 4  --iterations 1 	--yield d --sync m
./lab2_list --threads 4  --iterations 2 	--yield d --sync m
./lab2_list --threads 4  --iterations 4 	--yield d --sync m
./lab2_list --threads 4  --iterations 8 	--yield d --sync m
./lab2_list --threads 4  --iterations 16 	--yield d --sync m
./lab2_list --threads 4  --iterations 32 	--yield d --sync m
./lab2_list --threads 8  --iterations 1 	--yield d --sync m
./lab2_list --threads 8  --iterations 2 	--yield d --sync m
./lab2_list --threads 8  --iterations 4 	--yield d --sync m
./lab2_list --threads 8  --iterations 8 	--yield d --sync m
./lab2_list --threads 8  --iterations 16 	--yield d --sync m
./lab2_list --threads 8  --iterations 32 	--yield d --sync m
./lab2_list --threads 12 --iterations 1 	--yield d --sync m
./lab2_list --threads 12 --iterations 2 	--yield d --sync m
./lab2_list --threads 12 --iterations 4 	--yield d --sync m
./lab2_list --threads 12 --iterations 8 	--yield d --sync m
./lab2_list --threads 12 --iterations 16 	--yield d --sync m
./lab2_list --threads 12 --iterations 32 	--yield d --sync m

./lab2_list --threads 2  --iterations 1 	--yield d --sync s
./lab2_list --threads 2  --iterations 2 	--yield d --sync s
./lab2_list --threads 2  --iterations 4 	--yield d --sync s
./lab2_list --threads 2  --iterations 8 	--yield d --sync s
./lab2_list --threads 2  --iterations 16 	--yield d --sync s
./lab2_list --threads 2  --iterations 32 	--yield d --sync s
./lab2_list --threads 4  --iterations 1 	--yield d --sync s
./lab2_list --threads 4  --iterations 2 	--yield d --sync s
./lab2_list --threads 4  --iterations 4 	--yield d --sync s
./lab2_list --threads 4  --iterations 8 	--yield d --sync s
./lab2_list --threads 4  --iterations 16 	--yield d --sync s
./lab2_list --threads 4  --iterations 32 	--yield d --sync s
./lab2_list --threads 8  --iterations 1 	--yield d --sync s
./lab2_list --threads 8  --iterations 2 	--yield d --sync s
./lab2_list --threads 8  --iterations 4 	--yield d --sync s
./lab2_list --threads 8  --iterations 8 	--yield d --sync s
./lab2_list --threads 8  --iterations 16 	--yield d --sync s
./lab2_list --threads 8  --iterations 32 	--yield d --sync s
./lab2_list --threads 12 --iterations 1 	--yield d --sync s
./lab2_list --threads 12 --iterations 2 	--yield d --sync s
./lab2_list --threads 12 --iterations 4 	--yield d --sync s
./lab2_list --threads 12 --iterations 8 	--yield d --sync s
./lab2_list --threads 12 --iterations 16 	--yield d --sync s
./lab2_list --threads 12 --iterations 32 	--yield d --sync s

#yield with il
#sync with mutex and spin
#run with 2, 4, 8, 12 threads
#with 1, 2, 4, 8, 16, 32 iterations
./lab2_list --threads 2  --iterations 1 	--yield il --sync m
./lab2_list --threads 2  --iterations 2 	--yield il --sync m
./lab2_list --threads 2  --iterations 4 	--yield il --sync m
./lab2_list --threads 2  --iterations 8 	--yield il --sync m
./lab2_list --threads 2  --iterations 16 	--yield il --sync m
./lab2_list --threads 2  --iterations 32 	--yield il --sync m
./lab2_list --threads 4  --iterations 1 	--yield il --sync m
./lab2_list --threads 4  --iterations 2 	--yield il --sync m
./lab2_list --threads 4  --iterations 4 	--yield il --sync m
./lab2_list --threads 4  --iterations 8 	--yield il --sync m
./lab2_list --threads 4  --iterations 16 	--yield il --sync m
./lab2_list --threads 4  --iterations 32 	--yield il --sync m
./lab2_list --threads 8  --iterations 1 	--yield il --sync m
./lab2_list --threads 8  --iterations 2 	--yield il --sync m
./lab2_list --threads 8  --iterations 4 	--yield il --sync m
./lab2_list --threads 8  --iterations 8 	--yield il --sync m
./lab2_list --threads 8  --iterations 16 	--yield il --sync m
./lab2_list --threads 8  --iterations 32 	--yield il --sync m
./lab2_list --threads 12 --iterations 1 	--yield il --sync m
./lab2_list --threads 12 --iterations 2 	--yield il --sync m
./lab2_list --threads 12 --iterations 4 	--yield il --sync m
./lab2_list --threads 12 --iterations 8 	--yield il --sync m
./lab2_list --threads 12 --iterations 16 	--yield il --sync m
./lab2_list --threads 12 --iterations 32 	--yield il --sync m

./lab2_list --threads 2  --iterations 1 	--yield il --sync s
./lab2_list --threads 2  --iterations 2 	--yield il --sync s
./lab2_list --threads 2  --iterations 4 	--yield il --sync s
./lab2_list --threads 2  --iterations 8 	--yield il --sync s
./lab2_list --threads 2  --iterations 16 	--yield il --sync s
./lab2_list --threads 2  --iterations 32 	--yield il --sync s
./lab2_list --threads 4  --iterations 1 	--yield il --sync s
./lab2_list --threads 4  --iterations 2 	--yield il --sync s
./lab2_list --threads 4  --iterations 4 	--yield il --sync s
./lab2_list --threads 4  --iterations 8 	--yield il --sync s
./lab2_list --threads 4  --iterations 16 	--yield il --sync s
./lab2_list --threads 4  --iterations 32 	--yield il --sync s
./lab2_list --threads 8  --iterations 1 	--yield il --sync s
./lab2_list --threads 8  --iterations 2 	--yield il --sync s
./lab2_list --threads 8  --iterations 4 	--yield il --sync s
./lab2_list --threads 8  --iterations 8 	--yield il --sync s
./lab2_list --threads 8  --iterations 16 	--yield il --sync s
./lab2_list --threads 8  --iterations 32 	--yield il --sync s
./lab2_list --threads 12 --iterations 1 	--yield il --sync s
./lab2_list --threads 12 --iterations 2 	--yield il --sync s
./lab2_list --threads 12 --iterations 4 	--yield il --sync s
./lab2_list --threads 12 --iterations 8 	--yield il --sync s
./lab2_list --threads 12 --iterations 16 	--yield il --sync s
./lab2_list --threads 12 --iterations 32 	--yield il --sync s

#yield with dl
#sync with mutex and spin
#run with 2, 4, 8, 12 threads
#with 1, 2, 4, 8, 16, 32 iterations
./lab2_list --threads 2  --iterations 1 	--yield dl --sync m
./lab2_list --threads 2  --iterations 2 	--yield dl --sync m
./lab2_list --threads 2  --iterations 4 	--yield dl --sync m
./lab2_list --threads 2  --iterations 8 	--yield dl --sync m
./lab2_list --threads 2  --iterations 16 	--yield dl --sync m
./lab2_list --threads 2  --iterations 32 	--yield dl --sync m
./lab2_list --threads 4  --iterations 1 	--yield dl --sync m
./lab2_list --threads 4  --iterations 2 	--yield dl --sync m
./lab2_list --threads 4  --iterations 4 	--yield dl --sync m
./lab2_list --threads 4  --iterations 8 	--yield dl --sync m
./lab2_list --threads 4  --iterations 16 	--yield dl --sync m
./lab2_list --threads 4  --iterations 32 	--yield dl --sync m
./lab2_list --threads 8  --iterations 1 	--yield dl --sync m
./lab2_list --threads 8  --iterations 2 	--yield dl --sync m
./lab2_list --threads 8  --iterations 4 	--yield dl --sync m
./lab2_list --threads 8  --iterations 8 	--yield dl --sync m
./lab2_list --threads 8  --iterations 16 	--yield dl --sync m
./lab2_list --threads 8  --iterations 32 	--yield dl --sync m
./lab2_list --threads 12 --iterations 1 	--yield dl --sync m
./lab2_list --threads 12 --iterations 2 	--yield dl --sync m
./lab2_list --threads 12 --iterations 4 	--yield dl --sync m
./lab2_list --threads 12 --iterations 8 	--yield dl --sync m
./lab2_list --threads 12 --iterations 16 	--yield dl --sync m
./lab2_list --threads 12 --iterations 32 	--yield dl --sync m

./lab2_list --threads 2  --iterations 1 	--yield dl --sync s
./lab2_list --threads 2  --iterations 2 	--yield dl --sync s
./lab2_list --threads 2  --iterations 4 	--yield dl --sync s
./lab2_list --threads 2  --iterations 8 	--yield dl --sync s
./lab2_list --threads 2  --iterations 16 	--yield dl --sync s
./lab2_list --threads 2  --iterations 32 	--yield dl --sync s
./lab2_list --threads 4  --iterations 1 	--yield dl --sync s
./lab2_list --threads 4  --iterations 2 	--yield dl --sync s
./lab2_list --threads 4  --iterations 4 	--yield dl --sync s
./lab2_list --threads 4  --iterations 8 	--yield dl --sync s
./lab2_list --threads 4  --iterations 16 	--yield dl --sync s
./lab2_list --threads 4  --iterations 32 	--yield dl --sync s
./lab2_list --threads 8  --iterations 1 	--yield dl --sync s
./lab2_list --threads 8  --iterations 2 	--yield dl --sync s
./lab2_list --threads 8  --iterations 4 	--yield dl --sync s
./lab2_list --threads 8  --iterations 8 	--yield dl --sync s
./lab2_list --threads 8  --iterations 16 	--yield dl --sync s
./lab2_list --threads 8  --iterations 32 	--yield dl --sync s
./lab2_list --threads 12 --iterations 1 	--yield dl --sync s
./lab2_list --threads 12 --iterations 2 	--yield dl --sync s
./lab2_list --threads 12 --iterations 4 	--yield dl --sync s
./lab2_list --threads 12 --iterations 8 	--yield dl --sync s
./lab2_list --threads 12 --iterations 16 	--yield dl --sync s
./lab2_list --threads 12 --iterations 32 	--yield dl --sync s

#sync with mutex and spin
#run with 1, 2, 4, 8, 12, 16, 24 threads
#with 1000 iterations

./lab2_list --threads 1  --iterations 1000 --sync m
./lab2_list --threads 2  --iterations 1000 --sync m
./lab2_list --threads 4  --iterations 1000 --sync m
./lab2_list --threads 8  --iterations 1000 --sync m
./lab2_list --threads 12 --iterations 1000 --sync m
./lab2_list --threads 16 --iterations 1000 --sync m
./lab2_list --threads 24 --iterations 1000 --sync m

./lab2_list --threads 1  --iterations 1000 --sync s
./lab2_list --threads 2  --iterations 1000 --sync s
./lab2_list --threads 4  --iterations 1000 --sync s
./lab2_list --threads 8  --iterations 1000 --sync s
./lab2_list --threads 12 --iterations 1000 --sync s
./lab2_list --threads 16 --iterations 1000 --sync s
./lab2_list --threads 24 --iterations 1000 --sync s

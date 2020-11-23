#! /usr/bin/gnuplot

# output:
#       lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations.
#       lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations.
#       lab2b_3.png ... successful iterations vs. threads for each synchronization method.
#       lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists.
#       lab2b_5.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists.

# csv format
# name, threads, iterations, number of lists, number of operations performed, total run time, average time per operation
# 1, 2, 3, 4, 5, 6, 7                       
# general plot parameters
set terminal png
set datafile separator ","

#PLOT 1
#To get the throughput, divide one billion by the time per operation.
set title "List-1: throughput vs. number of threads for mutex \n and spin-lock synchronized list operations"

set xlabel "number of threads"
set ylabel "throughput"

set xrange [1:25]

set logscale x 10
set logscale y 10

set output 'lab2b_1.png'
#grep single threaded mutex and spin
plot \
        "< grep '^list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'mutex lock' with linespoints lc rgb 'blue', \
        \
        "< grep '^list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'spin lock' with linespoints lc rgb 'green'

#PLOT 2
set title "List-2: mean time per mutex wait and mean time per \n operation for mutex-synchronized list operations"

set xlabel "threads"
set ylabel "time"

set xrange [1:25]

set logscale x 2
set logscale y 10

set output 'lab2b_2.png'

plot \
        "< grep '^list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
        title 'time for wait' with linespoints lc rgb 'blue', \
        \
        "< grep '^list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
        title 'time per operation' with linespoints lc rgb 'green'

#PLOT 3
set title "List-3: successful iterations vs. threads \n for each synchronization method"

set xlabel "threads"
set ylabel "iterations per thread"

set logscale x 2
set logscale y 10

set xrange [0.75:]

set output 'lab2b_3.png'


plot \
        "< grep '^list-id-none' lab2b_list.csv" using ($2):($3) \
        title 'no lock' with points lc rgb 'blue', \
        \
        "< grep '^list-id-m' lab2b_list.csv" using ($2):($3) \
        title 'mutex lock' with points lc rgb 'green', \
        \
        "< grep '^list-id-s' lab2b_list.csv" using ($2):($3) \
        title 'spin lock' with points lc rgb 'red'

#PLOT 4
set title "List-4: throughput vs. number of threads for \n mutex synchronized partitioned lists"

set xlabel "threads"
set ylabel "mutex lock throughput"

set logscale x 2
set logscale y 10

set xrange [0.75:]

set output 'lab2b_4.png'

plot \
        "< grep '^list-none-m,.*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 1' with linespoints lc rgb 'blue', \
        \
        "< grep '^list-none-m,.*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 4' with linespoints lc rgb 'green', \
        \
        "< grep '^list-none-m,.*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 8' with linespoints lc rgb 'red', \
        \
        "< grep '^list-none-m,.*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 16' with linespoints lc rgb 'black'

#PLOT 5
set title "List-5: throughput vs. number of threads for \n spin-lock-synchronized partitioned lists"

set xlabel "threads"
set ylabel "spin lock throughput"

set logscale x 2
set logscale y 10

set xrange [0.75:]

set output 'lab2b_5.png'

plot \
        "< grep '^list-none-s,.*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 1' with linespoints lc rgb 'blue', \
        \
        "< grep '^list-none-s,.*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 4' with linespoints lc rgb 'green', \
        \
        "< grep '^list-none-s,.*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 8' with linespoints lc rgb 'red', \
        \
        "< grep '^list-none-s,.*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7)) \
        title 'lists - 16' with linespoints lc rgb 'black'


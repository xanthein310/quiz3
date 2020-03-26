reset
set xlabel 'size'
set ylabel 'time(ns)'
set title 'time measurement'
set term png enhanced font 'Verdana,10'
set output 'measure.png'

plot 'time_opt.txt' using 1:2 with linespoints title 'opt merge sort', \

#plot 'time.txt' using 1:2 with linespoints title 'opt merge sort', \
#'time_merge.txt' using 1:2 with linespoints title 'merge sort', \
#'time_insert.txt' using 1:2 with linespoints title 'insert sort', \

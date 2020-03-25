reset
set xlabel 'size'
set ylabel 'time(ns)'
set title 'time measurement'
set term png enhanced font 'Verdana,10'
set output 'measure.png'

plot 'time.txt' using 1:2 with linespoints title 'merge sort', \

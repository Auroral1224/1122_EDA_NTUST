reset
set size ratio -1
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 0, 2 to 2, 5
set label "block_0" at 1, 3 center
set object 2 rect from 1, 0 to 3, 2
set label "block_1" at 2, 1 center
set object 3 rect from 2, 2 to 5, 3
set label "block_2" at 3, 2 center
set object 4 rect from 2, 3 to 5, 5
set label "block_3" at 3, 4 center
set object 5 rect from 0, 0 to 1, 2
set label "block_4" at 0, 1 center
set object 6 rect from 3, 0 to 5, 2
set label "block_5" at 4, 1 center
set xtics 1
set ytics 1
plot [0:5][0:5]0
set terminal png size 1024, 768
set output "output.png"
replot

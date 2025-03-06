reset
set size ratio -1
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 0, 1300 to 750, 1700
set label "block_0" at 375, 1500 center
set object 2 rect from 1000, 0 to 1625, 640
set label "block_1" at 1312, 320 center
set object 3 rect from 1000, 640 to 1625, 1280
set label "block_2" at 1312, 960 center
set object 4 rect from 0, 1700 to 800, 2325
set label "block_3" at 400, 2012 center
set object 5 rect from 1625, 1625 to 2425, 2250
set label "block_4" at 2025, 1937 center
set object 6 rect from 0, 700 to 1000, 1300
set label "block_5" at 500, 1000 center
set object 7 rect from 1625, 0 to 2425, 750
set label "block_6" at 2025, 375 center
set object 8 rect from 0, 0 to 1000, 700
set label "block_7" at 500, 350 center
set object 9 rect from 1625, 750 to 2425, 1625
set label "block_8" at 2025, 1187 center
set object 10 rect from 800, 1300 to 1600, 2300
set label "block_9" at 1200, 1800 center
set xtics 485
set ytics 465
plot [0:2425][0:2325]0
set terminal png size 1024, 768
set output "output.png"
replot

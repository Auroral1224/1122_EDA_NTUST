reset
set size ratio -1
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 1000, 0 to 1625, 480
set label "0" at 1312, 240 center
set object 2 rect from 1625, 0 to 2250, 640
set label "1" at 1937, 320 center
set object 3 rect from 0, 0 to 800, 500
set label "2" at 400, 250 center
set object 4 rect from 0, 1800 to 1000, 2300
set label "3" at 500, 2050 center
set object 5 rect from 1000, 1600 to 1625, 2400
set label "4" at 1312, 2000 center
set object 6 rect from 1625, 640 to 2425, 1390
set label "5" at 2025, 1015 center
set object 7 rect from 0, 1200 to 1000, 1800
set label "6" at 500, 1500 center
set object 8 rect from 0, 500 to 1000, 1200
set label "7" at 500, 850 center
set object 9 rect from 1000, 480 to 1625, 1600
set label "8" at 1312, 1040 center
set object 10 rect from 1625, 1390 to 2425, 2390
set label "9" at 2025, 1890 center
set xtics 485
set ytics 480
plot [0:2425][0:2400]0
set terminal png size 1024, 768
set output "output.png"
replot

reset
set size ratio -1
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 0, 3740 to 500, 4140
set label "0" at 250, 3940 center
set object 2 rect from 1000, 1500 to 1500, 2100
set label "1" at 1250, 1800 center
set object 3 rect from 500, 2740 to 1100, 3240
set label "2" at 800, 2990 center
set object 4 rect from 1000, 0 to 1500, 600
set label "3" at 1250, 300 center
set object 5 rect from 3625, 2500 to 4125, 3300
set label "4" at 3875, 2900 center
set object 6 rect from 3365, 1500 to 4165, 2000
set label "5" at 3765, 1750 center
set object 7 rect from 1000, 600 to 1500, 1400
set label "6" at 1250, 1000 center
set object 8 rect from 3125, 2500 to 3625, 3300
set label "7" at 3375, 2900 center
set object 9 rect from 2725, 0 to 3350, 640
set label "8" at 3037, 320 center
set object 10 rect from 1250, 2100 to 2050, 2725
set label "9" at 1650, 2412 center
set object 11 rect from 1500, 1120 to 2125, 1920
set label "10" at 1812, 1520 center
set object 12 rect from 3125, 2000 to 4125, 2500
set label "11" at 3625, 2250 center
set object 13 rect from 3365, 875 to 4165, 1500
set label "12" at 3765, 1187 center
set object 14 rect from 0, 2740 to 500, 3740
set label "13" at 250, 3240 center
set object 15 rect from 1100, 2740 to 2100, 3240
set label "14" at 1600, 2990 center
set object 16 rect from 2125, 1000 to 2725, 2000
set label "15" at 2425, 1500 center
set object 17 rect from 0, 1500 to 1000, 2100
set label "16" at 500, 1800 center
set object 18 rect from 2125, 2000 to 3125, 2600
set label "17" at 2625, 2300 center
set object 19 rect from 2125, 0 to 2725, 1000
set label "18" at 2425, 500 center
set object 20 rect from 0, 0 to 1000, 600
set label "19" at 500, 300 center
set object 21 rect from 2125, 2600 to 3125, 3200
set label "20" at 2625, 2900 center
set object 22 rect from 1300, 3240 to 2100, 4115
set label "21" at 1700, 3677 center
set object 23 rect from 1500, 0 to 2125, 1120
set label "22" at 1812, 560 center
set object 24 rect from 3125, 3300 to 4125, 4000
set label "23" at 3625, 3650 center
set object 25 rect from 3365, 0 to 4165, 875
set label "24" at 3765, 437 center
set object 26 rect from 500, 3240 to 1300, 4115
set label "25" at 900, 3677 center
set object 27 rect from 2125, 3200 to 3125, 4000
set label "26" at 2625, 3600 center
set object 28 rect from 2725, 640 to 3365, 1890
set label "27" at 3045, 1265 center
set object 29 rect from 0, 2100 to 1250, 2740
set label "28" at 625, 2420 center
set object 30 rect from 0, 600 to 1000, 1500
set label "29" at 500, 1050 center
set xtics 833
set ytics 828
plot [0:4165][0:4140]0
set terminal png size 1024, 768
set output "output.png"
replot

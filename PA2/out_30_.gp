reset
set size ratio -1
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 800, 2175 to 1200, 2675
set label "block_0" at 1000, 2425 center
set object 2 rect from 1775, 750 to 2175, 1500
set label "block_1" at 1975, 1125 center
set object 3 rect from 0, 2800 to 500, 3400
set label "block_2" at 250, 3100 center
set object 4 rect from 875, 750 to 1275, 1500
set label "block_3" at 1075, 1125 center
set object 5 rect from 2950, 3300 to 3450, 4100
set label "block_4" at 3200, 3700 center
set object 6 rect from 2200, 0 to 2700, 800
set label "block_5" at 2450, 400 center
set object 7 rect from 1275, 750 to 1775, 1550
set label "block_6" at 1525, 1150 center
set object 8 rect from 2200, 2000 to 3000, 2500
set label "block_7" at 2600, 2250 center
set object 9 rect from 3700, 0 to 4200, 800
set label "block_8" at 3950, 400 center
set object 10 rect from 0, 2175 to 800, 2800
set label "block_9" at 400, 2487 center
set object 11 rect from 0, 800 to 800, 1425
set label "block_10" at 400, 1112 center
set object 12 rect from 3075, 2000 to 4075, 2500
set label "block_11" at 3575, 2250 center
set object 13 rect from 2200, 1500 to 3200, 2000
set label "block_12" at 2700, 1750 center
set object 14 rect from 1200, 2175 to 2200, 2675
set label "block_13" at 1700, 2425 center
set object 15 rect from 800, 2675 to 1300, 3675
set label "block_14" at 1050, 3175 center
set object 16 rect from 3200, 800 to 4200, 1400
set label "block_15" at 3700, 1100 center
set object 17 rect from 0, 1425 to 800, 2175
set label "block_16" at 400, 1800 center
set object 18 rect from 2200, 3300 to 2950, 4100
set label "block_17" at 2575, 3700 center
set object 19 rect from 3200, 1400 to 4200, 2000
set label "block_18" at 3700, 1700 center
set object 20 rect from 875, 1550 to 1835, 2175
set label "block_19" at 1355, 1862 center
set object 21 rect from 3450, 3300 to 4200, 4100
set label "block_20" at 3825, 3700 center
set object 22 rect from 800, 3675 to 1920, 4300
set label "block_21" at 1360, 3987 center
set object 23 rect from 0, 0 to 875, 800
set label "block_22" at 437, 400 center
set object 24 rect from 2200, 2500 to 3075, 3300
set label "block_23" at 2637, 2900 center
set object 25 rect from 2200, 800 to 3200, 1500
set label "block_24" at 2700, 1150 center
set object 26 rect from 0, 3400 to 800, 4275
set label "block_25" at 400, 3837 center
set object 27 rect from 3075, 2500 to 4075, 3300
set label "block_26" at 3575, 2900 center
set object 28 rect from 2700, 0 to 3700, 800
set label "block_27" at 3200, 400 center
set object 29 rect from 1300, 2675 to 2100, 3675
set label "block_28" at 1700, 3175 center
set object 30 rect from 875, 0 to 2075, 750
set label "block_29" at 1475, 375 center
set xtics 840
set ytics 860
plot [0:4200][0:4300]0
set terminal png size 1024, 768
set output "output.png"
replot

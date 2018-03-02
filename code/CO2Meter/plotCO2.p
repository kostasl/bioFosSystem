
set terminal x11 1 noraise
#set xtics 3600
#set mxtics 4
#set xdata time

#set ytics 1 nomirror
#set mytics 2
set yrange [300:6000000]
set xrange [0:1200000]

#set y2range [0:3]
#set y2tics 1
#set my2tics 4
#set grid
#set ylabel "CO2, degC"
#set y2label "Temp, degC" textcolor rgb '#00CD63'

if (!exists("filename")) filename='Co2stream.csv'


#cd "/home/kostasl/Dropbox/Bioluminesce/CO2Meter"
plot filename u 1:3 title "CO2" with lines lc rgb "yellow";
# "test5.txt" using (1):5 title "deg C" with lines lc rgb "blue" axis x1y2;
pause 1
refresh
reread

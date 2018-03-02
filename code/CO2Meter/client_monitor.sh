#!/bin/sh

#Title: Record CO2 sensor from USB 
#Description: Start the CO2Sensor Recording and display figure.  Co2 MG811 sensor is read by an arduino nano, doing AD conversion and sending serial data every 100ms via the USB connection. Sensor has been setup to average 26 CO2 sensor reading samples, for each sample sent via USB.
#Author : Kostas Lagogiannis 9/2016

echo "Co2 Sensor recording and display."
echo "Writing to filename '$1'"
echo "Reading from /dev/ttyUSB0"
echo "~~~~~~~~~~~~~~~~~~~~~~~v0.1 2016~~~~~~~~~~~~~~~~~~~~"
((stty raw; cat > $1) < /dev/ttyUSB0) |  tail $1 -F &


gnuplot -rv -e "filename='$1'" plotCO2.p
killall tail

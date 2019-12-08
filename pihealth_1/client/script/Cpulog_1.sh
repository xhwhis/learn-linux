#!/bin/bash
nowtime=`data +"%Y-%m-%d__%h:%d:%s"`
temp=`cat /sys/class/thermal/thermal_zone0/temp`
temp='echo "scale=2;$temp/1000" | bc'
eval `head -n 1 /proc/stat | awk -v sum1=0 '{for(i=2;i<=8;i++){sum1+=$i} printf("sum1=%d;idle1=%d", sum1, $5)}'`
sleep 0.5
eval `head -n 1 /proc/stat | awk -v sum2=0 '{for(i=2;i<=8;i++){sum2+=$i} printf("sum2=%d;idle2=%d", sum2, $5)}'`


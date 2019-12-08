#!/bin/sh 
time=$(date "+%Y-%m-%d__%H:%M:%S")
time_wait=0.5
load1=$(cat /proc/loadavg | awk '{print $1}')
load2=$(cat /proc/loadavg | awk '{print $2}')
load3=$(cat /proc/loadavg | awk '{print $3}')
temp=$(cat /sys/class/thermal/thermal_zone0/temp)
temp=`echo ${temp} | awk '{printf "%d", $1/1000}'`
if [[ $temp -gt 70 ]]; then
    due="warning"
elif [[ $temp -gt 50 && $temp -lt 70 ]]; then
    due="note"
elif [[ $temp -lt 50 ]]; then
    due="normal"
fi
last_info=$(cat /proc/stat | grep -w cpu | awk '{print $2,$3,$4,$5,$6,$7,$8}')
last_idle=$(echo $last_info | awk '{print $4}')
last_total=$(echo $last_info | awk '{print $1+$2+$3+$4+$5+$6+$7}')
sleep ${time_wait}
next_info=$(cat /proc/stat | grep -w cpu | awk '{print $2,$3,$4,$5,$6,$7,$8}')
next_idle=$(echo $next_info | awk '{print $4}')
next_total=$(echo $next_info | awk '{print $1+$2+$3+$4+$5+$6+$7}')
sys_idle=`echo ${next_idle} ${last_idle} | awk '{print $1-$2}'`
total_time=`echo ${next_total} ${last_total} | awk '{print $1-$2}'`
usage=`echo ${sys_idle} ${total_time} | awk '{printf "%.2f", 100-$1*100/$2}'`
echo $time ${load1} ${load2} ${load3} ${usage} ${temp}℃  ${due}

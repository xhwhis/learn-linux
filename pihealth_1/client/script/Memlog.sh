#!/bin/bash
time_wait=0.5
time=$(date "+%Y-%m-%d__%H:%M:%S")
total=$(free -m | grep Mem | awk '{print $2}')
free=$(free -m | grep Mem | awk '{print $4}')
usage=`echo ${total} ${free} | awk '{printf "%.2f", 100-$2*100/$1}'`
echo $time ${total}M ${usage}% 

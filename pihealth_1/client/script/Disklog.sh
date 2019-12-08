#!/bin/bash
eval `ps -aux --sort -%cpu -h | awk -v num1=0 '{if($3<25) {exit} else {num1++; printf("procid1["num1"]=%d;", $2); printf("cpu1["num1"]=%d;", $3)}} END {printf("cpunum1=%d;",num1)}'`
sleep 5
eval `ps -aux --sort -%cpu -h | awk -v num2=0 '{if($3<25) {exit} else {num2++; printf("procid2["num2"]=%d;", $2)}} END {printf("cpunum2=%d;",num2)}'`
time=`date +"%Y-%m-%d__%H:%M:%S"`
echo ${time}
for (( i=1; i<=cpunum1 && i <= cpunum2; i++ )); do
    if [[ procid1[i] -eq procid2[i] ]]; then
        echo ${procid1[i]} ${cpu1[i]}
    fi
done

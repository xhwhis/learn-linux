#!/bin/bash
time=`date +"%Y-%m-%d__%H:%M:%S"`
name=`uname -n`
kernel1=`uname -o`
kernel2=`uname -r`
eval `ps -aux --sort -%cpu -h | awk -v num1=0 '{if($3==0) {exit} else {num1++;sum+=$3}}'`
echo ${time} ${name} ${kernel1} ${kernel2} ${sum}

#!/bin/bash
time=`date +"%Y-%m-%d__%H:%M:%S"`
cnt=`cat /etc/passwd | wc -l`
user_active=`last | grep -v "^$" | grep -v wtmp | cut -d " " -f 1 | sort | uniq | head -3 | tr "\n" ","`
user_root=` cat /etc/passwd | cut -d : -f 1,3 | grep 1000 | cut -d : -f 1 | tr "\n" ","`
user_online=`last | grep -v "^$" | grep -v wtmp | grep still | cut -d " " -f 1 | sort | uniq | tr "\n" ","`
echo ${time} ${cnt} [${user_active}] [${user_root}] [${user_online}]

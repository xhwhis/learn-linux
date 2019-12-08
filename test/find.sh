#!/bin/bash
function echo_name(){
    for file in `ls $1`; do
        echo $1
        if [ -d $1 ]; then
            echo_name $1
        fi
    done
}

echo_name $1

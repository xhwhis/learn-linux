#!/bin/bash
if [[ $# -eq 1  ]]; then
    g++ -D _DEBUG wechat.c ./common/common.c ./common/linklist.c -I ./common -lpthread -o wechat1
else
    g++ wechat.c ./common/common.c ./common/linklist.c -I ./common -lpthread -o wechat1
fi

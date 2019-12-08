#!/bin/bash
if [[ $# -eq 1  ]]; then
    gcc -D _DEBUG wechat.c ./common/common.c ./common/linklist.c -I ./common -lpthread -o wechat
else
    gcc wechat.c ./common/common.c ./common/linklist.c -I ./common -lpthread -o wechat
fi

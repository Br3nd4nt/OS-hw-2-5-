#!/bin/bash
echo "Input 'ping' to 'pong', 'exit' to exit"
# переменная для чтения ввода
read input
# цикл, который будет выполняться, пока ввод не равен "exit"
while [ "$input" != "exit" ]; do
    if [ "$input" == "ping" ]; then
        echo "pong"
    else
        echo "You entered: $input"
    fi
    read input
done

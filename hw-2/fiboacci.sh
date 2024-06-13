#!/bin/bash

# Проверка на правильное количество аргументов
if [ $# -ne 1 ]; then
    echo "Usage: $0 <number>"
    exit 1
fi

# Создаем нужные переменные
n=$1
a=0
b=1
tmp=0

# Используем цикл for от 1 до n
for i in $(seq 1 $n);
do
    tmp=$b
    b=$((a+b))
    a=$tmp
done
echo "Fibbonacci($n) = $a"

#!/bin/bash

# Функция для вычисления факториала числа
factorial() {
    # докальная переменная с первым аргументом функции
    local n=$1
    # проверка что n меньше или равно 1
    if [ $n -le 1 ]; then
        echo 1
    else
        # сохраняем значение (n - 1) в переменную
        local temp=$((n - 1))
        # рекурсивный вызов функции factorial
        local result=$(factorial $temp)
        # вывод результата
        echo $((n * result))
    fi
}

# Функция для вызова функции факториала и вывода результата
calculate_factorial() {
    local num=$1
    local fact=$(factorial $num)
    echo "Факториал числа $num равен $fact"
}

# Вызов функции calculate_factorial с аргументом из командной строки
calculate_factorial $1

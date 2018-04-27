#!/bin/bash

open() {
    cd Py

    if [ "$2" == "" ]; then
        python3 arduino.py
    else
        python3 arduino.py "$2"
    fi

    cd ..
    rm nohup.out
}

test() {
    cp driver.ino Test/driver.c

    cd Py
    python3 to_test.py

    cd ../Test
}

push() {
    git add .
    git commit -m "update"
    git push origin
}

reset() {
    git fetch origin
    git reset --hard origin/master
}

do1() {
    cp ../tower.ino driver.ino
}

do2() {
    cp ../stable1.ino driver.ino
}

do3() {
    cp ../stable2.ino driver.ino
}

left() {
    cp ../left.ino driver.ino
}

left2() {
    cp ../left2.ino driver.ino
}

right() {
    cp ../right.ino driver.ino
}

right2() {
    cp ../right2.ino driver.ino
}

if [ "$1" == "open" ]; then
    open

elif [ "$1" == "test" ]; then
    test

elif [ "$1" == "push" ]; then
    push

elif [ "$1" == "reset" ]; then
    reset

elif [ "$1" == "1" ]; then
    do1 

elif [ "$1" == "2" ]; then
    do2

elif [ "$1" == "3" ]; then
    do3

elif [ "$1" == "left" ]; then
    left

elif [ "$1" == "right" ]; then
    right

elif [ "$1" == "left2" ]; then
    left2

elif [ "$1" == "right2" ]; then
    right2

elif [ "$1" == "" ]; then
    echo "open 
test
push


1 (高台)
2 (无后退)
3 (有后退)

left (直冲)
right (直冲)

left2 (过弯)
right2 (过弯)"

fi

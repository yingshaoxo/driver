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

if [ "$1" == "open" ]; then
    open

elif [ "$1" == "test" ]; then
    test

elif [ "$1" == "push" ]; then
    push

elif [ "$1" == "" ]; then
    echo "open 
test
push"

fi

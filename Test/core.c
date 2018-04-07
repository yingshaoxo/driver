#include<stdio.h>

#define INPUT 0
#define OUTPUT 1
#define HIGH 1
#define LOW 0
#define false 0

#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

void pinMode(int pin, int value) {
    printf("%d", pin);
    printf("%d", value);
}

int analogRead(int pin) {
    printf("%d", pin);
    return 1;
}

void analogWrite(int pin, int value) {
    printf("%d", pin);
    printf("%d", value);
}

int digitalRead(int pin) {
    printf("%d", pin);
    return 1;
}

void digitalWrite(int pin, int value) {
    printf("%d", pin);
    printf("%d", value);
}

void delay(int time) {
    printf("%d", time);
}

void delayMicroseconds(int time) {
    printf("%d", time);
}

int pulseIn(int pin, int value) {
    printf("%d", pin);
    printf("%d", value);
    return 1;
}

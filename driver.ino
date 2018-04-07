//#include "core.c"

/*
#define AA0 A0
#define AA1 A1
#define AA2 A2
#define AA3 A3
#define AA4 A4
#define AA5 A5
*/

int A, B, C;

int little = 50;//50;//72;
int much = 150;//150;//200;
int heavy = 255;//150;//200;
int speed = 120;//120;

struct node 
{
    int key;
    int delay_time;
    int direction;
    int speed;
} nodes[8] = {
    {0, 2500, 0, 0},
    {1, 0, 1, 255},
    {2, 0, -1, 255},
    {3, 0, 0, 0},
    {4, 0, 0, 0},
    {5, 0, 0, 0},
    {6, 0, 0, 0},
    {7, 0, 0, 0},
};
int num = -1;

void set_left_wheels(int go_or_back, int value) {
    if (go_or_back == 1) {
        analogWrite(9, value);
        analogWrite(10, 0);
    } else if (go_or_back == -1) {
        analogWrite(10, value);
        analogWrite(9, 0);
    } else if (go_or_back == 0) {
        analogWrite(9, 0);
        analogWrite(10, 0);
    }
}

void set_right_wheels(int go_or_back, int value) {
    if (go_or_back == 1) {
        analogWrite(5, value);
        analogWrite(6, 0);
    } else if (go_or_back == -1) {
        analogWrite(6, value);
        analogWrite(5, 0);
    } else if (go_or_back == 0) {
        analogWrite(5, 0);
        analogWrite(6, 0);
    }
}

void go_straight(int value) {
    set_left_wheels(1, value);
    set_right_wheels(1, value);
}

void go_back(int value) {
    set_left_wheels(-1, value);
    set_right_wheels(-1, value);
}

void stop() {
    set_left_wheels(0, 0);
    set_right_wheels(0, 0);
}

void turn_left(int value) {
    float difference = value / 2;
    float middle_point = 255 / 2;
    int right = middle_point + difference;
    int left = middle_point - difference;

    set_left_wheels(1, left);
    set_right_wheels(1, right);
}

void turn_right(int value) {
    float difference = value / 2;
    float middle_point = 255 / 2;
    int left = middle_point + difference;
    int right = middle_point - difference;

    set_left_wheels(1, left);
    set_right_wheels(1, right);
}

int ultrasonic_wave(int trigPin, int echoPin) {
    long duration;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    duration = duration / 59;
    if ((duration < 2) || (duration > 300)) return false;
    return duration;
}

void white_detect() {
    A = digitalRead(A0); //black detector
    B = digitalRead(A2); //white detector
    C = digitalRead(A4); //black detector
}

int make_sure(int whatsA, int whatsB, int whatsC, int timeout) {
    int interval = timeout / 10;

    int possibility = 0;
    int time_spent = 0;

    int hit = 0;
    int all = 0;
    while ((possibility <= 50) && (time_spent < timeout)) {
        if ((whatsA == A) && (whatsB == B) && (whatsC == C)) {
            hit = hit + 1;

        } else {
        }
        all = all + 1;
        possibility = hit / all * 100;

        delay(interval);
        time_spent = time_spent + interval;
    }
    
    if (time_spent >= timeout) {
        return 0;
    } else {
        return 1;
    }
}

void arrive_non_black_action() {
    go_straight(speed/100 * 50); 
    while ((A == 1) && (B == 1) && (C == 1)) {
        //delay(1000 * 0.5);
        white_detect();
    }
}

void arrive_black_action() {
    num = num + 1;

    go_straight(speed);
    delay(nodes[num].delay_time);

    stop();
    delay(3000);

    if (nodes[num].direction == -1) {
        turn_left(heavy);
        delay(2000);
    } else if (nodes[num].direction == 0) {

    } else if (nodes[num].direction == 1) {
        turn_right(heavy);
        delay(2000);
    }

    if (nodes[num].speed != 0 ) {
        go_straight(nodes[num].speed);
    }
}

void find_line() {
    white_detect();

    if ((A == 1) && (B == 1) && (C == 1)) {
        //arrive_non_black_action();

    } else if ((A == 1) && (B == 1) && (C == 0)) {
        turn_right(much);
        
    } else if ((A == 1) && (B == 0) && (C == 1)) {
        go_straight(speed);
        
    } else if ((A == 1) && (B == 0) && (C == 0)) {
        turn_right(little);
        
    } else if ((A == 0) && (B == 1) && (C == 1)) {
        turn_left(much);

    } else if ((A == 1) && (B == 0) && (C == 1)) {
        go_straight(speed);
        
    } else if ((A == 0) && (B == 0) && (C == 1)) {
        turn_left(little);
        
    } else if ((A == 0) && (B == 0) && (C == 0)) {
        if (make_sure(0, 0, 0, 200) == 1) {
            //arrive_black_action();
        }
    }
}

/*
int main() {
}
*/

void setup() {
    Serial.begin(9600);
    go_straight(120);
    delay(1000);
}

void loop() {
    find_line();

    /*
    Serial.print("A4: ");
    Serial.print(digitalRead(A4));//can't change
    Serial.print(" ");//can't change
    Serial.print("A3: ");
    Serial.print(digitalRead(A3));//can't change
    delay(200);
    */

    /*
    Serial.print("wave: ");
    printfln(ultrasonic_wave(2, 17));//can't change
    printfln(" ");
    delay(200);
    */
}

//#include "core.c"

int A, B, C, D;
int grey_gate = 900; //450
int white_gate = 50;

int light = 70;
int normal = 255;
int heavy = 255;

float left_bias = 0;
float right_bias = 0.02734375;

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
    value = value - (value * left_bias);
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
    value = value - (value * right_bias);
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

/*
void turn_back_left(int value) {
    float difference = value / 2;
    float middle_point = 255 / 2;
    int right = middle_point + difference;
    int left = middle_point - difference;

    set_left_wheels(-1, left);
    set_right_wheels(-1, right);
}

void turn_back_right(int value) {
    float difference = value / 2;
    float middle_point = 255 / 2;
    int left = middle_point + difference;
    int right = middle_point - difference;

    set_left_wheels(-1, left);
    set_right_wheels(-1, right);
}
*/

void left_rotate(int value) {
    set_right_wheels(1, value);
    set_left_wheels(-1, value);
}

void right_rotate(int value) {
    set_left_wheels(1, value);
    set_right_wheels(-1, value);
}

int handle_white_detector(int pin) {
    Serial.print(analogRead(pin));
    Serial.print("   ");
    if (analogRead(pin) < white_gate) {
        return 1;
    } else {
        return 0;
    }
}

int handle_gray_detector(int pin) {
    Serial.print(analogRead(pin));
    Serial.print("   ");
    if (analogRead(pin) > grey_gate) {
        return 1;
    } else {
        return 0;
    }
}

void sensor_detect(int white_or_black, int AAA1, int AAA2, int AAA3, int AAA4) {
    if (white_or_black == 1) {
        A = handle_white_detector(AAA1);
        B = handle_white_detector(AAA2);
        C = handle_white_detector(AAA3);
        D = handle_white_detector(AAA4);
    } else if (white_or_black == -1) {
        A = handle_gray_detector(AAA1);
        B = handle_gray_detector(AAA2);
        C = handle_gray_detector(AAA3);
        D = handle_gray_detector(AAA4);
    } else {
        A = digitalRead(AAA1);
        B = digitalRead(AAA2);
        C = digitalRead(AAA3);
        C = digitalRead(AAA4);
    }
    //return 1 if it is white

    Serial.print("\nABC and D: \n");
    Serial.print(A);
    Serial.print(B);
    Serial.print(C);
    Serial.print(" and ");
    Serial.print(D);
    Serial.println();
    Serial.println();
    Serial.println();
}

void white_detect() {
    //ssensor_detect(0, A0, A2, A4, A3);
    sensor_detect(0, A1, A2, A3, A4);
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

void return_to_black_line() {
    while (1) {
        go_back(light);
        delay(1000 * 0.01);

        white_detect();
        if ((make_sure(1, 1, 1, 200) != 0)) {
            break;
        }
    }
}

int if_its_90_degree_corner(int timeout) {
    int time_spent = 0;
    float interval = 0.01;

    stop();
    delay(1000 * interval);

    while (1) {
        go_straight(normal);
        delay(1000 * interval);

        white_detect();
        if ((A == 1) && (B == 1) && (C == 1)) {
            return 1;
        }

        time_spent = time_spent + (1000 * interval);
        if (time_spent > timeout) {
            break;
        }
    }
    
    return 0;
}

void turn_left_90_degrees_intelligently() {
    return_to_black_line();

    while (1) {
        left_rotate(normal);
        delay(1000 * 0.05);

        white_detect();
        if (make_sure(1, 0, 1, 200) == 1) {
            break;
        }
        if (C == 0) {
            break;
        }
    }

    go_straight(normal);
    delay(1000 * 0.1);
}

void turn_right_90_degrees_intelligently() {
    return_to_black_line();

    while (1) {
        right_rotate(normal);
        delay(1000 * 0.05);

        white_detect();
        if (make_sure(1, 0, 1, 200) == 1) {
            break;
        }
        if (C == 0) {
            break;
        }
    }

    go_straight(normal);
    delay(1000 * 0.1);
}

void turn_left_90_degrees_stupidly() {
    left_rotate(255);
    delay(1000 * 0.3);
}

void turn_right_90_degrees_stupidly() {
    right_rotate(255);
    delay(1000 * 0.3);
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

void arrive_non_black_action() {
    go_straight(normal/100 * 50); 
    while ((A == 1) && (B == 1) && (C == 1)) {
        //delay(1000 * 0.5);
        white_detect();
    }
}

void arrive_black_action() {
    num = num + 1;

    go_straight(normal);
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
        //turn_right(heavy);
        right_rotate(normal);
        
    } else if ((A == 1) && (B == 0) && (C == 1)) {
        go_straight(normal);
        
    } else if ((A == 1) && (B == 0) && (C == 0)) {
        //turn_right(light);
        if (if_its_90_degree_corner(200) == 1) {
            turn_right_90_degrees_intelligently();
        }
        
    } else if ((A == 0) && (B == 1) && (C == 1)) {
        //turn_left(heavy);
        left_rotate(normal);

    } else if ((A == 0) && (B == 0) && (C == 1)) {
        //turn_left(light);
        if (if_its_90_degree_corner(200) == 1) {
            turn_left_90_degrees_intelligently();
        }        

    } else if ((A == 0) && (B == 0) && (C == 0)) {
        //arrive_black_action();
    }
}

void setup() {
    Serial.begin(9600);
    /*
    go_straight(120);
    delay(1000);
    */
}

void loop() {
    find_line();
    //white_detect();

    //delay(500);
    //Serial.print("Hello, world!");
}

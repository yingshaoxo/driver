//#include "core.c"

int A, B, C, D;

int white_gate = 50;

int gray_gate_A = 170;
int gray_gate_B = 270;
int gray_gate_C = 250;
int gray_gate_D = 500;

/*
int gray_gate_A = 400;
int gray_gate_B = 650;
int gray_gate_C = 600;
int gray_gate_D = 500;
*/

int light = 180;
int normal = 200;
int heavy = 255;

float left_bias = 0;//.02634375;
float right_bias = 0;//0.02734375;

int last_action;
int after_node;

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

void left_rotate(int value) {
    set_right_wheels(1, value);
    set_left_wheels(-1, value);
}

void right_rotate(int value) {
    set_left_wheels(1, value);
    set_right_wheels(-1, value);
}

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

int handle_white_detector(int pin) {
    Serial.print(analogRead(pin));
    Serial.print("   ");
    if (analogRead(pin) < white_gate) {
        return 1;
    } else {
        return 0;
    }
}

int handle_gray_detector(int pin, int gate_number) {
    Serial.print(analogRead(pin));
    Serial.print("   ");
    if (gate_number == 1) {
        if (analogRead(pin) > gray_gate_A) {
            return 1;
        } else {
            return 0;
        }
    } else if (gate_number == 2) {
        if (analogRead(pin) > gray_gate_B) {
            return 1;
        } else {
            return 0;
        }
    } else if (gate_number == 3) {
        if (analogRead(pin) > gray_gate_C) {
            return 1;
        } else {
            return 0;
        }
    } else if (gate_number == 4) {
        if (analogRead(pin) > gray_gate_D) {
            return 1;
        } else {
            return 0;
        }
    }
}

void sensor_detect(int white_or_black, int AAA1, int AAA2, int AAA3, int AAA4) {
    if (white_or_black == 1) {
        A = handle_white_detector(AAA1);
        B = handle_white_detector(AAA2);
        C = handle_white_detector(AAA3);
        D = handle_white_detector(AAA4);
    } else if (white_or_black == -1) {
        A = handle_gray_detector(AAA1, 1);
        B = handle_gray_detector(AAA2, 2);
        C = handle_gray_detector(AAA3, 3);
        D = handle_gray_detector(AAA4, 4);
    } else {
        A = digitalRead(AAA1);
        B = digitalRead(AAA2);
        C = digitalRead(AAA3);
        D = digitalRead(AAA4);
    }
    //return 1 if it is white

    Serial.print("\nABC and D: \n");
    Serial.print(A);
    Serial.print(B);
    Serial.print(C);
    Serial.print("  ");
    Serial.print(D);
    Serial.println();
    Serial.println();
    Serial.println();
}

void white_detect() {
    sensor_detect(-1, A0, A2, A4, 8);
}

void go_through_node() {
    while (1) {
        go_straight(heavy);

        white_detect();
        if ((A == 1) || (C == 1)) {
            break;
        }
    }
}

void turn_left_45_degrees_stupidly() {
    left_rotate(heavy);
    delay(600);
}

void turn_right_45_degrees_stupidly() {
    right_rotate(heavy);
    delay(600);
}

void find_line() {
    white_detect();

    if ((A == 1) && (B == 1) && (C == 1)) {
        if (after_node == 1) {
            if (last_action == 0) {
                go_back(normal);
            } else if (last_action == -1) {
                turn_back_right(normal);
            } else if (last_action == 1) {
                turn_back_left(normal);
            }
        } else {
            go_back(normal);
        }

        after_node = 0;

    } else if ((A == 0) && (B == 0) && (C == 0)) {
        go_through_node();

        after_node = 1;

    } else if ((A == 1) && (C == 0)) {
        right_rotate(heavy);

        last_action = 1;

        after_node = 0;
        
    } else if ((A == 1) && (C == 1)) {
        go_straight(heavy);

        last_action = 0;

        after_node = 0;
        
    } else if ((A == 0) && (C == 1)) {
        left_rotate(heavy);

        last_action = -1;

        after_node = 0;

    } else {
        after_node = 0;

    }
}

void setup() {
    Serial.begin(9600);

    /*
    go_straight(heavy);
    delay(5500);
    stop();
    delay(1000);

    go_straight(heavy);
    */
}

void loop() {
    find_line();

    //white_detect();
    //delay(500);
}

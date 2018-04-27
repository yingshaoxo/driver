//#include "core.c"

int A, B, C, D;

int white_gate = 50;

int gray_gate_A = 400;
int gray_gate_B = 650;
int gray_gate_C = 600;
int gray_gate_D = 500;

int all_times = 0;
int back_times = 0;

int light = 180;
int normal = 200;
int heavy = 255;

float left_bias = 0;//.02634375;
float right_bias = 0;//0.02734375;

int last_action;

struct node 
{
    int key;
    int direction;
    int delay_seconds;
} nodes[8] = {
    {0, 1, 3},
    {1, 1, 3},
    {2, 0, 0},
    {3, 0, 0},
    {4, 0, 0},
    {5, 0, 0},
    {6, 0, 0},
    {7, 0, 0},
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

void turn_left_45_degrees_stupidly() {
    left_rotate(heavy);
    delay(600);
}

void turn_right_45_degrees_stupidly() {
    right_rotate(heavy);
    delay(600);
}

/*
void turn_left_45_degrees_stupidly() {
    left_rotate(heavy);
    delay(300);
}

void turn_right_45_degrees_stupidly() {
    right_rotate(heavy);
    delay(300);
}
*/

void go_through_node() {
    while (1) {
        go_straight(normal);
        //delay(1000 * 0.05);

        white_detect();
        if ((A == 1) && (C == 1)) {
            break;
        }
    }
}

void turn_left_90_degrees_intelligently() {
    if (D == 1) {
        turn_left_45_degrees_stupidly();
    }

    while (1) {
        left_rotate(normal);
        //delay(1000 * 0.05);

        white_detect();
        if (B == 1) {
            break;
        }
    }

    while (1) {
        left_rotate(normal);
        //delay(1000 * 0.05);

        white_detect();
        if (B == 0) {
            break;
        }
    }
}

void turn_right_90_degrees_intelligently() {
    if (D == 1) {
        turn_right_45_degrees_stupidly();
    }

    while (1) {
        right_rotate(normal);
        //delay(1000 * 0.05);

        white_detect();
        if (B == 1) {
            break;
        }
    }

    while (1) {
        right_rotate(normal);
        //delay(1000 * 0.05);

        white_detect();
        if (B == 0) {
            break;
        }
    }
}

/*
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
*/

void node_arriving_action() {
    num = num + 1;

    go_through_node();

    stop();
    delay(3000);

    if (nodes[num].direction == -1) {
        turn_left_90_degrees_intelligently();

    } else if (nodes[num].direction == 0) {
        go_straight(normal);
        //delay(800);

    } else if (nodes[num].direction == 1) {
        turn_right_90_degrees_intelligently();

    }

    if (num == 1) {
        num = 0;
    }

    go_back(heavy);
    delay(200);
}

void find_line() {
    white_detect();

    if ((A == 1) && (B == 1) && (C == 1)) {
        if (back_times < 3) {
            if (last_action == 0) {
                go_back(heavy);
            } else if (last_action == -1) {
                turn_back_right(heavy);
            } else if (last_action == 1) {
                turn_back_left(heavy);
            }
        }

        back_times += 1;
        all_times += 1;

    } else if ((A == 0) && (B == 0) && (C == 0)) {
        //go_straight(heavy);
        node_arriving_action();

    } else if ((A == 1) && (C == 0)) {
        right_rotate(heavy);

        last_action = 1;
        
    } else if ((A == 1) && (C == 1)) {
        go_straight(heavy);

        all_times += 1;

        last_action = 0;
        
    } else if ((A == 0) && (C == 1)) {
        left_rotate(heavy);

        last_action = -1;
    }

    if (all_times > 27) {
        all_times = 0;
    }
}

void setup() {
    Serial.begin(9600);
    /*
    go_straight(heavy);
    delay(1800);
    */
    //go_straight(heavy);
    
}

void loop() {
    find_line();

    //white_detect();
    //delay(500);
}

//#include "core.c"

int A, B, C;
int white_gate = 50;

int gate_A = 400;
int gate_B = 400;
int gate_C = 400;

int light = 180;
int normal = 200;
int heavy = 255;

int rotate_delay = 250; //250

float left_bias = 0;
float right_bias = 0;//0.02734375;

struct node 
{
    int key;
    int delay_time;
    int direction;
    int speed;
} nodes[8] = {
    {0, 100, -1, 255},
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

int handle_gray_detector(int pin, int gate_number) {
    Serial.print(analogRead(pin));
    Serial.print("   ");
    if (gate_number == 1) {
        if (analogRead(pin) > gate_A) {
            return 1;
        } else {
            return 0;
        }
    } else if (gate_number == 2) {
        if (analogRead(pin) > gate_B) {
            return 1;
        } else {
            return 0;
        }
    } else if (gate_number == 3) {
        if (analogRead(pin) > gate_C) {
            return 1;
        } else {
            return 0;
        }
    }

}

void sensor_detect(int white_or_black, int AAA1, int AAA2, int AAA3) {
    if (white_or_black == 1) {
        A = handle_white_detector(AAA1);
        B = handle_white_detector(AAA2);
        C = handle_white_detector(AAA3);
    } else if (white_or_black == -1) {
        A = handle_gray_detector(AAA1, 1);
        B = handle_gray_detector(AAA2, 2);
        C = handle_gray_detector(AAA3, 3);
    } else {
        A = digitalRead(AAA1);
        B = digitalRead(AAA2);
        C = digitalRead(AAA3);
    }
    //return 1 if it is white

    Serial.print("\nABC: \n");
    Serial.print(A);
    Serial.print(B);
    Serial.print(C);
    Serial.println();
    Serial.println();
    Serial.println();
}

void white_detect() {
    sensor_detect(0, A0, A2, A4);
    //sensor_detect(-1, A1, A2, A3);
}

void turn_left_90_degrees_intelligently() {
    stop();
    delay(200);
    go_straight(normal);
    delay(1000);

    while (1) {
        left_rotate(normal);
        delay(1000 * 0.05);

        white_detect();
        if (B == 0) {
            break;
        }
    }
}

void turn_right_90_degrees_intelligently() {
    stop();
    delay(200);
    go_straight(normal);
    delay(1000);

    while (1) {
        right_rotate(normal);
        delay(1000 * 0.05);

        white_detect();
        if (B == 0) {
            break;
        }
    }
}

void turn_left_90_degrees_stupidly() {
    left_rotate(heavy);
    delay(1000 * 0.3);
}

void turn_right_90_degrees_stupidly() {
    right_rotate(heavy);
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

void node_arriving_action() {
    num = num + 1;

    stop();
    delay(3000);

    if (nodes[num].direction == -1) {
        turn_left_90_degrees_intelligently();

    } else if (nodes[num].direction == 0) {
        go_straight(normal);
        delay(800);

    } else if (nodes[num].direction == 1) {
        turn_right_90_degrees_intelligently();

    }
}

void find_line() {
    white_detect();

    if ((A == 1) && (C == 0)) {
        stop();
        delay(200);
        right_rotate(heavy);
        delay(rotate_delay);
        
    } else if ((A == 1) && (C == 1)) {
        go_straight(normal);
        
    } else if ((A == 0) && (C == 1)) {
        stop();
        delay(200);
        left_rotate(heavy);
        delay(rotate_delay);

    } else if ((A == 0) && (C == 0)) {
        stop();

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
    //find_line();
    //white_detect();

    Serial.println(ultrasonic_wave(2, A3));
    delay(100);
}

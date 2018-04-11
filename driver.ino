//#include "core.c"

int A, B, C, D;
int grey_gate = 500;

int light = 70;
int normal = 200;
int heavy = 255;

float left_bias = 0;
float right_bias = 0.02734375;

float special_action_interval = 0.5 * 1000;

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

void left_rotate(int value) {
    set_right_wheels(1, value);
    set_left_wheels(-1, value);
}

void right_rotate(int value) {
    set_left_wheels(1, value);
    set_right_wheels(-1, value);
}

int handle_white_detector(int pin) {
    if (analogRead(pin) < 100) {
        return 1;
    } else {
        return 0;
    }
}

int handle_gray_detector(int pin) {
    //Serial.println(analogRead(pin));
    if (analogRead(pin) > grey_gate) {
        return 1;
    } else {
        return 0;
    }
}

void analog_white_detect() {
    A = handle_gray_detector(A1); //return 1 if it is white
    B = handle_gray_detector(A2);
    C = handle_gray_detector(A3);
    D = handle_gray_detector(A4);

    Serial.print("ABC and D: \n");
    Serial.print(A);
    Serial.print(B);
    Serial.print(C);
    Serial.print(" and ");
    Serial.print(D);
    Serial.println();
    Serial.println();
}

void white_detect() {
    analog_white_detect();

    /*
    A = digitalRead(A1); //return 1 if it is white
    B = digitalRead(A2);
    C = digitalRead(A3);
    D = digitalRead(A4);
    */

    /*
    Serial.print("ABC and D: \n");
    Serial.print(A);
    Serial.print(B);
    Serial.print(C);
    Serial.print(" and ");
    Serial.print(D);
    Serial.println();
    Serial.println();
    */
}

int make_sure(int whatsA, int whatsB, int whatsC, int timeout) {
    if (timeout == 0) {
        if ((whatsA == A) && (whatsB == B) && (whatsC == C)) {
            return 1;
        } else {
            return 0;
        }
    }

    int interval = timeout / (timeout/4);

    int possibility = 0;
    int time_spent = 0;

    int hit = 0;
    int all = 0;
    while ((possibility <= 50) && (time_spent < timeout)) {
        white_detect();
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

void prepare_for_anything() {
    int times = 1;
    while (1) {
        left_rotate(light);
        delay(special_action_interval * times);

        white_detect();
        if ((make_sure(1, 0, 1, 0) == 0) && (D == 0)) {
            break;
        }

        right_rotate(light);
        delay(special_action_interval * times);

        white_detect();
        if ((make_sure(1, 0, 1, 0) == 0) && (D == 0)) {
            break;
        }

        times = times + 1;
    }
}

void back_to_black_line() {
    while (1) {
        go_back(light);

        white_detect();
        if ((make_sure(1, 1, 1, 0) == 0)) {
            break;
        }
    }
}

int if_its_90_degree_corner(int timeout) {
    int time_spent = 0;

    stop();
    delay(1000);

    while (1) {
        go_straight(speed);
        delay(special_action_interval);
        stop();

        white_detect();
        if (make_sure(1, 1, 1, 0) == 1) {
            return 1;
        }

        time_spent = time_spent + special_action_interval;
        if (time_spent > timeout) {
            break;
        }
    }

    back_to_black_line();
    return 0;
}

/*
void turn_left_90_degrees_intelligently() { 
    while (1) { 
        left_rotate(heavy); 
        delay(special_action_interval); 
        stop(); 
 
        white_detect(); 
        if (make_sure(1, 0, 1, 0) == 1) { 
            break; 
        } 
    } 
 
    go_straight(normal); 
    delay(500); 
} 
 
void turn_right_90_degrees_intelligently() { 
    while (1) { 
        right_rotate(heavy);
        delay(special_action_interval);
        stop();

        white_detect();
        if (make_sure(1, 0, 1, 0) == 1) {
            break;
        }
    }

    go_straight(normal);
    delay(500);
}
*/

void turn_left_90_degrees_intelligently() {
    while (1) {
        left_rotate(light);

        white_detect();
        if (make_sure(1, 0, 1, 0) == 1) {
            break;
        }
    }
    /*
    go_straight(speed);
    delay(1000 * 0.1);
    */
}

void turn_right_90_degrees_intelligently() {
    while (1) {
        right_rotate(light);

        white_detect();
        if (make_sure(1, 0, 1, 0) == 1) {
            break;
        }
    }
    /*
    go_straight(speed);
    delay(1000 * 0.1);
    */
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

/*
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
*/

void find_line() {
    white_detect();

    if ((A == 1) && (B == 1) && (C == 1)) {
        //arrive_non_black_action();

    } else if ((A == 1) && (B == 1) && (C == 0)) {
        right_rotate(normal);
        
    } else if ((A == 1) && (B == 0) && (C == 1)) {
        go_straight(normal);
        
    } else if ((A == 1) && (B == 0) && (C == 0)) {
        if (if_its_90_degree_corner(200) == 1) {
            turn_right_90_degrees_intelligently();
        }        

    } else if ((A == 0) && (B == 1) && (C == 1)) {
        left_rotate(normal);

    } else if ((A == 0) && (B == 0) && (C == 1)) {
        if (if_its_90_degree_corner(200) == 1) {
            turn_left_90_degrees_intelligently();
        }        

    } else if ((A == 0) && (B == 0) && (C == 0)) {
        //arrive_black_action();
        go_straight(normal);
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
    go_straight(heavy);
    //find_line();

    delay(500);
    //Serial.print("Hello, world!");
}

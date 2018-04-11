//#include "core.c"

int A, B, C, D;

int light = 70;
int normal = 200;
int heavy = 255;

float left_bias = 0;
float right_bias = 0.02734375;

float special_action_interval = 0.1 * 1000;

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

/*
// handle pins by hands
void set_left_wheels(int go_or_back, int value) {
    value = value - (value * left_bias);
    if (go_or_back == 1) {
        analogWrite(6, value);
        analogWrite(7, 0);
    } else if (go_or_back == -1) {
        analogWrite(7, value);
        analogWrite(6, 0);
    } else if (go_or_back == 0) {
        analogWrite(6, 0);
        analogWrite(7, 0);
    }
}

void set_right_wheels(int go_or_back, int value) {
    value = value - (value * right_bias);
    if (go_or_back == 1) {
        analogWrite(4, value);
        analogWrite(5, 0);
    } else if (go_or_back == -1) {
        analogWrite(5, value);
        analogWrite(4, 0);
    } else if (go_or_back == 0) {
        analogWrite(4, 0);
        analogWrite(5, 0);
    }
}
*/

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

void white_detect() {
    A = digitalRead(A0); //return 1 if it is white
    B = digitalRead(A2); 
    C = digitalRead(A4); 
    D = digitalRead(A3); 

    /*
    // handle pins by hands
    A = digitalRead(A1); //return 1 if it is white
    B = digitalRead(A2); 
    C = digitalRead(A3); 
    D = digitalRead(A4); 
    */
}

int make_sure(int whatsA, int whatsB, int whatsC, int whatsD, int timeout) {
    if (timeout == 0) {
        if ((whatsA == A) && (whatsB == B) && (whatsC == C) && (whatsD == D)) {
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
        if ((whatsA == A) && (whatsB == B) && (whatsC == C) && (whatsD == D)) {
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

void back_to_black_line() {
    while (1) {
        go_back(light);
        delay(special_action_interval);
        stop();

        white_detect();
        if ((make_sure(1, 1, 1, 1, 0) == 0)) {
            break;
        }
    }
}

/*
// uncertain function
void back_to_black_line() {
    while (1) {
        go_back(light);

        white_detect();
        if ((make_sure(1, 1, 1, 1, 200) == 0)) {
            break;
        }
    }
}
*/

int if_its_90_degree_corner(int timeout) {
    int time_spent = 0;

    stop(); //moveable
    delay(1000);

    while (1) {
        go_straight(normal);
        delay(special_action_interval);
        stop();

        white_detect();
        if (make_sure(1, 1, 1, 1, 0) == 1) {
            back_to_black_line();
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
// uncertain function
int if_its_90_degree_corner(int timeout) {
    int time_spent = 0;

    stop();
    delay(special_action_interval);

    while (1) {
        go_straight(speed);

        white_detect();
        if (make_sure(1, 1, 1, 1, 200) == 1) {
            back_to_black_line();
            return 1;
        }

        time_spent = time_spent + (1000 * interval);
        if (time_spent > timeout) {
            break;
        }
    }

    back_to_black_line();
    return 0;
}
*/

void turn_left_90_degrees_intelligently() {
    while (1) {
        left_rotate(heavy);
        delay(special_action_interval);
        stop();

        white_detect();
        if (make_sure(1, 1, 1, 1, 0) == 1) {
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
        if (make_sure(1, 1, 1, 1, 0) == 1) {
            break;
        }
    }

    go_straight(normal);
    delay(500);
}

void find_line() {
    white_detect();

    if ((A == 1) && (B == 1) && (C == 1) && (D == 1)) {
        go_straight(normal);

    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 1)) {
        right_rotate(light);

    } else if ((A == 1) && (B == 0) && (C == 1) && (D == 1)) {
        left_rotate(light);

    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 0)) {
        right_rotate(heavy);
        /*
        if (if_its_90_degree_corner(200) == 1) {
            turn_right_90_degrees_intelligently();
        }
        */

    } else if ((A == 0) && (B == 0) && (C == 1) && (D == 1)) {
        left_rotate(heavy);
        /*
        if (if_its_90_degree_corner(200) == 1) {
            turn_left_90_degrees_intelligently();
        }
        */

    } else if ((A == 0) && (B == 0) && (C == 0) && (D == 0)) {
        stop();

    }
}

void setup() {
    Serial.begin(9600);

    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(A2, INPUT);
    pinMode(A4, INPUT);
    pinMode(A3, INPUT);

    /*
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    */
}

void loop() {
    Serial.println(analogRead(A0));
    delay(500);
    //find_line();
}


#!/usr/bin/env python3

with open("../driver.ino", "r") as f:
    text = f.read()

text = text.replace('#include "core.c"', '//#include "core.c"')
text = text.replace('//Serial.begin(9600)', 'Serial.begin(9600)')
text = text.replace('printf(', 'Serial.print(')

with open("../driver.ino", "w") as f:
    f.write(text)



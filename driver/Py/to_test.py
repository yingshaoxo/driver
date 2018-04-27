#!/usr/bin/env python3

with open("../Test/driver.c", "r") as f:
    text = f.read()

text = text.replace('//#include "core.c"', '#include "core.c"')
text = text.replace('Serial.begin(9600);', '//Serial.begin(9600);')
text = text.replace('Serial.print', 'printf')

with open("../Test/driver.c", "w") as f:
    f.write(text)



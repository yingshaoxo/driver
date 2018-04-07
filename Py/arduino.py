#!/usr/bin/env python3
from auto_everything.base import Terminal
from os.path import abspath

t = Terminal()
args = t.get_args()

if args == None:
    t.run_program("sudo /opt/arduino/arduino /home/yingshaoxo/Codes/Python/driver/driver/driver.ino")
else:
    t.run_program("sudo /opt/arduino/arduino {path}".format(path = abspath(args[0])))

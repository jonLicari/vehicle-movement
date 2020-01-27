#!/usr/bin/env python
import time
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)

# ----- Variable Declarations ------ #

# MOTOR A (LEFT)
# Port Assignments
ena_port = 3    # ENA Port
fwda_port = 5   # IN1 Port (forward)
bwda_port = 7   # IN2 Port (backward)

# Values
ena = 0.5       # Duty cycle for Motor A
fwda = True     # Motor A forward pin
bwda = False    # Motor A backward pin

# MOTOR B (RIGHT)
# Port Assignments
enb_port = 14   # ENB Port
fwdb_port = 15  # IN3 Port (forward)
bwdb_port = 18  # IN4 Port (forward)

# Values
enb = 0.5       # Duty cycle for Motor B
fwdb = True     # Motor B forward pin
bwdb = False    # Motor B backward pin

# Setting GPIO pins
GPIO.setup(ena_port, GPIO.OUT)
GPIO.setup(fwda_port, GPIO.OUT)
GPIO.setup(bwda_port, GPIO.OUT)
GPIO.setup(enb_port, GPIO.OUT)
GPIO.setup(fwdb_port, GPIO.OUT)
GPIO.setup(bwdb_port, GPIO.OUT)

#output can be toggled using GPIO.output(var_name, GPIO.HIGH/LOW)

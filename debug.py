#!/usr/bin/env python

import time
import control

ser = control.setup()
ser.write('90,90,90,90,0\n')

# Main loop
while True:
    next = raw_input('Q/W/A/S for in, E/R/D/F for out ')
    next = next.upper()[0]

    if next == 'W': speeds = '80,90,90,90,0\n'
    if next == 'Q': speeds = '90,100,90,90,0\n'
    if next == 'S': speeds = '90,90,100,90,0\n'
    if next == 'A': speeds = '90,90,90,80,0\n'
    if next == 'R': speeds = '100,90,90,90,0\n'
    if next == 'E': speeds = '90,80,90,90,0\n'
    if next == 'F': speeds = '90,90,80,90,0\n'
    if next == 'D': speeds = '90,90,90,100,0\n'

    ser.write(str(speeds))
    time.sleep(0.5)
    ser.write('90,90,90,90,0\n')

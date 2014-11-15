#!/usr/bin/env python

import time
import control

ser = control.setup()

# Main loop
while True:
    next = input('Q/W/A/S for in, E/R/D/F for out ')
    if next == 'Q': speeds = str(0) + '90,90,90\n'
    if next == 'W': speeds = '90,' + str(180) + ',90,90\n'
    if next == 'A': speeds = '90,90,' + str(0) + '90\n'
    if next == 'S': speeds = '90,90,90,' + str(180) + '\n'
    if next == 'E': speeds = str(180) + '90,90,90\n'
    if next == 'R': speeds = '90,' + str(0) + ',90,90\n'
    if next == 'D': speeds = '90,90,' + str(180) + '90\n'
    if next == 'F': speeds = '90,90,90,' +str(0) + '\n'
    ser.write(str(speeds))
    time.sleep(0.5)
    ser.write('90,90,90,90\n')

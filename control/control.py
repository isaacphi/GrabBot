# Get your Bot on 2014

import serial
import math

PORT = 9600
device = "/dev/ttyACM0"
device2 = "/dev/ttyAMC1"

# Physical dimensions in mm
L = 500
W = 400
R = 20
MAX_RPM = 50
MAX_SPEED = MAX_RPM * 2 * math.pi * R / 60

x, y, z, theta = 0, 0, 0, 0

def main():
    # Connect to arduino
    try:
        ser = serial.Serial(device, PORT)
        print 'Connected to ', device
    except serial.SerialException:
        ser = serial.Serial(device2, PORT)
        print 'Connected to ', device2
        time.sleep(1)

    # Main loop
    while True:
        speeds = input('speeds: ')
        ser.write(str(speeds))

def move(p1, p2, phi, del_t):
    theta = 90 - phi
    if theta > 90: theta = 90
    if theta < 0: theta = 0
    x1, y1, z1 = p1 # last points
    x2, y2, z2 = p2 # new points
    # Calculate last string lengths
    l_SW1 = math.sqrt( x1**2 + y1**2 + z1**2 )
    l_SE1 = math.sqrt( (L-x1)**2 + y1**2 + z1**2 )
    l_NW1 = math.sqrt( x1**2 + (W-y1)**2 + z1**2 )
    l_NE1 = math.sqrt( (L-x1)**2 + (W-y1)**2 + z1**2 )
    # Calculate new string lengths
    l_SW2 = math.sqrt( x2**2 + y2**2 + z2**2 )
    l_SE2 = math.sqrt( (L-x2)**2 + y2**2 + z2**2 )
    l_NW2 = math.sqrt( x2**2 + (W-y2)**2 + z2**2 )
    l_NE2 = math.sqrt( (L-x2)**2 + (W-y2)**2 + z2**2 )
    # Calculate differences
    del_SW = l_SW2 - l_SW1
    del_SE = l_SE2 - l_SE1
    del_NW = l_NW2 - l_NW1
    del_NE = l_NE2 - l_NE1
    # And Calculate motor velocites
    V_SW = del_SW / del_t
    V_SE = del_SE / del_t
    V_NW = del_NW / del_t
    V_NE = del_NE / del_t
    # Convert these values into numbers 0-180
    SW_speed = (V_SW * (90 / MAX_SPEED)) + 90
    if SW_speed > 180: SW_speed = 180
    if SW_speed < 0: SW_speed = 0
    SE_speed = (V_SE * (90 / MAX_SPEED)) + 90
    if SE_speed > 180: SE_speed = 180
    if SE_speed < 0: SE_speed = 0
    SW_speed = (V_SW * (90 / MAX_SPEED)) + 90
    if NW_speed > 180: NW_speed = 180
    if NW_speed < 0: NW_speed = 0
    SW_speed = (V_SW * (90 / MAX_SPEED)) + 90
    if NE_speed > 180: NE_speed = 180
    if NE_speed < 0: NE_speed = 0
    # TODO make angle right
    # Update the motor speeds & Angle
    ser.write(NE_speed, NW_speed, SE_speed, SW_speed, theta)

if __name__ == '__main__':
    main()

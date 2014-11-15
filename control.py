# Get your Bot on 2014

import serial
import math

PORT = 9600

# Physical dimensions in mm
L = 500
W = 400
H = 320
R = 20
MAX_RPM = 50
MAX_SPEED = MAX_RPM * 2 * math.pi * R / 60
MIN_X = -500
MAX_X = 500
MIN_Y = -500
MAY_Y = 500
MIN_Z = 50
MAX_Z = 500

x, y, z, theta = 0, 0, 0, 0


def main():
    setup()
    while True:
        speeds = input('speeds: ')
        ser.write(str(speeds))


def setup():
    """ Connect to arduino """

    for path in ("/dev/ttyACM0", "/dev/ttyACM1", "/dev/tty.usbmodem1411", "/dev/cu.usbmodem1411"):
        try:
            ser = serial.Serial(path, PORT)
            print "connected to", path
            break
        except serial.SerialException as e:
            pass
    else:
        raise e


def move(p1, p2, phi, del_t):
    theta = 90 - phi
    if theta > 90: theta = 90
    if theta < 0: theta = 0
    x1, y1, z1 = p1 # last points
    x2, y2, z2 = p2 # new points
    # Convert from Neil's coordinate system to the physical one
    x1 = L * (x1 + MIN_X) / (MAX_X-MIN_X)
    x2 = L * (x2 + MIN_X) / (MAX_X-MIN_X)
    y1 = W * (y1 + MIN_Y) / (MAX_Y-MIN_Y)
    y2 = W * (y2 + MIN_Y) / (MAX_Y-MIN_Y)
    z1 = -H * (z1 + MIN_Z) / (MAX_X-MIN_Z)
    z2 = -H * (z2 + MIN_Z) / (MAX_X-MIN_Z)
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
    ser.write(NE_speed + NW_speed + SE_speed + SW_speed + theta + '\n')


if __name__ == '__main__':
    main()
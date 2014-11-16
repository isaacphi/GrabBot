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
SPEED_FACTOR = 1.0

MIN_X = -200
MAX_X = 200
MIN_Y = -200
MAX_Y = 200
MIN_Z = 50
MAX_Z = 250
XOFFSET = 50
YOFFSET = 63

x, y, z, theta = 0, 0, 0, 0
ser = None

def main():
    global ser
    ser = setup()
    while True:
        speeds = input('speeds: ')
        ser.write(str(speeds))


def setup():
    """ Connect to arduino """
    global ser

    for path in ("/dev/ttyACM0", "/dev/ttyACM1", "/dev/tty.usbmodem1411", "/dev/cu.usbmodem1411"):
        try:
            ser = serial.Serial(path, PORT)
            print "connected to", path
            return ser
        except serial.SerialException as e:
            pass
        except OSError as e:
            pass
    else:
        raise e


def move(p1, p2, phi, del_t):
    # transform angle
    theta = 90 - min(max(phi, 0), 90)

    x1, y1, z1 = p1 # last points
    x2, y2, z2 = p2 # new points

    # clamp inputs
    x1 = min(max(x1, MIN_X), MAX_X)
    x2 = min(max(x2, MIN_X), MAX_X)
    y1 = min(max(y1, MIN_Y), MAX_Y)
    y2 = min(max(y2, MIN_Y), MAX_Y)
    z1 = min(max(z1, MIN_Z), MAX_Z)
    z2 = min(max(z2, MIN_Z), MAX_Z)

    # Convert from leap motion's coordinate system to the physical one
    x1 = L * (x1 - MIN_X) / (MAX_X-MIN_X)
    x2 = L * (x2 - MIN_X) / (MAX_X-MIN_X)
    y1 = W * (y1 - MIN_Y) / (MAX_Y-MIN_Y)
    y2 = W * (y2 - MIN_Y) / (MAX_Y-MIN_Y)
    z1 = H - H * (z1 - MIN_Z) / (MAX_Z-MIN_Z)
    z2 = H - H * (z2 - MIN_Z) / (MAX_Z-MIN_Z)

    print x2-x1, y2-y1, z2-z1

    # Calculate last string lengths
    l_SW1 = math.sqrt( (x1-XOFFSET)**2 + (y1-YOFFSET)**2 + z1**2 )
    l_SE1 = math.sqrt( (L-x1-XOFFSET)**2 + (y1-YOFFSET)**2 + z1**2 )
    l_NW1 = math.sqrt( (x1-XOFFSET)**2 + (W-y1-YOFFSET)**2 + z1**2 )
    l_NE1 = math.sqrt( (L-x1-XOFFSET)**2 + (W-y1-YOFFSET)**2 + z1**2 )

    # Calculate new string lengths
    l_SW2 = math.sqrt( (x2-XOFFSET)**2 + (y2-YOFFSET)**2 + z2**2 )
    l_SE2 = math.sqrt( (L-x2-XOFFSET)**2 + (y2-YOFFSET)**2 + z2**2 )
    l_NW2 = math.sqrt( (x2-XOFFSET)**2 + (W-y2-YOFFSET)**2 + z2**2 )
    l_NE2 = math.sqrt( (L-x2-XOFFSET)**2 + (W-y2-YOFFSET)**2 + z2**2 )

    # Calculate differences
    del_SW = l_SW2 - l_SW1
    del_SE = l_SE2 - l_SE1
    del_NW = l_NW2 - l_NW1
    del_NE = l_NE2 - l_NE1

    print del_NE, del_NW, del_SE, del_SW

    # And Calculate motor velocites
    V_SW = SPEED_FACTOR * del_SW / del_t
    V_SE = SPEED_FACTOR * del_SE / del_t
    V_NW = SPEED_FACTOR * del_NW / del_t
    V_NE = SPEED_FACTOR * del_NE / del_t

    # Convert these values into numbers 0-180

    NE_speed = (V_NE * (90 / MAX_SPEED)) + 90
    NE_speed = max(min(NE_speed, 180), 0)
    if (NE_speed > 80 and NE_speed < 100):
        NE_speed = 90

    NW_speed = -(V_NW * (90 / MAX_SPEED)) + 90
    NW_speed = max(min(NW_speed, 180), 0)
    if (NW_speed > 80 and NW_speed < 100):
        NW_speed = 90

    SE_speed = -(V_SE * (90 / MAX_SPEED)) + 90
    SE_speed = max(min(SE_speed, 180), 0)
    if (SE_speed > 80 and SE_speed < 100):
        SE_speed = 90

    SW_speed = (V_SW * (90 / MAX_SPEED)) + 90
    SW_speed = max(min(SW_speed, 180), 0)
    if (SW_speed > 80 and SW_speed < 100):
        SW_speed = 90

    # Update the motor speeds & Angle
    cmd = ",".join(["{:03d}".format(int(x)) for x in [NE_speed, NW_speed, SE_speed, SW_speed, theta]]) + '\n'
    print cmd
    ser.write(cmd)


if __name__ == '__main__':
    main()

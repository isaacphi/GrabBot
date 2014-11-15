# Get your Bot on 2014

import serial

PORT = 9600
device = "/dev/ttyACM0"
device2 = "/dev/ttyAMC1"

def main():
    # Connect to arduino
    try:
        ser = serial.Serial(device, PORT)
        print 'Connected to ', device
    except serial.SerialException:
        ser = serial.Serial(device2, PORT)
        print 'Connected to ', device2
        time.sleep(1)

    while True:
        speed = input('go: ')
        ser.write(str(speed))

if __name__ == '__main__':
    main()

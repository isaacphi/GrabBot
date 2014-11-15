import time
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
    next = input('Q/W/A/S for in, E/R/D/F for out ')
    if next == 'Q': speeds = str(0) + '0,0,0\n'
    if next == 'W': speeds = '0,' + str(180) + ',0,0\n'
    if next == 'A': speeds = '0,0,' + str(0) + '0\n'
    if next == 'S': speeds = '0,0,0,' + str(180) + '\n'
    if next == 'E': speeds = str(180) + '0,0,0\n'
    if next == 'R': speeds = '0,' + str(0) + ',0,0\n'
    if next == 'D': speeds = '0,0,' + str(180) + '0\n'
    if next == 'F': speeds = '0,0,0,' +str(0) + '\n'
    
    time.sleep(0.5)
    ser.write(str(speeds))

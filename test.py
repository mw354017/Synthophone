import serial

s = serial.Serial('COM4')
while 1 < 2:
    res = s.read();
    print(res);

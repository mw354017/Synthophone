import serial

s = serial.Serial('/dev/cu.webusbserial1420')
while true:
    s.write("text");

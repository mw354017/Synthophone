import serial

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3'
ser.open()

ser.write("note B");
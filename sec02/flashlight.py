import serial as ser

qtpy = ser.Serial('/dev/tty.usbmodem142101', 115200)
print(qtpy.name)
flag = True
while(flag):
    for x in range(5):
        line = qtpy.readline()
        print(line)
    i = input()
    if i == 'exit':
        break
    qtpy.write(bytearray(i,'ascii'))
    
qtpy.close()


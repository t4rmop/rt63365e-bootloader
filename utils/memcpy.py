import serial

from uart_interface import *
usbuart = serial.Serial('/dev/ttyUSB0', 115200)
uartinterface = UartInterface(usbuart,False)
p = ProxyUart(uartinterface,False)

f = open("text_comp.xz","rb")
a = []
a = f.read()

b = 0x80013b70
b=p.writemem(b,a)
print(hex(b))


#limit = b + len(a)
#while(b<limit): 
#    print(hex(p.read32(b)))
#    b+=4

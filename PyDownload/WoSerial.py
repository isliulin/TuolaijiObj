#!/usr/bin/env python
# it's a program of luo, piedgogo@sina.com

import serial
import array
import os
import signal
import os
import struct
from time import sleep
import six

flag_stop = False


def onsignal_int(a, b):
    print( "sigint!")
    global flag_stop
    flag_stop = True


signal.signal(signal.SIGINT, onsignal_int)
signal.signal(signal.SIGTERM, onsignal_int)

ser = serial.Serial(port='COM3', baudrate=38400, parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS, timeout=0.001)
print("serial.isOpen() =", ser.isOpen())


def waitAckOk():
    while True:
        if ser.inWaiting() > 0 :
            text = ser.read(1)
            if text == b'\x43':
                break;

def sendStart():
    cmd=b'\x56'
    ser.write(cmd)

def SumCrc(bstr):
    a = 0;
    for btem in bstr:
        a = a ^ btem
        #print(btem, a)
    return struct.pack('B', a)

def sendPacketHead(addr, filepath):
    filesize = os.path.getsize(filepath)
    print(filesize)
    #print (six.int2byte())
    #print(hex(filesize))
    bfilesize=struct.pack('>L', filesize)
    baddr=struct.pack('>L', (int(addr,16)))
    # start | type  |  lenght   | downaddr    |  filesize    | crc | end |
    #  AA   |   00 |   00  00  | 00 00 00 00 |  00 00 00 00 | 00  |  BB |
    #
    cmd = b'\xAA'          #start
    cmd += b'\x01'     #type
    cmd += b'\x00\x08'     #lenght
    tradata = baddr + bfilesize
    cmd += tradata
    cmd += SumCrc(tradata)
    cmd += b'\xBB'         #end
    print(cmd.hex())
    ser.write(cmd)

def sendPacketBody(databody):
    cmd = b'\xAA\x02' + struct.pack('>H', len(databody)) + databody + SumCrc(databody) + b'\xBB'
    print(cmd.hex())
    ser.write(cmd)

filename = 'LOGO.BIN'


sendStart()  #send   '0x56'
print("send start signal")
waitAckOk()  #wait   '0x43'
print("ack ok")
sendPacketHead('0xFF0000', filename)
print("send packet head")
waitAckOk()  #wait   '0x43'
print("ack ok")


f = open(filename, 'rb')
ffilesize = 0;
while True:
    rdata = f.read(1024)
    ffilesize += len(rdata)
    if rdata:
        sendPacketBody(rdata)
        waitAckOk()
    else:
        print("read fisnis!")
        break
print(ffilesize)
f.close()




print("mcu has ready")

while True:
    if ser.inWaiting() > 0:
        text = ser.read(1)
        print(" ",text)


ser.close()


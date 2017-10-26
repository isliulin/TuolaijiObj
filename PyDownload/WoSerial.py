#!/usr/bin/env python
# it's a program of luo, piedgogo@sina.com

import serial
import array
import os
import signal
import sys
import struct
from time import sleep
import six
import  xml.dom.minidom

flag_stop = False


def onsignal_int(a, b):
    print( "sigint!")
    global flag_stop
    flag_stop = True


signal.signal(signal.SIGINT, onsignal_int)
signal.signal(signal.SIGTERM, onsignal_int)

ser = serial.Serial(port='COM6', baudrate=38400, parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS, timeout=0.001)
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
    if (os.path.exists(filepath)):
        filesize = os.path.getsize(filepath)
        #print (six.int2byte())
        #print(hex(filesize))
        bfilepath=struct.pack('>L', os.path.getsize(filepath))
        baddr=struct.pack('>L', (int(addr,16)))
        # start | type     |  lenght   | downaddr    |  filesize    | crc | end |
        #  AA   |  00   00 |   00  00  | 00 00 00 00 |  00 00 00 00 | 00  |  BB |
        #
        cmd = b'\xAA'          #start
        cmd += b'\x01'     #type
        cmd += b'\x00\x08'     #lenght
        tradata = baddr + bfilepath
        cmd += tradata
        cmd += SumCrc(tradata)
        cmd += b'\xBB'         #end
        ser.write(cmd)
        print("Has Send Head Meg:",cmd.hex())
        waitAckOk()
        print("Ok!")
    else:
        print(sys._getframe().f_lineno,"No find this file:", filepath)

def writeSerialData(databody):
    cmd = b'\xAA\x02' + struct.pack('>H', len(databody)) + databody + SumCrc(databody) + b'\xBB'
    ser.write(cmd)

	
	
def sendPacketBody(filepath):
    print(filepath,"   " , os.path.getsize(filepath))
    if (os.path.exists(filepath)):
        filesize = os.path.getsize(filepath)
        f = open(filepath, 'rb')
        CurrentIndex=0
        hasDownfilesize = 0
        while True:
            rdata = f.read(512)
            hasDownfilesize += len(rdata)
            if not rdata:
                print("read compelet!" )
                break
            else:
                CurrentIndex += 1
                writeSerialData(rdata)
                print("%04d down:%d size:%d :"  %(CurrentIndex, hasDownfilesize, filesize), rdata.hex())
                waitAckOk()


        print(hasDownfilesize)
        f.close()
    else:
        print(sys._getframe().f_lineno, "No find this file:", filepath)


	

def dumpXmlFile(file_name):
    if(os.path.exists(file_name)):
        dom = xml.dom.minidom.parse(file_name)
        root =dom.documentElement
        childs = root.childNodes
        for child in childs:
            if child.nodeName == "#text":
                continue
            print("............start..........")
            sendStart()
            print("Send Start Signal:", end=' ')
            waitAckOk()
            print("Ok!")
            print(child.getAttribute('Address') ,child.getAttribute('FileName'), os.path.getsize(child.getAttribute('FileName') ))
            sendPacketHead(child.getAttribute('Address') , child.getAttribute('FileName'))
            sendPacketBody(child.getAttribute('FileName'))
            waitAckOk()
            print("............. end.........")

if __name__ == "__main__":
    dumpXmlFile("image.xml")
    exit(0)

ser.close()


#!/usr/bin/python
# coding: utf-8

HEAD_BYTE   = b'\x7e'[0]
ESCAPE_BYTE = b'\x7D7'[0]
ESCAPE_MASK = b'\x207'[0]

def decode(msg):
    for j in range(len(msg)):
        if  msg[j] == HEAD_BYTE:
            for i in range(len(msg[j+1:])):
                if msg[i] == ESCAPE_BYTE:
                    pass
                else:
                    pass
                    #print(i)
            data = msg[j+1:j+8]
    print('F: ', (msg[3] << 8) + msg[4])
    print('R: ', (msg[5] << 8) + msg[6])
    print('L: ', (msg[7] << 8) + msg[8])
    print('B: ', (msg[9] << 8) + msg[10])

    #return data

    #print(data)
    fRange = msg[1]
    fRange = msg[2]
    #rRange = msg[3]
    #rRange = msg[4]
    #lRange = msg[5]
    #lRange = msg[6]
    #bRange = msg[7]
    #bRange = msg[8]

if __name__ == '__main__':
    msg = b'\x7e\x50\xd4\x02'
    decode(msg)

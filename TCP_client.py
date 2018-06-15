#!/usr/bin/python
# coding: utf-8

import socket
import time
import decode_sensor_data as decode

host = '192.168.11.5'
port = 2391
buff = 1024
data = 'hello'

def make_connection(host_, port_):
  while True:
    try:
      sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
      sock.connect((host_, port_))
      print('connected')
      return sock
    except socket.error as e:
      print('failed to connect, try reconnect')
      time.sleep(0.1)

if __name__ == '__main__':
  s = make_connection(host, port)
  while True:
    try:
      s.send(data.encode(encoding='utf-8'))
      msg = s.recv(buff)
      if msg[0] == 126:
          #print(msg[1])
          #print(msg[2])
          #print(msg[3])
          #print(msg[4])
          decode.decode(msg)

      #print('Client received: %s' % int.from_bytes(msg, byteorder='big'))
      #print('Client received: %s' % msg)
      time.sleep(1)
    except socket.error as e:
      print('connection lost, try reconnect')
      s = make_connection(host, port)
    except socket.timeout:
      print('connection timeout, try reconnect')
      s = make_connection(host, port)

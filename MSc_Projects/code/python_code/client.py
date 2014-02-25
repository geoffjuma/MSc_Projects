#!/usr/bin/env python

import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost',8081))
s.send("Your ass is open")
data = s.recv(1024)
s.close
print "recieved", data


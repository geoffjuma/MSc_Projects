#!/usr/bin/env python

import socket
import sys 
#create a tcpip socket
server = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
#binding
server_address = ('localhost',8081)
print 'connecting on ..%s port %s',  server_address
server.bind(server_address)

#listen for connections
server.listen(60)
#wait for an incoming connection
connection, client_address = server.accept()
print 'connection from', connection.getpeername()

#recieve something
data = connection.recv(4096)
if data:
	print 'Recieved',repr(data)
	#send back
	data= data.rstrip()
	connection.send('%s\n%s\n%s\n %'('-'*80,data.center(80), '-'*80))
	print 'response sent!..'

#close connection
connection.shutdown(socket.SHUT_RD | socket.SHUT_WR)
connection.close()
print 'connection closed'

#stop listening

server.close()

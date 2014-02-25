#!/usr/bin/python

import sys

class new(object):
	def __init__(self, name, idnum, dob):
		self.name = name
		self.idnum = idnum
		self.dob = dob
	def display(self):
		print "hello sir %s" % self.name
		print "You were born in %s" % self.dob
		print "your id number is %s" %self.idnum
myObject = new('Juma','456723','12/12/2032')
myObject.display()


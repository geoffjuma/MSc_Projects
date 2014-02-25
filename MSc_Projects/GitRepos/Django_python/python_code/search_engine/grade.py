#!/usr/bin/env python

import sys
import math
import time 
import re

class Student(object):
	def __init__(self,name,idnum,marks):
		self.name = name
		self.idnum = idnum
		self.marks = marks

	def grade (self):
		if (self.marks > 70):
			mygrade = 'A'
			print "Excellent! You have an %s \n" %mygrade
		elif ((self.marks >=60) and (self.marks <=70)):
			mygrade = 'B'
			print ("Good! You have a %s \n" %mygrade)
		elif ((self.marks >=50) and (self.marks < 60)):
			mygrade = 'C'
			print ("Fair! You have a %s \n" %mygrade)
		else:
			mygrade = 'D'
			print ("Deil dog dalau! You are a fool with %s \n" %mygrade)
		return
	def enter(self):
		self.name = raw_input("Please enter the student's name: \n") 
		self.idnum = raw_input ("Please enter the student's id number: \n")
		self.marks = raw_input ("Please enter the student's marks : \n")
	
def main():
	student = Student("","","") 
	student.enter()
	student.grade()

if __name__ =="__main__":
	main()



:#!/usr/bin/env python

class Bank(object):
	def __init__(self,name,dob,idnum,balance):
		self.name = name
		self.dob = dob
		self.idnum =idnum
		self.balance = 1000

	def Withdraw(self,amount):
		self.amount = amount
		if (self.amount > self.balance):
			print "You dont have enough money"
		else:
			self.balance = (self.balance - amount)
			print "You have withdrawn %d KES " %amount
		return amount 

	def Deposit (self,deposit):
		self.deposit = deposit
		self.balance = deposit+self.balance
		print "You have deposited %d KES" %deposit
		return deposit
	def Display (self):
		print "Hello Mr. %s" %self.name + " We thank you for banking with us "
		print "Your new balance is %d KES" %self.balance

account1 = Bank('Juma','12/12/12/','3456789',1000)
account1.Withdraw(5000)
account1.Deposit(200)
account1.Display()

		


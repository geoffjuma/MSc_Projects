#!/usr/bin/python
class myClass:
 def addName(self,name):
  self.name = name
  print "my name is %s" %self.name
 def addAge(self, age):
  self.age = age
  return self.age
  print "my age is %s" %self.age
 def addSex(self,sex):
  self.sex = sex
  return self.sex
  print "my sex is %s" %self.sex
 #def myDisplay(name,age,sex):
 # self.name=name 
 # print "my name: %s age: %d sex %c" %self.name %self.age %self.sex

Jeff = myClass()
Jeff.addName("Jeff")
Jeff.addAge("34")
Jeff.addSex("male")
#Jeff.myDisplay(Jeff.addName(), Jeff.addAge(), Jeff.addSex())

Ashley = myClass()
Emma = myClass()





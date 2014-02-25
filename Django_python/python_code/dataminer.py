#! usr/bin/env python

import time
from datetime import datetime 
from time import mktime

bigDataFile = open('output.txt','r')
readFile = bigdataFile.read()

linesplit = readFile.split('\n')

for everyLine in linesplit:
	divideLine =everyLine.split(',')
	

#!/usr/bin/env python

import urllib
from xml.etree.ElementTree import parse

latitude = 41.98062
candidates =['4128','1774']

def distance(lat1, lat2):
 return 69* abs(lat1-lat2)

def monitor():
 u = urllib.urlopen ("http://ctabustracker.com/bustime/map/getBusesForRoute.jsp?route=22")
 doc = parse(u)
 for bus in doc.findall('bus'):
  busid = bus.findtext('id')
  #if busid in candidates :
  direction = bus.findtext('d')
  lat = float(bus.findtext('lat'))
  dis = distance(lat,latitude)
  print busid, direction, dis ,'miles'
print '*'*20


import time
while True:
 monitor()
 time.sleep(60)
 

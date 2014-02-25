#!/usr/bin/env python

latitude = 0
longitude = -87.668452

from xml.etree.ElementTree import parse
doc = parse('rt22.xml')

for bus in doc.findall('bus'):
   lat = float(bus.findtext('lat'))
 #if lat >latitude:
   direction = bus.findtext('d')
   #if direction.startswith('North'):
   busid = bus.findtext('id')
   print busid,lat,direction

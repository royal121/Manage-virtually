from geopy.geocoders import GoogleV3
import sys

location = sys.argv
location.remove("geocode.py")
location = ' '.join(location)

geolocator = GoogleV3()
location = geolocator.geocode(location,exactly_one=False,timeout=100)
f = open('geocode.txt','w')
for i in location:
	f.write(i.address.encode('utf-8')+'\n')
f.write('\n')
f.close()



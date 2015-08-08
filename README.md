# FoxmosaWatchFace-Pebble

Foxmosa is mascot of Mozilla Taiwan Community.  
Mozilla produces many products such as the Firefox, Thunderbird, Firefox OS.  
Picture designed by ECBp + Tatit + Indigo + Irvin (cc:by-nc-sa)  
MozTW, Mozilla Taiwan  

## Download
Apps is available on Pebble  
![Pebble download](http://pblweb.com/badge/55bed9ed39209aec5f000083/colour/size)  
[Download](https://apps.getpebble.com/en_US/application/55bed9ed39209aec5f000083)

## Build the code  

1. Clone the repository to your local machine.  

	```
git clone git@github.com:j796160836/FoxmosaWatchFace-Pebble.git
	```
	
2. Generate a new UUID for your copy of this project, and copy it into `appinfo.json`.

	```
uuidgen
	```
	
3. Install Pebble SDK at [Here](http://developer.pebble.com/sdk/) and do the pebble build command  

	```
pebble build
	```

4. Open the developer mode at your phone and enter the IP of your phone (eg. 192.168.1.2)

	```
pebble install --phone 192.168.1.2
	```

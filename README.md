 # Arduino sketches:


## Setup on PC
For temperature and humidity sensor open Arduino IDE and install the DHTLib as a .zip file https://github.com/RobTillaart/DHTlib

Open command prompt to see results

###  Setup on Raspberry Pi for serial communication 

if the .py programm does not work, run the following commands:
```console
$ sudo adduser your_username dialout
```
```console
$ python3 -m pip install pyserial

```

for checking the connection use:

```console
$ ls /dev/tty*
```


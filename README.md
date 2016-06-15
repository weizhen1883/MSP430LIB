# MSP430LIB

# Instill gcc compiler in Raspberry Pi
sudo apt-get install binutils-msp430 gcc-msp430 msp430-libc msp430mcu mspdebug

# Edit the Makefile in Raspberry Pi
CC_PATH=/usr
SUPPORT_PATH=/usr/msp430
OBJCOPY=${GCC_PATH}/bin/objcopy


# API
gpio
* pinNumber: a string of pin numbers such as "P1.1"
* status: pin output status ("ON", "OFF", and "TOGGLE")
* pinMode:


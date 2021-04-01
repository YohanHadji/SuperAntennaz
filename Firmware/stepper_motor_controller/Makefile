# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

ARDUINO_DIR = /home/azisi/opt/arduino-1.8.5
ARDMK_DIR = /home/azisi/opt/Arduino-Makefile
AVR_TOOLS_DIR = /home/azisi/opt/arduino-1.8.5/hardware/tools/avr

ARDUINO_QUIET = 0

ARCHITECTURE = avr
BOARD_TAG = satnogs
MONITOR_PORT = /dev/ttyUSB0

CFLAGS_STD = -std=gnu++11
USER_LIB_PATH = /home/azisi/workspace/arduino/libraries

AVRDUDE = /home/azisi/opt/arduino-1.8.5/hardware/tools/avr/bin/avrdude
AVRDUDE_OPTS = -v

BOOTLOADER_FILE = optiboot/optiboot_atmega328.hex

ISP_PROG = buspirate
ISP_PORT = /dev/ttyUSB0

#AVRDUDE_ARD_BAUDRATE = 57600

include /home/azisi/opt/Arduino-Makefile/Arduino.mk

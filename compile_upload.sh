#!/bin/bash
arduino-cli compile --fqbn arduino:avr:nano pc_info_lcd.ino
arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano:cpu=atmega328old pc_info_lcd.ino


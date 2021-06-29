#!/bin/bash
arduino-cli compile --fqbn arduino:avr:micro pc_info_lcd.ino

arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:micro pc_info_lcd.ino

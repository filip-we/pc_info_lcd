#PC-Info-LCD

This project lets you display status signals from a PC to an external LCD character screen.

Firmware for Arduino Nano
The backgroud color of the LCD can be adjusted with a potentiometer.

# Hardware
## Bill of materials
1. Character LCD, 1 pcs.
    - Should be compatible with the HD44780-controller. The LCD used here also has RGB-lighting.
2. Arduino Nano, 1 pcs.
3. 500 kOhm potentiometers, 2 pcs.
    - Used in the user input-gauges.
4. 10 kOhm potentiometer, 1 pcs.
    - To adjust contrast of the text on the LCD.
5. 10 kOhm resistor, 2 pcs.
    - Offset for potentiometers

## Circuit diagram
Circuit diagram was made with help from this website: https://www.circuit-diagram.org/editor/


### LCD
RegisterSelect = Arduino pin 2
Enable = Arduino pin 3
Data 4 = Arduino pin 4
Data 5 = Arduino pin 5
Data 6 = Arduino pin 6
Data 7 = Arduino pin 7
LED common anode = Arduino pin 8
Red LED cathode = Arduino pin 9
Green LED cathode = Arduino pin 10
Blue LED cathode = Arduino pin 11

### Potentiometers
Left potentiometer = A0
Right potentiometer = A1

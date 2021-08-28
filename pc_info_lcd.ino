#include <LiquidCrystal.h>
#include "LCD.h"
#include "Communications.h"

void setup() {
    setupLCD();
    processHardwareInputs();
    updateLCDColors();

    lcd.begin(16,2);
    lcd.clear();
    lcd.write("Hello, Dave!");
    delay(800);

    setupComs();
}

void processMessage() {
    switch(comBuffer[0]) {
        case 0x11:
            gpu_temperature = decodeFloat();
    }
}

void processHardwareInputs() {
    left_pot_updated = false;
    right_pot_updated = false;

    float temp = (float(analogRead(LEFT_POT_PIN)) - 90.0) / 1023.0;
    if (abs(temp - left_pot)  > 0.02) {
        left_pot_updated = true;
        left_pot = temp;
    }

    temp = (float(analogRead(RIGHT_POT_PIN)) - 90.0) / 1023.0;
    if (abs(temp - right_pot)  > 0.02) {
        right_pot_updated = true;
        right_pot = temp;
    }
}

float decodeFloat(){
    float f;
    byte temp[] = {comBuffer[1], comBuffer[2], comBuffer[3], comBuffer[4]};
    memcpy(&f, &temp, 4);
    return f;
}

void updateLCD(){
    lcd.setCursor(0, 0);
    char gpu_string[16] = "GPU: xx.y  C    ";
    gpu_string[10] = 223;
    char temp[4];
    dtostrf(gpu_temperature, 4, 1, temp);
    gpu_string[5] = temp[0];
    gpu_string[6] = temp[1];
    gpu_string[7] = temp[2];
    gpu_string[8] = temp[3];
    lcd.write(gpu_string);

    lcd.setCursor(0, 1);
    char pot_string[16] = "P1:xxxx P2:xxx";
    sprintf(temp, "%d.%02d", (int) left_pot, (int)(left_pot * 100) % 100);
    pot_string[3] = temp[0];
    pot_string[4] = temp[1];
    pot_string[5] = temp[2];
    pot_string[6] = temp[3];
    sprintf(temp, "%d.%02d", (int) right_pot, (int)(right_pot * 100) % 100);
    pot_string[11] = temp[0];
    pot_string[12] = temp[1];
    pot_string[13] = temp[2];
    pot_string[14] = temp[3];
    lcd.write(pot_string);
}

void loop() {
    uint8_t tries = 0;
    if ((Serial.available() % 8 == 0) and (Serial.available() > 0)){
      comBufferPtr += Serial.readBytes(comBuffer, 8);
      processMessage();
    }
    else if (Serial.available() > 0) {
        tries += 1;
        if (tries > 3) {
            while (Serial.available() > 0) {
                Serial.read();
            }
        }
    }
    delay(250);
    processHardwareInputs();
    updateLCD();
    if (left_pot_updated or right_pot_updated) {
        updateLCDColors();
    }
}

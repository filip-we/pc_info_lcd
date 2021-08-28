#include <LiquidCrystal.h>

struct RGB {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

const int RS = 2, EN = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7;
const int LED_ANODE = 8, RED_LED = 9, GREEN_LED = 10, BLUE_LED = 11;
const int LEFT_POT_PIN = A0, RIGHT_POT_PIN = A1;

const long BAUDRATE = 9600;
const int POT_MIN = 90, POT_MAX = 1023;

byte comBuffer[256];
uint8_t comBufferPtr = 0;

float gpu_temperature;
float left_pot = 0, right_pot = 0;
bool left_pot_updated;
bool right_pot_updated;

const RGB COLOR_PRESETS[31] = {
    {uint8_t(0),   uint8_t(0),    uint8_t(0)},
    {uint8_t(128), uint8_t(128),  uint8_t(128)},
    {uint8_t(191), uint8_t(191),  uint8_t(191)},
    {uint8_t(255), uint8_t(255),  uint8_t(255)},

    {uint8_t(0),   uint8_t(255),  uint8_t(255)},
    {uint8_t(0),   uint8_t(255),  uint8_t(191)},
    {uint8_t(0),   uint8_t(255),  uint8_t(128)},
    {uint8_t(0),   uint8_t(191),  uint8_t(255)},
    {uint8_t(0),   uint8_t(191),  uint8_t(191)},
    {uint8_t(0),   uint8_t(191),  uint8_t(128)},
    {uint8_t(0),   uint8_t(128),  uint8_t(255)},
    {uint8_t(0),   uint8_t(128),  uint8_t(191)},
    {uint8_t(0),   uint8_t(128),  uint8_t(128)},

    {uint8_t(255), uint8_t(0),    uint8_t(255)},
    {uint8_t(255), uint8_t(0),    uint8_t(191)},
    {uint8_t(255), uint8_t(0),    uint8_t(128)},
    {uint8_t(191), uint8_t(0),    uint8_t(255)},
    {uint8_t(191), uint8_t(0),    uint8_t(191)},
    {uint8_t(191), uint8_t(0),    uint8_t(128)},
    {uint8_t(128), uint8_t(0),    uint8_t(255)},
    {uint8_t(128), uint8_t(0),    uint8_t(191)},
    {uint8_t(128), uint8_t(0),    uint8_t(128)},

    {uint8_t(255), uint8_t(255),  uint8_t(0)},
    {uint8_t(255), uint8_t(191),  uint8_t(0)},
    {uint8_t(255), uint8_t(128),  uint8_t(0)},
    {uint8_t(191), uint8_t(255),  uint8_t(0)},
    {uint8_t(191), uint8_t(191),  uint8_t(0)},
    {uint8_t(191), uint8_t(128),  uint8_t(0)},
    {uint8_t(128), uint8_t(255),  uint8_t(0)},
    {uint8_t(128), uint8_t(191),  uint8_t(0)},
    {uint8_t(128), uint8_t(128),  uint8_t(0)},
};

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setBacklight(uint8_t red, uint8_t green, uint8_t blue){
    analogWrite(RED_LED, red);
    analogWrite(GREEN_LED, green);
    analogWrite(BLUE_LED, blue);
}

void setBacklight(RGB rgb){
    setBacklight(uint8_t(rgb.red),
            uint8_t(rgb.green),
            uint8_t(rgb.blue)
    );
}

void updateLCDColors(){
    uint8_t n = floor( (sizeof(COLOR_PRESETS) - 1) * left_pot);
    setBacklight(COLOR_PRESETS[n]);
}

void setupComs(){
    Serial.begin(BAUDRATE);
    Serial.setTimeout(1);
    delay(100);
    Serial.println("ok");
    while(!Serial){};
}

void setup() {
    digitalWrite(LED_ANODE, HIGH);
    pinMode(LED_ANODE, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
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

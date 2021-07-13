#include <LiquidCrystal.h>

const int RS = 2, EN = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7;
const int LED_ANODE = 8, RED_LED = 9, GREEN_LED = 10, BLUE_LED = 11;
const int LEFT_POT_PIN = A4, RIGHT_POT_PIN = A5;

const long BAUDRATE = 9600;
const int POT_MIN = 90, POT_MAX = 1023;

byte comBuffer[256];
uint8_t comBufferPtr = 0;

float gpu_temperature;
int left_pot = 0, right_pot = 0;
bool left_pot_updated;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setBacklight(uint8_t red, uint8_t green, uint8_t blue){
    analogWrite(RED_LED, red);
    analogWrite(GREEN_LED, green);
    analogWrite(BLUE_LED, blue);
}

void showOff(uint8_t red, uint8_t green, uint8_t blue){
    setBacklight(red, green, blue);
    delay(2000);
}

void setupComs(){
    Serial.begin(BAUDRATE);
    Serial.setTimeout(1);
    delay(100);
    //Serial.println("ok");
    //lcd.clear();
    //lcd.write("Connecting...");
    //while(!Serial){};
}

void setup() {
    digitalWrite(LED_ANODE, HIGH);
    pinMode(LED_ANODE, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    setBacklight(0, 230, 230);

    lcd.begin(16,2);
    lcd.clear();
    lcd.write("Hello, Dave!");
    delay(1000);

    setupComs();
}

void processMessage() {
    switch(comBuffer[0]) {
        case 0x11:
            gpu_temperature = decodeFloat();
    }
    //float gpu;
    //byte temp[] = {comBuffer[1], comBuffer[2], comBuffer[3], comBuffer[4]};
    //memcpy(&gpu, &temp, 4);

    //char string[8];
    //dtostrf(gpu, 1, 1, string);

    //lcd.clear();
    //lcd.write("GPU: ");
    //lcd.write(string);
    //lcd.write(" C");
}

void processHardwareInputs() {
    int temp = analogRead(LEFT_POT_PIN);
    if (abs(temp - left_pot)  > 3) {
        left_pot_updated = true;
        left_pot = temp;
    }
    else {
        left_pot_updated = false;
    }
}

float decodeFloat(){
    float f;
    byte temp[] = {comBuffer[1], comBuffer[2], comBuffer[3], comBuffer[4]};
    memcpy(&f, &temp, 4);
    return f;
}

void updateLCD(){
    lcd.clear();

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
    char pot_string[16] = "Pot.:          ";
    sprintf(temp, "%i", left_pot);
    pot_string[6] = temp[0];
    pot_string[7] = temp[1];
    pot_string[8] = temp[2];
    pot_string[9] = temp[3];
    lcd.write(pot_string);
}

void updateLCDColors(){
    if (left_pot < 91) {
        setBacklight(0, 230, 230);
    }
    else if (left_pot < 110) {
        setBacklight(230, 0, 230);
    }
    else if (left_pot < 130) {
        setBacklight(230, 230, 0);
    }
    else if (left_pot < 150) {
        setBacklight(230, 230, 230);
    }
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
    if (left_pot_updated) {
        updateLCDColors();
    }
}

void runLightShow(){
    uint8_t red = 255;
    uint8_t green = 255;
    uint8_t blue = 255;

    lcd.clear();
    lcd.write("Green");
    for (int i = 255; i >= 0; i--){
      delay(25);
      green -= 1;
      setBacklight(red, green, blue);
    }

    lcd.clear();
    lcd.write("Blue");
    for (int i = 255; i >= 0; i--){
      delay(25);
      blue -= 1;
      setBacklight(red, green, blue);
    }

    lcd.clear();
    lcd.write("Red");
    for (int i = 255; i >= 0; i--){
      delay(25);
      red -= 1;
      setBacklight(red, green, blue);
    }


    lcd.clear();
    lcd.write("Some colors!");
    delay(2000);
    showOff(255, 255, 255);

    showOff(0,   128,   128);
    showOff(0,   255,   128);
    showOff(0,   128,   255);
    showOff(0,   255,   255);

    showOff(128, 128,     0);
    showOff(128, 255,     0);
    showOff(255, 128,     0);
    showOff(255, 255,     0);

    showOff(128,   0,   128);
    showOff(255,   0,   128);
    showOff(128,   0,   255);
    showOff(255,   0,   255);

    showOff(128, 128,   128);
    showOff(128, 255,   128);
    showOff(128, 128,   255);
    showOff(128, 255,   255);

    showOff(255, 128,   128);
    showOff(255, 255,   128);
    showOff(255, 128,   255);
    showOff(255, 255,   255);

}

#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int LED_ANODE = 8, RED_LED = 9, GREEN_LED = 10, BLUE_LED = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const long BAUDRATE = 9600;

byte comBuffer[256];
uint8_t comBufferPtr = 0;

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
    //char string[8];
    //memcpy(string, comBuffer, 8);
    float gpu;
    byte temp[] = {comBuffer[1], comBuffer[2], comBuffer[3], comBuffer[4]};
    memcpy(&gpu, &temp, 4);

    char string[8];
    dtostrf(gpu, 1, 1, string);

    lcd.clear();
    lcd.write("GPU: ");
    lcd.write(string);
    lcd.write(" C");
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
    delayMicroseconds(1000);
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

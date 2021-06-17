#include <LiquidCrystal.h>

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int LED_ANODE = 8, RED_LED = 9, GREEN_LED = 10, BLUE_LED = 11;
//uint8_t RED = 255, GREEN = 255, BLUE = 255;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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
    Serial.begin(9600);
    Serial.println("ok");
    lcd.write("Hello!");
    while(!Serial){};
    lcd.write(" Connected");
}

void setup() {
    pinMode(LED_ANODE, OUTPUT);
    digitalWrite(LED_ANODE, HIGH);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    setBacklight(0, 230, 230);

    setupComs();

    lcd.begin(16,2);
    lcd.clear();
}

void loop() {
  if (Serial.available() > 1){
    //lcd.clear();
    lcd.write(Serial.read());
//    Serial.write("Received");
  }
//  delayMicroseconds(1000);
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
   //
    showOff(128, 128,   128);
    showOff(128, 255,   128);
    showOff(128, 128,   255);
    showOff(128, 255,   255);
   
    showOff(255, 128,   128);
    showOff(255, 255,   128);
    showOff(255, 128,   255);
    showOff(255, 255,   255);

}

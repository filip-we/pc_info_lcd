const long BAUDRATE = 9600;
const int POT_MIN = 90, POT_MAX = 1023;

byte comBuffer[256];
uint8_t comBufferPtr = 0;

void setupComs(){
    Serial.begin(BAUDRATE);
    Serial.setTimeout(1);
    delay(100);
    Serial.println("ok");
    while(!Serial){};
}

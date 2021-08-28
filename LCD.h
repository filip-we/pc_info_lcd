struct RGB {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

const int RS = 2, EN = 3, D4 = 4, D5 = 5, D6 = 6, D7 = 7;
const int LED_ANODE = 8, RED_LED = 9, GREEN_LED = 10, BLUE_LED = 11;
const int LEFT_POT_PIN = A0, RIGHT_POT_PIN = A1;

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

void setupLCD() {
    digitalWrite(LED_ANODE, HIGH);
    pinMode(LED_ANODE, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
 }

#include <FastLED.h>

#define NUM_LEDS 65
#define LED_PIN 7
#define SPEED 0.1
#define DIM_SPEED 500

int const buttonPin = 3;
int const potPin = A2;
int const colPin = A0;
int const satPin = A1;

float colVal = 100;
int colValTemp = 100;
int HIGH_VAL = 255;
int satVal = 255;
int button = 0;
int mode = -1;

CRGB led[NUM_LEDS];

//TODO: Prevent flickering by only updating by intervals of 10
//TODO: Add button interrupt

void setup() {
  pinMode(3, INPUT);

  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);
  
  for(int i = 0; i < NUM_LEDS; i++){
    led[i] = CHSV(colVal, satVal, HIGH_VAL);
  }
  FastLED.show();
}

void loop() {
  button = digitalRead(buttonPin);
  if(button != 0){
    mode *= -1;

    delay(400);

  //DEBUG
  //  Serial.print(", ");
  //  Serial.print(satVal);
  //  Serial.print(", ");
  //  Serial.print(HIGH_VAL);
  }

  if(mode == 1){ //Manually set colors
    //Read Values
    HIGH_VAL = analogRead(potPin); // read the value of the potentiometer
    colValTemp = (int)analogRead(colPin);
    satVal = analogRead(satPin) -13; //-13 to prevent flashing
    colValTemp = map(colValTemp, 80, 920, 0, 255);
    HIGH_VAL = map(HIGH_VAL, 0, 1000, 0, 255);
    satVal = map(satVal, 0, 1000, 0, 255);

    //Avoid too low light case
    if(HIGH_VAL < 8){
      HIGH_VAL = 0;
    }
    
    if(colValTemp != colVal && colValTemp != colVal -1 && colValTemp != colVal + 1){
      colVal = colValTemp;
    }

    //Update LEDs
    for(int i = 0; i < NUM_LEDS; i++){
      led[i] = CHSV(colVal, satVal, HIGH_VAL);
    }
    FastLED.show();

    //Debug
    Serial.print("Color: ");
    Serial.print(analogRead(colPin));
    Serial.print("\n");
    
  } else { //Cycle through colors
    HIGH_VAL = analogRead(potPin); // read the value of the potentiometer
    satVal = analogRead(satPin) -13; //-13 to prevent flickering
    HIGH_VAL = map(HIGH_VAL, 0, 1000, 0, 255);
    satVal = map(satVal, 0, 1000, 0, 255);

    //update LEDs
    for(int i = 0; i < NUM_LEDS; i++){
      led[i] = CHSV(colVal, 180, 180);
      //led[i] = CHSV(colVal, satVal, HIGH_VAL); //this line currently causes flickering
    }
    FastLED.show();
    delay(SPEED);

    //Debug
    Serial.print("Saturation: ");
    Serial.print(satVal);
    Serial.print("\n");

    //Cycle through colors
    if(colVal < 255){
      colVal = colVal + 0.01;
    } else {
      colVal = 0;
    }
  }
}

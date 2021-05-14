//VeloNoRush Small Model
//Pins that will be selected for the A,B,C, and D parameters of the MUX
const byte pinAin = 6; 
const byte pinBin = 7;    
const byte pinCin = 8;
const byte pinDin = 9;
const byte pinAout = 10;
const byte pinBout = 11;    
const byte pinCout = 12;
const byte pinDout = 13;

//Pins for the MUX and Buttons
const byte voltPin = A0;
const byte colorPin = A1;

//Storage for all voltage readings
const int numOfSensors = 256; //32
int voltageReadings[numOfSensors]; //Where voltage readings are stored
int ledBrightness[numOfSensors]; //storage for LED brightness
byte globalHue;
byte globalSat = 255;
bool colorChanged = false;

//Pins for the mode switch
const byte modeSwitchPin = 4;
const byte eraserPin = 3;
byte frameBufferHue[numOfSensors];  //Where all the hues are stored
byte frameBufferSat[numOfSensors];  //Where all the Saturations are stored
bool switchToPaint = false;         //Flag to reset the canvas

/* USE OF FASTLED LIBRARY*/
#include <FastLED.h>
#define NUM_LEDS 256
#define LED_PIN 5
#define MAX_BRIGHTNESS 100
#define MID_BRIGHTNESS 50
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB matrixLEDs[NUM_LEDS]; //FastLED container for LEDs

void setup() 
{
  //Setup of MUX pins for port selection
  pinMode(pinAout, OUTPUT);
  pinMode(pinBout, OUTPUT);
  pinMode(pinCout, OUTPUT);
  pinMode(pinDout, OUTPUT);
  pinMode(pinAin, OUTPUT);
  pinMode(pinBin, OUTPUT);
  pinMode(pinCin, OUTPUT);
  pinMode(pinDin, OUTPUT);
  //Outputs of the MUX wired into A0 and A1 of Arduino
  pinMode(voltPin, INPUT);
  pinMode(colorPin, INPUT);
  //Setup for mode switch
  pinMode(modeSwitchPin, INPUT_PULLUP);
  pinMode(eraserPin, INPUT_PULLUP);
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(matrixLEDs, NUM_LEDS);
  FastLED.setBrightness(MAX_BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  delay(1000);
  Serial.begin(9600);
}

void loop() 
{
  //Obtain voltage
  GetVoltages();
  bool switchStatus = digitalRead(modeSwitchPin);
  //Paint Mode
  if (switchStatus == HIGH)
  {
    if (switchToPaint == false)
    {
      switchToPaint = true;
      for(int i = 0; i < numOfSensors; i++)
      {
        ledBrightness[i] = 0;
      }
    }
    GetColor();
    PaintMode();
  }
  else
  {
    ConvertVoltageToBrightness();
    switchToPaint = false;
    GetColor();
  }
  if (colorChanged == true)
  {
    FastLED.show();
    delay(250);
  }
  else
  {
  //Tracking Mode
    AlignLEDs(switchStatus);
  }
}
void GetVoltages()
{
  bool pinAinval, pinBinval, pinCinval, pinDinval = 0;      //Storage for if A,B,C,D is on or off
  bool pinAoutval, pinBoutval, pinCoutval, pinDoutval = 0;      //Storage for if A,B,C,D is on or off
  int storageCounter = 0;
  //GET THE VOLTAGES FROM ALL 16 PINS ON THE MUX
  for (int o = 16; o > 0; o--) //for (int o = 0; o < 16; o++)
  {
    //Set the selection pins of the output MUX
    int currentPin = o;
    pinAoutval = currentPin % 2;
    currentPin/=2;
    pinBoutval = currentPin % 2;
    currentPin/=2;
    pinCoutval = currentPin % 2;
    currentPin/=2;
    pinDoutval = currentPin % 2;
    digitalWrite(pinAout, pinAoutval);
    digitalWrite(pinBout, pinBoutval);
    digitalWrite(pinCout, pinCoutval);
    digitalWrite(pinDout, pinDoutval);
    
    for(int i = 16; i > 0; i--) //for(int i = 0; i < 16; i++)
    {
      //Set the selection pins of the input MUX
      int currentPin = i;
      pinAinval = currentPin % 2;
      currentPin/=2;
      pinBinval = currentPin % 2;
      currentPin/=2;
      pinCinval = currentPin % 2;
      currentPin/=2;
      pinDinval = currentPin % 2;
      digitalWrite(pinAin, pinAinval);
      digitalWrite(pinBin, pinBinval);
      digitalWrite(pinCin, pinCinval);
      digitalWrite(pinDin, pinDinval);
      int voltageStorage = 0;
      voltageStorage = analogRead(voltPin);
      voltageReadings[storageCounter] = voltageStorage;
      storageCounter++;
    }
  }
}

void ConvertVoltageToBrightness()
{
  for(int i = 0; i < numOfSensors; i++)
  {
    ledBrightness[i] = map(voltageReadings[i], 0, 617, 0, MAX_BRIGHTNESS);
  }
}
void PaintMode()
{
  //Storage for the spot in the voltages readings of higehst pressure
  int highestVoltSpot = 0;
  int highestVoltage = 0;
  bool eraserStatus = digitalRead(eraserPin);
  //Check all voltage readings for the highest pressure
  for(int i = 0; i < numOfSensors; i++)
  {
    if (voltageReadings[i] > highestVoltage)
    {
      highestVoltage = voltageReadings[i];
      highestVoltSpot = i;
    }
  }
  //Application of a color to the highest pressure
  if (eraserStatus == HIGH)
  {
    ledBrightness[highestVoltSpot - 1] = 0;
    ledBrightness[highestVoltSpot - 16] = 0;
    ledBrightness[highestVoltSpot] = 0;
    ledBrightness[highestVoltSpot + 1] = 0;
    ledBrightness[highestVoltSpot + 16] = 0;
    frameBufferHue[highestVoltSpot] = globalHue;
    frameBufferSat[highestVoltSpot] = globalSat;
  }
  else
  {
    ledBrightness[highestVoltSpot] = MAX_BRIGHTNESS;
    frameBufferHue[highestVoltSpot] = globalHue;
    frameBufferSat[highestVoltSpot] = globalSat;
  }
}
void GetColor()
{
  int volt = analogRead(colorPin);
  byte ledHue = 0;
  byte ledSat = globalSat;
  colorChanged = true; 
  if (volt < 70)                      //Lower limit for first button - if below this limit then no button is pushed and LEDs are turned off
  {
    ledHue = globalHue;
    colorChanged = false; 
  }
  else if (volt < 80)                 //Press First and Second button at the same time
  {
    ledHue = 0;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 110)                //First button limit - if below this limit but above previous limit then the first button is pressed
  {
    ledHue = 32;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 140)                //Second button limit
  {
    ledHue = 64;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 170)                //Third button limit
  {
    ledHue = 96;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 210)                //Fourth button limit
  {
    ledHue = 128;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 260)                //Fifth button limit
  {
    ledHue = 160;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 350)                //Sixth button limit
  {
    ledHue = 192;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else if (volt < 500)                //Seventh button limit
  {
    ledHue = 224;
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  else                                //If none of the previous buttons are pressed, then the fifth button must be pressed                                
  {
    ledSat += 8;
    if (ledSat == 255)
    {
      ledSat += 1;
    }
    ledHue = globalHue;
  }
  if (ledSat != globalSat)
  {
    fill_solid(matrixLEDs, NUM_LEDS, CHSV(ledHue, ledSat, MID_BRIGHTNESS));
  }
  globalHue=ledHue;
  globalSat=ledSat;
}
void AlignLEDs(bool switchStatus)
{
  int ledCount = 0;
  int ledPosition = 0;
  bool isOdd = false;
  byte ledHue = 0;
  byte ledSat = 0;
  //Serial.println("Printing the Data Values...");
  for (int row = 0; row < 16; row ++)
  {
    if (row % 2 == 1)
      isOdd = true;
    else
      isOdd = false;
    for (int col = 0; col < 16; col++)
    {
      if (isOdd == true)
      {
        ledPosition = (row * 16) + col;
      }
      else
      {
        ledPosition = ((row * 16) + 15) - col;
      }

      if (switchStatus == HIGH)
      {
        ledHue = frameBufferHue[ledCount];
        ledSat = frameBufferSat[ledCount];        
      }
      else if (switchStatus == LOW)
      {
        ledHue = globalHue;
        ledSat = globalSat;  
      }
      matrixLEDs[ledPosition] = CHSV(ledHue, ledSat, ledBrightness[ledCount]);
      ledCount++;
      //Serial.print(ledBrightness[ledCount]);
      //Serial.print("  ");
    }
    //Serial.println("");
  }
  FastLED.show();
}

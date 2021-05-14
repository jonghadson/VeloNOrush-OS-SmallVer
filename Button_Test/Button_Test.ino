int colorPin = A1;     //Button and LED pin numbers
byte globalHue;
byte globalSat;
void setup() 
{
    Serial.begin(9600);           //Serial monitor used to determine limit values
}

void loop() 
{
  int voltage = analogRead(colorPin);
  int volt = voltage;
  byte ledHue = 0;
  byte ledSat = globalSat; 
  if (volt < 70)                     //Lower limit for first button - if below this limit then no button is pushed and LEDs are turned off
  {
    ledHue = globalHue;
  }
  else if (volt < 80) //Press First and Second button at the same time
  {
    ledHue = 0;
  }
  else if (volt < 100)                //First button limit - if below this limit but above previous limit then the first button is pressed
  {
    ledHue += 32;
  }
  else if (volt < 130)                //Second button limit
  {
    ledHue = 64;
  }
  else if (volt < 160)                //Third button limit
  {
    ledHue = 96;
  }
  else if (volt < 200)                //Fourth button limit
  {
    ledHue = 128;
  }
  else if (volt < 250)
  {
    ledHue = 160;
  }
  else if (volt < 350)
  {
    ledHue = 192;
  }
  else if (volt < 500)
  {
    ledSat += 15;
    if (ledSat == 255)
    {
      ledSat += 1;
    }
    delay(500); 
  }
  else                                //If none of the previous buttons are pressed, then the fifth button must be pressed                                
  {
    if (ledSat == 0)
    {
      ledSat -= 1;
    }
    ledSat -= 15;
    delay(500); 
  }
  globalHue=ledHue;
  globalSat=ledSat;
  Serial.print("Voltage: ");
  Serial.println(volt);               //Display the read value in the Serial monitor
//  Serial.print("globalHue: ");
//  Serial.println(globalHue);
//  Serial.print("ledHue: ");
//  Serial.println(ledHue);
//  Serial.print("globalSat: ");
//  Serial.println(globalSat);
//  Serial.print("ledSat: ");
//  Serial.println(ledSat);
  delay(1000);                         //Delay for stability
}

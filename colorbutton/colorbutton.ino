const int colorBtnPin = 12;
bool cotorBtnVal = 0;
bool colorBtnLast = 0;
byte globalHue = 0;
byte colorChoice = 0;
void setup() 
{
  // put your setup code here, to run once:
  pinMode(colorBtnPin, INPUT_PULLUP); 
}

void loop() 
{
  // put your main code here, to run repeatedly:

  // 1 means no press, 0 means a press
  colorBtnVal = digitalRead(colorBtnPin);
  if (colorBtnVal == 0 && colorBtnLast == 1)
  {
    colorBtnLast = 0; // 255 / 5
    //globalHue += 15;
    if (colorChoice == 9)
    {
      colorChoice = 0;
    }
    else
      colorChoice++;
  }
  if (colorBtnVal == 1 && colorBtnLast == 0)
  {
    colorBtnLast = 1;
    delay(20);
  }
  switch(colorChoice)
  {
    case 0:
    {
      globalHue = 0;
      break;
    }
    case 1:
    {
      globalHue = 15;
      break;
    }
    case 2:
    {
      globalHue = 0;
      break;
    }
      
  }
}

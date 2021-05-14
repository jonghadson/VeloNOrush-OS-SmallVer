double blueLEDdelay = 400; //how much time to wait betwen loops in ms
double blueLEDlastRun = 0;

double currentTime = 0;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis(); //Store current arduino time
  if ((currentTime - blueLEDlastRun >= blueLEDdelay))
  {
    //Toggle blue LED
    //Check weight sensors
    blueLEDlastRun = currentTime;
  }
  if ((currentTime - redLEDlastRun >= redLEDdelay))
  {
    //Toggle red LED
    redLEDlastRun = currentTime;
  }
  delay(400);
  // Toggle blue LED
  delay(350); // to make it 750
  //Toggle red LED
  
}

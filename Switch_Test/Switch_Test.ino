const byte modeSwitchPin = 4;
void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int voltage = digitalRead(modeSwitchPin);
  Serial.print("Voltage: ");
  Serial.println(voltage);
}

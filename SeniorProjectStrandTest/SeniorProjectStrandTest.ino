//LED CHAIN TESTER!
#include <FastLED.h>

#define NUM_LEDS        256       //Number of LEDs in the chain
#define LED_PIN         5         //Pin the DATA line is connected to on Arduino
#define MAX_BRIGHTNESS  75        //Maximum possible brightness for the LEDs, max is 255
#define LED_TYPE        WS2812B   //The chipset that's inside the LEDs
#define COLOR_ORDER     GRB       //The order that the chips want the data in (Green, Red, Blue)

CRGB led_strand[NUM_LEDS];        //The container that holds the data for our LEDs

void setup() 
{
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(led_strand, NUM_LEDS); //.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(MAX_BRIGHTNESS);
}

void loop() 
{
  int delayTime = 50;
  // put your main code here, to run repeatedly:
  //turn leds red
  for(int i = 0; i < NUM_LEDS; i++)
  {
    if(i % 2 == 0)
    led_strand[i] = CRGB::Red;
    
    FastLED.show();
    delay(delayTime);
  }

  for(int i = 0; i < NUM_LEDS; i++)
  {
    led_strand[i] = CRGB::Green;
    FastLED.show();
    delay(delayTime);
  }

  for(int i = 0; i < NUM_LEDS; i++)
  {
    led_strand[i] = CRGB::Blue;
    FastLED.show();
    delay(delayTime);
  }

  for(int i = 0; i < NUM_LEDS; i++)
  {
    led_strand[i] = CRGB::Purple;
    FastLED.show();
    delay(delayTime);
  }

  delay(1000);
  FastLED.clear();
  delay(1000);
}

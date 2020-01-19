//We always have to include the library
#include "LedControl.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;


unsigned long startTime = 0;

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 11

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 pin 2 connected to ds18b20
 pinout:
 gnd, data, vcc (from left)
 black,yellow,red (waterproof)
 4.7k resistor between data and VCC
 in parasite mode gnd and vcc connected to GND
 */
LedControl lc=LedControl(12,11,10,1);
#define MAX_THERM 2
//int res[MAX_THERM]={0,0},numDev=0;
float temp[MAX_THERM];

  
void setup() 
{
 // Serial.begin(9600);
  clock.begin();
  lc.shutdown(0,false);
  lc.setIntensity(0,2);
  lc.clearDisplay(0);
 
  }

void loop() 
{ 
  unsigned long loopTime = millis() - startTime; //Calculate the time since last time the cycle was completed

//clock.setClockMode(true);
   if (loopTime <= 20000) //Check if the time is less than 20000 millis, and if so, run loop 1
 {

  clock.forceConversion();
 int i=0;
    temp[i] = clock.readTemperature();
//  Serial.println(clock.readTemperature());

    dt = clock.getDateTime();
   displayClock(dt.hour,dt.minute,dt.second);
    //displayClockAndTemp(dt.hour,dt.minute,dt.second,temp[0]);
 }
  else if (loopTime > 20000&& loopTime <= 25000) //If time is over 20000 millis and less than/or 25000 millis, run loop 2
 {

  displayTemp(temp[0]);
 }
 else if (loopTime > 26000) //If time is over 2000 millis, set the startTime to millis so the loop time will be reset to zero
 {
   startTime = millis();
 }
}


void displayClock(int h, int m, int s)
{
  lc.clearDisplay(0);
  lc.setDigit(0,6,h/10,false);
  lc.setDigit(0,5,h%10,true);
  lc.setDigit(0,4,m/10,false);
  lc.setDigit(0,3,m%10,true);
  lc.setDigit(0,2,s/10,false);
  lc.setDigit(0,1,s%10,false);
}

void displayClockAndTemp(int h, int m, int s,float t)
{
  lc.setDigit(0,7,h/10,false);
  lc.setDigit(0,6,h%10,true);
  lc.setDigit(0,5,m/10,false);
  lc.setDigit(0,4,m%10,false);
  displayVal(0,t);

}
void displayVal(int idx, float val)
{
  lc.setChar(0,idx+0,'\'',false);
  if(val<0 && val>-10.0) {
    lc.setChar(0,idx+3,'-',false);
    lc.setDigit(0,idx+2,(int)val%10,true);
    lc.setDigit(0,idx+1,(val-(int)val)*10,false);
  } else if(val<-10.0) {
    lc.setChar(0,idx+3,'-',false);
    lc.setDigit(0,idx+2,(int)val/10,false);
    lc.setDigit(0,idx+1,(int)val%10,true);
  } else {
    lc.setDigit(0,idx+3,(int)val/10,false);
    lc.setDigit(0,idx+2,(int)val%10,true);
    lc.setDigit(0,idx+1,(val-(int)val)*10,false);
  }
}

void displayTemp(float t)
{
  lc.clearDisplay(0);
   displayVal(2,t);
  lc.setChar(0, 2, 'C', false);
  
  delay(500);
}


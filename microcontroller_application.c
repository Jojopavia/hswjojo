#include <Wire.h>                     // I2C EEPROM
#include <SPI.h>                      // SPI PROTOCOL
#include <SD.h>                       // API EXTERNAL MEMORY
#include <SENSOR.h>                   // SENSOR reference
#include <Temperature.h>

#define TIMESTAMP 900000              // 15 minutes in miliseconds 

void setup () 
{
  Serial.begin(115200);
}

void loop() 
  unsigned long currentTime = millis();
  unsigned long lastTime = 0;
{
  if (currentTime - lastTime >= TIMESTAMP)
  {
  lastTime=currentTime;

#include <PS3USB.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "PS3Robot.h"

USB usb;
PS3USB PS3(&usb);
PS3Robot PS3Controller(usb);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() 
{
}

void loop() 
{
  usb.Task();

  if(PS3Controller.checkConnection(PS3))
  {
    servoStruct servoControlled;
    servoControlled = PS3Controller.checkController(servoControlled, PS3);
    PS3Controller.moveStructure(servoControlled, pwm);
  }
}

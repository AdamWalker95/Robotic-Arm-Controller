/*This is the main class of the 'Arm', it takes functions from multiple libraries 
(including one built by myself) and operates them in an easy to read main implementation. */

#include <PS3USB.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "PS3Robot.h"
#include "servoStruct.h"

USB usb;
PS3USB PS3(&usb);
PS3Robot PS3Controller(PS3, usb);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() 
{
   pwm.begin();
   pwm.setPWMFreq(50);  
}

void loop() 
{

  if(PS3Controller.checkConnection())
  {
    servoStruct servoControlled;
    PS3Controller.checkController(&servoControlled);
    PS3Controller.moveStructure(servoControlled, pwm);   
  }
}

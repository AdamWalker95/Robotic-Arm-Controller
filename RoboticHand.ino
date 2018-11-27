#include <PS3USB.h>
#include <SPI.h>
#include <Servo.h>
#include "PS3Robot.h"

const int servoCount = 6;
const int servos[servoCount] { 7,6,5,4,3,2};
const int digitalPins[servoCount] {8,9,10,11,12,13};
USB usb;
PS3USB PS3(&usb);
PS3Robot PS3Controller(servos, digitalPins, servoCount, usb);

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
    PS3Controller.moveStructure(servoControlled);
  }
}

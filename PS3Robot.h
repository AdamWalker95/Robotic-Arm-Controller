#include <Servo.h>
#include <Arduino.h>
#include <PS3USB.h>
#include <SPI.h>
#include "servoStruct.h"

class PS3Robot
{
  Servo *servo[];
  int *digitalPins[];
  const int SerialNum = 115200;
  void calibratePins(int *s, int *p, int i);
  void calibrateUsb(USB &usb);
  void buttonControlled(servoStruct &s);
  void joystickControlled(servoStruct &s);
  
  public:
  PS3Robot(int *s, int *p, int i, USB &usb);
  ~PS3Robot();
  bool checkConnection(PS3USB &PS3);
  servoStruct &checkController(servoStruct &s, PS3USB &PS3);
  void moveStructure(servoStruct &s);
};

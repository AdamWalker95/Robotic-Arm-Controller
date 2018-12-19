#include <Arduino.h>
#include <PS3USB.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "servoStruct.h"

class PS3Robot
{
  void calibrateUsb(USB &usb);
  void buttonControlled(servoStruct &s, Adafruit_PWMServoDriver &pwm);
  void joystickControlled(servoStruct &s, Adafruit_PWMServoDriver &pwm);
  void buttonForward(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength);
  void buttonForwardReset(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength);
  void buttonBackward(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength);
  void buttonBackwardReset(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength);
  
  public:
  PS3Robot(USB &usb);
  bool checkConnection(PS3USB &PS3);
  servoStruct &checkController(servoStruct &s, PS3USB &PS3);
  void moveStructure(servoStruct &s, Adafruit_PWMServoDriver &pwm);
};

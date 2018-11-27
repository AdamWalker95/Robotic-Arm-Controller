#include "PS3Robot.h"

  PS3Robot::PS3Robot(int *s, int *p, int i, USB &usb)
  {
      *servo = new Servo[i];
      *digitalPins = new int[i];
      
      calibratePins(s,p,i);

      calibrateUsb(usb);
      
  }
  PS3Robot::~PS3Robot()
  {
    delete []servo;
    delete []digitalPins;
  }
  void PS3Robot::calibratePins(int *s, int *p, int i)
  {
    for(int count = 0; count <i;count++)
       {
        servo[count]->attach(s[count]);
        pinMode(digitalPins[p[count]], OUTPUT);
        digitalWrite(digitalPins[p[count]], HIGH);
       }
  }
  void PS3Robot::calibrateUsb(USB &usb)
  {
     Serial.begin(SerialNum);
     
     if (usb.Init() == -1) {
        Serial.print(F("\r\nOSC did not start"));
        while (1); //halt
      }
      Serial.print(F("\r\nPS3 USB Library Started"));
  }

  bool PS3Robot::checkConnection(PS3USB &PS3)
  {
    if(PS3.PS3Connected || PS3.PS3NavigationConnected)
      return true;
    else
      return false;
  }
  servoStruct &PS3Robot::checkController(servoStruct &s, PS3USB &PS3)
  {
    servoStruct servo;
    if(PS3.getAnalogHat(RightHatX) > 130 || PS3.getAnalogHat(RightHatX) < 120)
    {
      servo.servoNum = 1;
      servo.range = map(PS3.getAnalogHat(RightHatX), 0, 255, 0, 180);
      servo.isntbutton = false;
      return servo;
    }
    else if(PS3.getAnalogHat(RightHatY) > 130 || PS3.getAnalogHat(RightHatY) < 120)
    {
      servo.servoNum = 2;
      servo.range = map(PS3.getAnalogHat(RightHatY), 0, 255, 0, 180);
      servo.isntbutton = false;
      return servo;
    }
    else if(PS3.getAnalogHat(LeftHatY) > 130 || PS3.getAnalogHat(LeftHatY) < 120)
    {
      servo.servoNum = 3;
      servo.range = map(PS3.getAnalogHat(LeftHatY), 0, 255, 0, 180);
      servo.isntbutton = false;
      return servo;
    }
    else if(PS3.getAnalogButton(L2) > 5)
    {
      servo.servoNum = 4;
      servo.buttonOn = true;
      servo.isbutton = true;
      return servo;
    }
    else if(PS3.getAnalogButton(R2) > 5)
    {
      servo.servoNum = 5;
      servo.buttonOn = true;
      servo.isbutton = true;
      return servo;
    }
  }
  void PS3Robot::moveStructure(servoStruct &s)
  {
    if(s.isbutton)
      buttonControlled(s);
    else
      joystickControlled(s);
  }
  void PS3Robot::buttonControlled(servoStruct &s)
  {
    for(int i = 0; s.buttonOn; i=i+100)
    {
      servo[s.servoNum]->writeMicroseconds(i);
      delayMicroseconds(100);
    }
  }
  void PS3Robot::joystickControlled(servoStruct &s)
  {
    servo[s.servoNum]->write(s.range);
  }

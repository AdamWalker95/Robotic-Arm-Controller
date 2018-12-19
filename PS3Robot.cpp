#include "PS3Robot.h"

  PS3Robot::PS3Robot(USB &usb)
  {
      calibrateUsb(usb);
  }
  void PS3Robot::calibrateUsb(USB &usb)
  {
     Serial.begin(115200);
     
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
      servo.range = map(PS3.getAnalogHat(RightHatX), 0, 255, 0.001, 0.002);
      servo.isntbutton = false;
      return servo;
    }
    else if(PS3.getAnalogHat(RightHatY) > 130 || PS3.getAnalogHat(RightHatY) < 120)
    {
      servo.servoNum = 2;
      servo.range = map(PS3.getAnalogHat(RightHatY), 0, 255, 0.001, 0.002);
      servo.isntbutton = false;
      return servo;
    }
    else if(PS3.getAnalogHat(LeftHatY) > 130 || PS3.getAnalogHat(LeftHatY) < 120)
    {
      servo.servoNum = 3;
      servo.range = map(PS3.getAnalogHat(LeftHatY), 0, 255, 0.001, 0.002);
      servo.isntbutton = false;
      return servo;
    }
    else if(PS3.getButtonClick(L1))
    {
      servo.servoNum = 4;
      servo.buttonOn = true;
      servo.isForward = true;
      servo.isbutton = true;
      return servo;
    }
    else if(PS3.getButtonClick(R1))
    {
      servo.servoNum = 4;
      servo.buttonOn = true;
      servo.isForward = false;
      servo.isbutton = true;
      return servo;
    }
    else if(PS3.getButtonClick(CROSS))
    {
      servo.servoNum = 5;
      servo.buttonOn = true;
      servo.isForward = true;
      servo.isbutton = true;
      return servo;
    }
    else if(PS3.getButtonClick(TRIANGLE))
    {
      servo.servoNum = 5;
      servo.buttonOn = true;
      servo.isForward = false;
      servo.isbutton = true;
      return servo;
    }
    else if(PS3.getButtonClick(UP))
    {
      servo.servoNum = 6;
      servo.buttonOn = true;
      servo.isForward = true;
      servo.isbutton = true;
      return servo;
    }
    else if(PS3.getButtonClick(DOWN))
    {
      servo.servoNum = 6;
      servo.buttonOn = true;
      servo.isForward = false;
      servo.isbutton = true;
      return servo;
    }
  }
  void PS3Robot::moveStructure(servoStruct &s, Adafruit_PWMServoDriver &pwm)
  {
    if(s.isbutton)
      buttonControlled(s, pwm);
    else
      joystickControlled(s, pwm);
  }
  void PS3Robot::buttonControlled(servoStruct &s, Adafruit_PWMServoDriver &pwm)
  {
    int pulselength = 1000000;   // 1,000,000 us per second
    pulselength /= 60;   // 60 Hz
    pulselength /= 4096;  // 12 bits of resolution

    if(s.isForward)
      buttonForward(s, pwm, pulselength);
    else
      buttonBackward(s, pwm, pulselength);

  }
  void PS3Robot::buttonForward(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength)
  {
    double conversion;
    for(double i = 0.001; s.buttonOn; i +=0.0001)
    {   
      
      if(i < 0.002)
      {
        conversion = i * 1000000;  // convert to us
        conversion /= pulselength;
        pwm.setPWM(s.servoNum, 0, conversion);
      }
      else
      {
        pwm.setPWM(s.servoNum, 0, 0.002);
      }   
      delay(100);
   }
   buttonForwardReset(s, pwm, pulselength);
 }
  void PS3Robot::buttonForwardReset(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength)
  {
    double conversion;
    for(double i = 0.002; i > 0.0015; i -=0.0001)
   {   
      conversion = i * 1000000;  // convert to us
      conversion /= pulselength;
      pwm.setPWM(s.servoNum, 0, conversion);
      delay(100);
      if(s.buttonOn)
      {
        buttonForward(s, pwm, pulselength);
      }
   }
  }
  void PS3Robot::buttonBackward(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength)
  {
    double conversion;
    for(double i = 0.001; s.buttonOn; i -=0.0001)
    {   
      
      if(i > 0.001)
      {
        conversion = i * 1000000;  // convert to us
        conversion /= pulselength;
        pwm.setPWM(s.servoNum, 0, conversion);
      }
      else
      {
        pwm.setPWM(s.servoNum, 0, 0.001);
      }   
      delay(100);
   }
   buttonBackwardReset(s, pwm, pulselength);
 }
  void PS3Robot::buttonBackwardReset(servoStruct &s, Adafruit_PWMServoDriver &pwm, int pulselength)
  {
    double conversion;
    for(double i = 0.002; i > 0.0015; i +=0.0001)
   {   
      conversion = i * 1000000;  // convert to us
      conversion /= pulselength;
      pwm.setPWM(s.servoNum, 0, conversion);
      delay(100);
      if(s.buttonOn)
      {
        buttonBackward(s, pwm, pulselength);
      }
   }
  }
  void PS3Robot::joystickControlled(servoStruct &s, Adafruit_PWMServoDriver &pwm)
  {
    int pulselength = 1000000;   // 1,000,000 us per second
    pulselength /= 60;   // 60 Hz
    pulselength /= 4096;  // 12 bits of resolution
    s.range *= 1000000;  // convert to us
    s.range /= pulselength;
    pwm.setPWM(s.servoNum, 0, s.range);
  }

#include "Arduino.h"
#include "PS3Robot.h"
#include <SPI.h>
#include <Wire.h>
#include <PS3USB.h>
#include <Adafruit_PWMServoDriver.h>
#include "servoStruct.h"

  PS3Robot::PS3Robot(PS3USB &PS3, USB &usb)
  {
    this->PS3 = &PS3;
    this->usb = &usb;
  }
  PS3Robot::~PS3Robot()
  {
    delete PS3;
    delete usb;
  }
  void PS3Robot::calibrate()
  {
    /*Serial can be used for testing usb board configured correctly*/
    
   //Serial.begin(115200);
   if (usb->Init() == -1) 
   {
    //Serial.print("OSC did not start");
    while (1);
   }
   //Serial.print("PS3 USB Library Started");
  }
  bool PS3Robot::checkConnection()
  {
    /*The task function is used to clear data recorded from PS3 controller,
      the function worked more effectively when entered twice. */
    
    usb->Task();
    usb->Task();
            
    if(PS3->PS3Connected || PS3->PS3NavigationConnected)
      return true;
    else
      return false;
  }
  void PS3Robot::checkController(servoStruct *servo)
  {

    if(PS3->getAnalogHat(RightHatX) > 150 || PS3->getAnalogHat(RightHatX) < 100)
    {
      servo->servoNum = 1;
      servo->range = map(PS3->getAnalogHat(RightHatX), 0, 255, servoMin, servoMax);
      servo->isntbutton = false;
    }
    else if(PS3->getAnalogHat(RightHatY) > 135 || PS3->getAnalogHat(RightHatY) < 110)
    {
      servo->servoNum = 2;
      servo->range = map(PS3->getAnalogHat(RightHatY), 0, 255, servoMin, servoMax);
      servo->isntbutton = false;
    }
    else if(PS3->getAnalogHat(LeftHatY) > 130 || PS3->getAnalogHat(LeftHatY) < 120)
    {
      servo->servoNum = 3;
      servo->range = map(PS3->getAnalogHat(LeftHatY), 0, 255, servoMin, servoMax);
      servo->isntbutton = false;
    }
    else if(PS3->getButtonPress(L1))
    {
      servo->servoNum = 4;
      servo->buttonOn = true;
      servo->isForward = true;
      servo->isbutton = true;
    }
    else if(PS3->getButtonPress(R1))
    {
      servo->servoNum = 4;
      servo->buttonOn = true;
      servo->isForward = false;
      servo->isbutton = true;
    }
    else if(PS3->getButtonPress(CROSS))
    {
      Serial.begin(115200);
      Serial.println("X selected");
      servo->servoNum = 5;
      servo->buttonOn = true;
      servo->isForward = true;
      servo->isbutton = true;
    }
    else if(PS3->getButtonPress(TRIANGLE))
    {
     servo->servoNum = 5;
      servo->buttonOn = true;
      servo->isForward = false;
      servo->isbutton = true;
    }
    else if(PS3->getButtonPress(UP))
    {
      servo->servoNum = 6;
      servo->buttonOn = true;
      servo->isForward = true;
      servo->isbutton = true;
    }
    else if(PS3->getButtonPress(DOWN))
    {
      servo->servoNum = 6;
      servo->buttonOn = true;
      servo->isForward = false;
      servo->isbutton = true;
    }
    else
    {
      servo->servoNum = NULL;
    }
  }
  void PS3Robot::moveStructure(servoStruct &servo, Adafruit_PWMServoDriver &pwm)
  {
    if(servo.servoNum != NULL)
    {
      if(servo.isbutton)       
        buttonControlled(servo, pwm);
      else
        joystickControlled(servo, pwm);
         
    }     
  }
  void PS3Robot::buttonControlled(servoStruct &servo, Adafruit_PWMServoDriver &pwm)
  {

    if(servo.isForward)
      buttonForward(servo, pwm);
    else
      buttonBackward(servo, pwm);

  }
  servoStruct& PS3Robot::checkButton(servoStruct &servo)
  {
    usb->Task();
    usb->Task();
    
    if(servo.servoNum == 4)
    {
     if(PS3->getButtonPress(L1) || PS3->getButtonPress(R1))
     {
        servo.buttonOn = true;
        return servo;
     }
    }
    else if(servo.servoNum == 5)
    {
      if(PS3->getButtonPress(CROSS) || PS3->getButtonPress(TRIANGLE))
      {
        servo.buttonOn = true;
        return servo;
      }
          
    }
    else if(servo.servoNum == 6) 
    {
        if(PS3->getButtonPress(UP) || PS3->getButtonPress(DOWN))
        {
          servo.buttonOn = true;
          return servo;
        }
    }

    servo.buttonOn = false;
    return servo;
  }
  void PS3Robot::buttonForward(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(;servo.buttonOn; i += 15)
    {   
      if(i < servoMax)
        pwm.setPWM(servo.servoNum, 0, i);
      else
        pwm.setPWM(servo.servoNum, 0, servoMax);
        
      delay(setTime);
      servo = checkButton(servo);
   }
   //buttonForwardReset(servo, pwm, i);
 }
  void PS3Robot::buttonForwardReset(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(;i > midPoint; i -=15)
   {   
      pwm.setPWM(servo.servoNum, 0, i);
      delay(setTime);
      servo = checkButton(servo);
      if(servo.buttonOn)
        buttonForward(servo, pwm, i);
   }
  }
  void PS3Robot::buttonBackward(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(;servo.buttonOn; i -= 15)
    {         
      if(i > servoMin)
        pwm.setPWM(servo.servoNum, 0, i);
      else
        pwm.setPWM(servo.servoNum, 0, servoMin);
      delay(setTime);
      servo = checkButton(servo);
   }
   //buttonBackwardReset(servo, pwm, i);
 }
  void PS3Robot::buttonBackwardReset(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(;i < midPoint; i +=15)
   {   
      pwm.setPWM(servo.servoNum, 0, i);
      delay(setTime);
      servo = checkButton(servo);
      if(servo.buttonOn)
        buttonBackward(servo, pwm, i);
   }
  }
  void PS3Robot::joystickControlled(servoStruct &servo, Adafruit_PWMServoDriver &pwm)
  {
    if(servo.range > 475)
      joystickForward(servo, pwm);
    else
      joystickBackward(servo, pwm);
      
  }
  void PS3Robot::joystickForward(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(;joystickActive(servo); i += 10)
    //for(;i <=servo.range;i+=15)
    {
      /*if(i >=servoMax)
        pwm.setPWM(servo.servoNum, 0, servoMax);
      else*/
        pwm.setPWM(servo.servoNum, 0, i);
      delay(setTime);
    }
    //joystickForwardReset(servo, pwm, i);
  }
  void PS3Robot::joystickForwardReset(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(; i >= midPoint; i -= 15)
    {
      pwm.setPWM(servo.servoNum, 0, i);
      delay(setTime);
      if(joystickActive(servo))
        joystickForward(servo, pwm, i);
        
    }
  }
  void PS3Robot::joystickBackward(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(; joystickActive(servo); i -= 10)
    //for(; i >= servo.range; i-= 15)
    {
      /*if(i <=servoMin)
        pwm.setPWM(servo.servoNum, 0, servoMin);
      else*/
        pwm.setPWM(servo.servoNum, 0, i);
      delay(setTime);
    }
    //joystickBackwardReset(servo, pwm, i);
  }
  void PS3Robot::joystickBackwardReset(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i)
  {
    for(; i <= midPoint; i += 15)
    {
      pwm.setPWM(servo.servoNum, 0, i);
      delay(setTime);
      if(joystickActive(servo))
        joystickBackward(servo,pwm,i);
    }
  }
  bool PS3Robot::joystickActive(servoStruct &servo)
  {
      usb->Task();
      usb->Task();
      
      delay(setTime);
      if(servo.servoNum == 1)
      {
          if(PS3->getAnalogHat(RightHatX) > 160 || PS3->getAnalogHat(RightHatX) < 90)
          return true;
      }
      else if(servo.servoNum == 2)
      {
          if(PS3->getAnalogHat(RightHatY) > 135 || PS3->getAnalogHat(RightHatY) < 110)
          return true;
      }
      else if(servo.servoNum == 3)
      {
          if(PS3->getAnalogHat(LeftHatY) > 150 || PS3->getAnalogHat(LeftHatY) < 100)
          return true;
      }

      return false;
  }

#ifndef PS3Robot_h
#define PS3Robot_h

#include <Arduino.h>
#include <PS3USB.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "servoStruct.h"

class PS3Robot
{
  /*Servo range, mid points, and delays defined*/
  #define servoMin 100
  #define servoMax 800
  #define midPoint 250
  #define setTime 200

  /*Holds PS3 and USB objects for configuring*/
  PS3USB* PS3;
  USB* usb;
  
  void calibrate(); /*Calibrates USB board */
  void buttonControlled(servoStruct &s, Adafruit_PWMServoDriver &pwm); /*Divides move operations to move between forward or backwards */
  void joystickControlled(servoStruct &s, Adafruit_PWMServoDriver &pwm); /*Divides move operations to move between forward or backwards */
  void buttonForward(servoStruct &s, Adafruit_PWMServoDriver &pwm, int i = midPoint); 
  void buttonForwardReset(servoStruct &s, Adafruit_PWMServoDriver &pwm, int i); /*See Below. */
  void buttonBackward(servoStruct &s, Adafruit_PWMServoDriver &pwm, int i = midPoint);
  void buttonBackwardReset(servoStruct &s, Adafruit_PWMServoDriver &pwm, int i); /*See Below. */
  servoStruct &checkButton(servoStruct &s); /*Checks input is maintained (some details of this below). */
  void joystickForward(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i = midPoint);
  void joystickForwardReset(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i); /*See Below. */
  void joystickBackward(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i = midPoint);
  void joystickBackwardReset(servoStruct &servo, Adafruit_PWMServoDriver &pwm, int i); /*See Below. */
  bool joystickActive(servoStruct &servo); /*Checks input is maintained (some details of this below). */
  
  public:
  PS3Robot(PS3USB &PS3, USB &usb);
  ~PS3Robot();
  bool checkConnection(); /*Checks PS3 controller is connected */
  void checkController(servoStruct *s); /*Determines which servo to active, returns NULL if controller is not used. */
  void moveStructure(servoStruct &s, Adafruit_PWMServoDriver &pwm); /*Divides move operations depending on if a button or joystick was used. */
};

#endif

/*Below:
  Initially, it was best considered to have an operation to reset the servos back to a mid-point level (this being based of the operations of the original Arduino servo library).
  It was found however, that there are a few flaws with this operation (this operation doesn't reset the 'Arm's' movements and possibly places it in a particular awkward position,
  there can be use with leaving the arm in a set position, and overall speed of operation is reduced), and so it has not be implemented into the final code. It is however still included
  into the file for someone who may wish to make use of this operation, all that would be required is that the comments preventing the machine from reading these functions be removed.

  There are functions implemented to check if controller buttons or joysticks are still in use, and these functions were originally used to check when each individual servo needed reset.
  They are however still  in use to determine when to stop moving each servo. No changes need to be done to these operations to operate the reset functions.
 */

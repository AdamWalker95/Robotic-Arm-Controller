# Robotic-Arm-Controller
This is a project that makes use of a PS3 controller to operate a "robotic arm". The project is written to be intended for use with the 
Arduino microcontroller (in the prototype: the Arduino Uno microcontroller). The project has been written primarily in C++ and makes use of
the USB Host Shield 2.0 Library, as well as the Adafruit PWM Servo Driver Library (both of which will be required for operation of the project). Aside from that, a USB shield, six servos, a 16 channel servo module, and a frame are required for the build.

This is a completed verison of the system. An earlier prototype had previously been attempted to work without an I2C servo module, however due to limited power options the original prototype had went untested. This version will yield the same results but without the need for the same power requirements.

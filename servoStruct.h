
/*This struct is used to save what kind of motion is required from
  the arm. */
  
#ifndef servoStruct_h
#define servoStruct_h

struct servoStruct
{
  int servoNum;
  union
  {
    struct {bool isntbutton; double range;};
    struct {bool isbutton; bool isForward; bool buttonOn;};
  };
};

#endif

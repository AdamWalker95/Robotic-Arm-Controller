

struct servoStruct
{
  int servoNum;
  union
  {
    struct {bool isntbutton; double range;};
    struct {bool isbutton; bool isForward; bool buttonOn;};
  };
};

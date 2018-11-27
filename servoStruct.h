

struct servoStruct
{
  int servoNum;
  union
  {
    struct { int range; bool isntbutton;};
    struct { bool buttonOn; bool isbutton;};
  };
};

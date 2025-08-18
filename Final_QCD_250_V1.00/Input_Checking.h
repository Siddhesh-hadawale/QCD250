#ifndef Input_Checking_h
#define Input_Checking_h

#define level_Sensor 10
#define Flow_sensor 6         //flow sensor

class Inputchecksclass
{
  public:
  Inputchecksclass();
  void InputChecksSetup();
  void Input_errorcheck();
  void Level_sense();
  void Flow_Timer_Stop();
};

extern Inputchecksclass Input_checks;

#endif

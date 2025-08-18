#ifndef button_h
#define button_h

#define DOWN_KEY 7 
#define SET_KEY 11 
#define UP_KEY 8 
#define RESET_KEY 5 

class Buttonclass
{
  public:
  Buttonclass();
  void Butt_Setup();
  void calibration_steps();
  void button_update();
  void Emergency_Procees_Halt();
};

extern Buttonclass Butt;

#endif

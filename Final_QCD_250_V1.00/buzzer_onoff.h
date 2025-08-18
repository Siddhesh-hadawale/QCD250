#ifndef buzzer_onoff_h
#define buzzer_onoff_h

const int buzzer=23;

class Buzzerclass
{
  public:
  Buzzerclass();
  void buzzer_on_Setup();
  void buzzer_on(uint32_t interval1);
  void  Buzzer_updates();
  void Buzzer_Error();
};

extern Buzzerclass buz1;

#endif

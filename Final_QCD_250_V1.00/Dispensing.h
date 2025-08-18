#ifndef Dispensing_h
#define Dispensing_h

#define DISPENSE_KEY 12
#define peristalitic_pump 4

class Dispenseclass
{
  public:
  Dispenseclass();
  void disp_Setup();
  void ticker_stop();
  void Pump_ticker_Update();
  void button_update();
  void pump_calculations();
  void secondboil_stop_pump();
};

extern Dispenseclass Dispense;

#endif

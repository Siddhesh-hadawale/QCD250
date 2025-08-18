#ifndef display_h
#define display_h

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class Displayclass
{
  public:
  Displayclass();
  void Disp_Setup();
  void LCD();
  void pump_disp();
  void disp_menu(int a);
};

extern LiquidCrystal_I2C lcd;
extern Displayclass disp;

#endif

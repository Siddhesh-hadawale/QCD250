#include "arduino.h"
#include "Ext_Var.h"
#include "buzzer_onoff.h"

void buzzer_off();

Ticker buz(buzzer_off, 100, 1, MILLIS);

Buzzerclass::Buzzerclass()
{}

void Buzzerclass:: buzzer_on_Setup()
{
 pinMode(buzzer, OUTPUT);
}

void Buzzerclass:: buzzer_on(uint32_t interval1)
 {
   buz.interval(interval1);                     //set buzzer On time//have to add buz
   digitalWrite(buzzer, HIGH);
   buz.start();                                 //have to add buzzer 
 }
 
void buzzer_off()                               //Buzzer beep 
{
  digitalWrite(buzzer, LOW);                    //---off
  buz.stop();
}

void Buzzerclass::Buzzer_updates()
{
  buz.update();
}

Buzzerclass buz1 = Buzzerclass(); 

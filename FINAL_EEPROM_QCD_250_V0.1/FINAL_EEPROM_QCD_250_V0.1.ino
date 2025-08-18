/*
 * Edit : Siddhesh Hadawale
 * Project Description: QCD 250 Quartz Distillation.
 * Date : 19/11/2024
 * FINAL_EEPROM_QCD_250_V0.1
 */
 
#include <EEPROM.h>

const int buzzer=23;

void setup()
{
 pinMode(buzzer, OUTPUT);

 EEPROM.write(1, 65 >> 8);
 EEPROM.write(1 + 1, 65 & 0xFF);

 EEPROM.write(14, 55); //safe temp
 EEPROM.write(1, 65);  //pump

 digitalWrite(buzzer, HIGH);
 delay(1000);
 digitalWrite(buzzer, LOW);
}

void loop() 
{

}

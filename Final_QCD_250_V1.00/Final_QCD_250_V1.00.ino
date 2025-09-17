/*
 * Edit : Siddhesh Hadawale
 * Project Description: QCD 250 Quartz Distillation.
 * Date : 19/11/2024
 * Final_QCD_250_V1.00
 * Bug fixes and various improvements have been implemented in this code.
 */

#include "Arduino.h"
#include "Ext_Var.h"
#include "Ticker.h"
#include <EEPROM.h>

#define PERIOD_EXAMPLE_VALUE (0x00C2)   //...........................................................................Timer overflow value(12.5 msec timer)
uint8_t incr=0;
void Display1();
Ticker T1(Display1,250,0 ,MILLIS);//..................................................................................MICROS_MICROS

//.................................................................Dispensing....................................................................//

//New object creation
// Dispensing

void setup()
{ 
   Serial.begin(9600);
   disp.Disp_Setup();
   Pros.process_Setup();
   Input_checks.InputChecksSetup();
   buz1.buzzer_on_Setup();
   Temperature1.Temp_Setup();
   Butt.Butt_Setup();
   Dispense.disp_Setup(); 
   EEPROM1.Read_memory();
   T1.start();
   cli();
   
   /* enable overflow interrupt */
   TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

   /* set Normal mode */
   TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;
   
   /* disable event counting */
   TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
   
   /* set the period */
   TCA0.SINGLE.PER = PERIOD_EXAMPLE_VALUE;

   TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;/* set clock source (sys_clk/256) */
   
   sei();//allow 
} //..............................................................setup end.....................................................//


ISR(TCA0_OVF_vect)//...........................................................................................................Timer interrupt ISR
{
    incr++;    
    if (incr >= 80) // Only execute the rest every 80 counts
    
 {
    // Timer for Process1
    if (Process1 == 1) 
    {
        sec_Heater++;
        if (sec_Heater > 240) sec_Heater = 80;
    }

    // Safety and Filling Timer
    if (sfil == 1) sfil_on++;

    // Calibration Timer
    if (pcalib == 2 && Pump_calib == 1) calib_count++;

    // Heater Safety Timer
    if (Temp_Err || Heater_safety == 1) Heater_safety_timer++;
    incr = 0;
   }//........................................................end of incr........................................................................................//
 
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}//.........................................................end of timer.........................................................................................//

void loop() 
{
   T1.update(); 
   Dispense.Pump_ticker_Update();
   buz1.Buzzer_updates();
   Butt.button_update();
   Pros.process_start();            
   Input_checks.Level_sense();                                                        
   Temperature1.TempRead_Boilor();                             //for temperature1 reading
   Temperature1.TempRead_Boilor2();                            //for temperature2 reading
   Input_checks.Input_errorcheck();                            //flow sensor reading
   Pros.primary_check();
}

 void Display1()                                               //display updating
{
  disp.LCD();
}
  

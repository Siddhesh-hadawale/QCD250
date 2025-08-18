#include "arduino.h"
#include "Ext_Var.h"
#include "Process.h"

bool once1=0,Temp_Err=0,Flow_Err=0,boil2_var,sfil_comp=0,Process1,warning_generate=0,once_boil2,process_stop_variable=0 ;        //sfil_comp=1
uint8_t sec_Heater=0;
void Resume_t();

void Heaters_stop();
Ticker Heaters_stop1(Heaters_stop, 5000 ,MILLIS);

Processclass::Processclass()
{}

void Processclass::process_Setup()
{
   pinMode(solonoid,OUTPUT);
   pinMode(Heater1,OUTPUT);
   pinMode(Heater2,OUTPUT);
   pinMode(Contacter1,OUTPUT);
   pinMode(Contacter2,OUTPUT);
   pinMode(Red_LED,OUTPUT);                    
   pinMode(Green_LED,OUTPUT);                                                  
   digitalWrite(Heater1,LOW);
   digitalWrite(Heater2,LOW);
   digitalWrite(Red_LED,LOW);                                                  
   digitalWrite(Green_LED,LOW);  
}

void Processclass::process_start()
{     
   Heaters_stop1.update();  
   if(sfil_comp==0)                                                        //only for secondory boiler fill
   {
    boil2_var=0;
    sfil=1;
   }
   if((level_detect !=1)&&(Stop_cycle==1))                                 //turn on solonoide when level is not full and standby mode is not activated
   {
   digitalWrite(solonoid,HIGH);                                            //solonoide on 
   }
  
   if((level_detect !=1)&&(Flow_Err==0) && (Temp_Err==0)&&(Stop_cycle==1)) //change  level is less tha 2 liters  && (Lev_Err==0)
   {   
     Process1=1;                                                           //when process is started 
     if( sec_Heater>= 40)                                                  //start the heater after 60 sec    //when count reaches 6 seconds turn on
     {    
      process_stop_variable=0;                                             //when the level goes down update the Vfill to zero so that it should strt time from zero
      Resume_t();
     }    
   }
     
  else if((level_detect ==1))                                              // when level is full  
  {
   Flow_Err=0;
   Temp_Err=0;
   once1=0;                                                                //to start process again
   Process1=0;
   sec_Heater=0;
   digitalWrite(Red_LED,LOW);
   digitalWrite(solonoid,LOW);  
   digitalWrite(Contacter1,LOW);                                           //solonoide off
   Pros.process_stop();
   Level_var=1;  
  }  
}
void Processclass:: Resume_t()                                             //to resume the timer if any error arrived and start again when everything is clear
{  
  digitalWrite(Contacter1,HIGH); 
  if(((sfil==1)&&(sfil_comp==0))|| (thermopile==1))                        //when secondary fill on
  {
   level_detect=2;                                                         //no level detection till second boil filling
   boil2_var=1;
   Dispense.secondboil_stop_pump();                                        //added here for continuous check
   if(once_boil2==0)
   {    
   Dispense.secondboil_stop_pump();                                        //safety to stop dispensing when peristalitic pump starts changed
   sec_Heater=0;
   once_boil2=1;
   screen=0;
   }
   if(sec_Heater>45)
   {
   digitalWrite(Heater1,HIGH);
   digitalWrite(Heater2,LOW);
   }
   
   if(sfil_on>=300)                                                         //5 min increase the time as per boilor fill,boilor will stop after this time
   {
   once_boil2=0;
   Level_var=0;
   sfil_comp=1;
//   EEPROM.write(10,sfil_comp);                                              //when secondory filled completed
   sfil=0;
   sfil_on=0;
   Process1=0;                                                              //off the sencond boilor
   digitalWrite(Red_LED,LOW);                                               //active high
   digitalWrite(solonoid,LOW);   
   }
  }
  
else if((sfil==0)&&(sfil_comp==1))                                         //Working on normal mode
{
  if(sec_Heater>45)digitalWrite(Contacter2,HIGH);                          //Changing the timing of switching of heater  45 sec.
  if(sec_Heater>50)digitalWrite(Heater1,HIGH);                             //Changing the timing of switching of heater  50 sec.
  if(sec_Heater>55)digitalWrite(Heater2, HIGH);                            //ON                                          55 sec.
                   digitalWrite(Red_LED,HIGH);                             //Active high
  Level_var=0;                                                             //If needed need to removed it from her
}
}
//.................................................................................end of resume_t......................................................................................//
 
void Processclass:: primary_check()
{
  if((Flow_Err==1)||( Temp_Err ==1)&&(Process1==1))                        //if showing error in function stop unit
  { 
     sec_Heater=0;
     Pros.process_stop(); 
  }

   if((calib_Heater1>80) || (calib_Heater2>80))                            //only when relay fails should generate warning
   {
     digitalWrite(Contacter1,LOW);  
     sec_Heater=0;
     Temp_Err=0;
     Flow_Err=0;
     Pros.process_stop();                                                  //contacter low
     screen=3;
     warning_generate=1;  
   }
}

void Processclass::process_stop()
{    
  if(process_stop_variable==0)
  { 
    digitalWrite(Red_LED,LOW);
    digitalWrite(Heater2,LOW);
    Heaters_stop1.start();    
    process_stop_variable=1;
  }  
}

 void Heaters_stop()
 {
  digitalWrite(Heater1,LOW);
  digitalWrite(Contacter2,LOW);
  digitalWrite(Red_LED,LOW);
  Heaters_stop1.stop();
 }

Processclass Pros = Processclass();  

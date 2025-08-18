#include "arduino.h"
#include "Ext_Var.h"
#include "Input_Checking.h"

bool       level_det1,level_det2,flow_sens_timerstart=0;
uint8_t    level_detect=2,level_Inc=0;
int        Flow_Err_INC=0;
static int flowlcd_cler;

void Flow_delay1();
Ticker Flow_delay(Flow_delay1,1000, 0 ,MILLIS);                                //MICROS_MICROS

Inputchecksclass::Inputchecksclass()
{}

void Level_sense();

void Inputchecksclass:: InputChecksSetup()
{ 
   pinMode(Flow_sensor, INPUT);                                                //initially input
   pinMode(level_Sensor,INPUT_PULLUP);
}

void Flow_delay1()
{
 Flow_Err_INC++; 
}

void Inputchecksclass::Input_errorcheck()                                       //flow level temp check
{ 
  Flow_delay.update();
  
  if((digitalRead(Flow_sensor)==1) && (F_override==0) && (Process1==1))          //process only when system is working
   {   
    if(!flow_sens_timerstart){Flow_delay.start();flow_sens_timerstart=1;}
    
    if(Flow_Err_INC>=30 && Flow_Err_INC<240)                                    //30 sec. flow error comes & 240 sec. (4 min.) goto process standby mode
    {    
     Flow_Err=1;
     screen=3;                                                                  //error screen
     sec_Heater=0;
     flowlcd_cler=0; //..........................................................to clear the lcd when flow error arrives
    }
    if(Flow_Err_INC>240)   //this provision only for when flow error is continue for 10 min then solonoide turn off to save wastage of water and turn on after long press reset 
    {
      Butt.Emergency_Procees_Halt();//changed
    }
   }
   else if((digitalRead(Flow_sensor)==0) || (F_override==1) || (Process1==0))   //if flow override is yes
    {      
      Flow_Err_INC=0;
      flow_sens_timerstart=0;
      Input_checks.Flow_Timer_Stop();
      Flow_Err=0;
      
      if(flowlcd_cler==0)
      {
       screen=0;
      if(flowerror_screen_switch ==1)
      {
      screen=1; 
      }
      else 
      {
       screen=0;
      }
      flowlcd_cler=1;
      digitalWrite(Green_LED,LOW);
      digitalWrite(Red_LED,LOW);
      digitalWrite(buzzer,LOW);  
      }
   }
}

void Inputchecksclass::Flow_Timer_Stop()
{
  Flow_delay.stop();
}
void Inputchecksclass::Level_sense()
{
   level_det1=digitalRead(level_Sensor);
   
   if(L_override || level_det1)  //tank filling                                     //Normal operation
    {
    level_detect=2;                                                                 //To change level status 
    }
    
   else if(!L_override && !level_det1 && (sfil !=1))                                //Tank Full to check continuously for
     {  
      level_Inc++;
      if(level_Inc>150)level_Inc=0; 
      if(level_Inc>100)                                                             //delay factor
      {
      level_detect=1;                                                               //tank full 
      level_Inc=0;                                                                  //level delay increment variable 
      }     
     }

     ////level_detect=3....for future scope  when tank volume is low this value is use for no dispense when tank is low
}

Inputchecksclass Input_checks = Inputchecksclass(); 

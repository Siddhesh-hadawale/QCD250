#include "arduino.h"
#include "Ext_Var.h"

//..........................................................................Temp sensor.......................................................................//
float R0 = 100.0;                                        //Resistance of Temp_sense1 at 0 degree
float R1 = 4700.0;
float R2 = 100.0;
float R3 = 4700.0;
float Vin=0,Vin1=0,Rt=0;
const int Vs = 5.00;                                     //input vtg.
const float Alpha = 0.00385;
float temperature_read=0;
bool temp_safety_once=0; 
int Probe1_actual=0,Probe2_actual=0,Heater_safety_timer; //for probe detection
uint8_t Heater_safety=0;


float  instant_val=0;
float  instant_val1=0,Rt1;

float  Vout=0, TotalVout = 0,AvgVout = 0;
float  Heater_temp=0,calib_Heater1=0;

float  Vout1=0, TotalVout1 = 0,AvgVout1 = 0;
float  Heater_temp1=0,calib_Heater2=0,Heater_temp2=0;


Tempclass::Tempclass()
{}
void Tempclass:: Temp_Setup()
{
   pinMode(Temp_sense1,INPUT);
   pinMode(Temp_sense3,INPUT); 
}

void Tempclass:: TempRead_Boilor()
 { 
   static int sample = 0,templcd_cler;
   sample++;
   if(sample < 500)
   {
   float instant_val = analogRead(Temp_sense1);          //digital values
   Vout = ((instant_val * Vs) / 1023.0);                 //analog values
   TotalVout += Vout;
   }
   
  if(sample >= 500)
  {
  AvgVout = TotalVout / 500;
  Vin = AvgVout /10;                                     //gain divide
  Rt = (R2 * R3 + R3 * (R1 + R2) * Vin / Vs) / (R1 - (R1 + R2) * Vin / Vs);
  Heater_temp = ((Rt / R0) - 1) / (Alpha);
  calib_Heater1= Heater_temp+Probe1_actual;              //data will not changed unless it saves
  sample = 0;
  TotalVout = 0; AvgVout = 0; 
  }
  
  if((calib_Heater1>safe_temp)&&(calib_Heater1<120))     //when the boilor temperature is less than 70 degree should stop the process//commeneted for now
  { 
   if((Process1==1)&&(sec_Heater>=60))                   //only show the error when in process to save the heater from burning after 2 min because heater is hot and if the delay doesnt apply will start showing errot immidiatly
   {
    warning_generate=0;
    Temp_Err=1;
    templcd_cler=0;
    
    if(temp_safety_once==0){Heater_safety++;temp_safety_once=1;}
    screen=3;                                            //need to add screen here
    if(Heater_safety_timer<=420 && Heater_safety==2)
   {
    Heater_safety_timer=0;
    Butt.Emergency_Procees_Halt();                       //changed to save heater when no water and it giving warning again and again
    Heater_safety=0;
   }
   }
  }
  else
  {
    Temp_Err=0;
    if ((Heater_safety>=0 && Heater_safety<2 )&& (Heater_safety_timer>540)) //when no dry run and working on normal operations
    {
     Heater_safety=0;
     Heater_safety_timer=0;
    }
   
   if(templcd_cler==0)
    {
    screen=0;
    templcd_cler=1;
    temp_safety_once=0;
    digitalWrite(buzzer,LOW);   
    }
  }
 
 }//.......................................................Boilor temp sensor.....................................................................

void Tempclass:: TempRead_Boilor2()
 { 
   static int sample = 0;
   sample++;
   if(sample < 500)
   {
   float instant_val1 = analogRead(Temp_sense3);                                     //tempereture sense probe 2
   Vout1 = ((instant_val1 * Vs) / 1023.0);                                           //analog values
   TotalVout1 += Vout1;
   }
   
   if(sample >= 500)
   {
   AvgVout1 = TotalVout1/ 500;
   Vin1 = AvgVout1 /10;                                                              //gain divide
   Rt1 = (R2 * R3 + R3 * (R1 + R2) * Vin1 / Vs) / (R1 - (R1 + R2) * Vin1 / Vs);
   Heater_temp2 = ((Rt1 / R0) - 1) / (Alpha);
   calib_Heater2= Heater_temp2+ Probe2_actual;  
   sample = 0;
   TotalVout1 = 0; AvgVout1 = 0; 
    
   if(calib_Heater2>(safe_temp))//..........................After testing value of teperature sensor when get dry run can be change
   {
    screen=0;
    vol_set=0;
    warning_generate=0;
    uint8_t inc;
    inc++;
    if(inc>100){thermopile=1;inc=0;}                                                 //changed
//    digitalWrite(Red_LED,LOW);

    sfil=1;
    sfil_comp=0;                                                                     //variable to start and stop the proce
    sfil_on=0;                                                        
   }
   else
   thermopile=0; 
   }//.................................................................Sample end..............................................................................//

 }//...................................................................Boilor temp sensor.....................................................................//

Tempclass Temperature1 = Tempclass();  

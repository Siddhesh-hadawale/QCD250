#include "arduino.h"
#include "Ext_Var.h"
# include "Dispensing.h"

//............................................................................................Dispensing variables............................................

uint32_t DC=757,VOLUME=10,VOLUME1=0,Point_dispense=10,sfil_on=0;
uint8_t  count,tim_disp=0,screen=0,Pump1=65;
uint8_t  STEP=1;//...........................................................................to check the value till priming
bool     once=0,stop_pump=0;
unsigned long capcity1 = 0;
unsigned long startTime=0,currentTime,Endtime = 0;    //remove this feature 


OneButton button1(DISPENSE_KEY,  true);

void pump_on();
void pump_off();
void Dispense1();
void Long_Dispense1();
void Dispense_start();
void Long_Dispense();

Ticker pump_on1(pump_on,DC, 0 ,MILLIS);
Ticker pump_Wait(pump_off,(Disp_t)*1000, 0 ,MILLIS);

Dispenseclass::Dispenseclass()
{}

void Dispenseclass::disp_Setup()
{
  pinMode(DISPENSE_KEY, INPUT_PULLUP); 
  pinMode(peristalitic_pump,OUTPUT);
  button1.attachClick(Dispense_start);
  button1.attachLongPressStart(Long_Dispense);
}

void disp_count()                                                              //to dispense count down
{
  if(tim_disp==1)                                                              //for normal dispensing /* tim_disp // !tim_disp */
  {
  DC=(VOLUME*(uint32_t)Pump1);                                                 //dispense for 1 ML(68msec/Ml),so timer on for amount enter by user(value 68 is use as calibrated )
  }
  
  if(tim_disp==2)                                                              //for continuous dispensing
  {
   unsigned long Period=0;
   Period= Endtime-startTime;
   capcity1=Period/Pump1;
   once=0; 
  }
}

void Dispense_start() //3 functions including this  1)step dispence 2)stop dispence 3)calibration dispence
{ 
  lcd.clear();
  if(level_detect==3) //for future scope                                                                                               //start for low level detect
  {
    screen=2;
  }
 
  if((level_detect != 3))                                                 //.............for low level no dispense
  {
  if((Pump_calib==1) && (pcalib>=2)&& sfil_comp)                          //.............for pump calibration
  {      
    DC=(Point_dispense*(uint32_t)Pump1); 
    pump_on1.interval(DC);                                                //............DC means Dispence count in millis
    screen=2;                                                             //............Dispence screen
    tim_disp=5;                                                           //............Sub screen 5                                                          
    pump_Wait.interval((Disp_t)*1000);
    pump_Wait.start();
  }
  
  else if((tim_disp != 2) && (stop_pump==0)&&(Pump_calib !=1) && (sfil_comp==1) )        //to start pump
  {
    i=0;
    tim_disp=1;                                                                          //to start dispense one time
    count =0;
    screen=2;
    if(Stop_cycle) Level_var=0;                                                          //Changed for process standoff
    disp_count();
    pump_on1.interval(DC);
    pump_Wait.interval((Disp_t)*1000);                                                   //if dispense delay updated
    pump_Wait.start();
    stop_pump=1;
  }
  
  else if((tim_disp ==1)&&(stop_pump==1))                                               //to stop pump for single step dispense
  {
    stop_pump=0;
    screen=2;
    digitalWrite(peristalitic_pump,LOW);
    tim_disp=4;                                                                         //dispense finish
    buz1.buzzer_on(500);
    Dispense.ticker_stop();
  }
  
  else if(tim_disp==2)                                                                  //to stop dispensing long time
   {
    currentTime = millis();
    Endtime=currentTime;
    i=0;
    if(Stop_cycle) Level_var=0;                                                         //Changed for process standoff
    screen=2;
    digitalWrite(peristalitic_pump,LOW);
    tim_disp=3; 
    disp_count();                                                                            //dispense complet
    buz1.buzzer_on(1000);  
   }
   
  }//...........................................................................end of no dispense....................................................................................
}

void Dispenseclass::secondboil_stop_pump()
{
  stop_pump=0;
  tim_disp=3;
  Dispense.ticker_stop();
  digitalWrite(peristalitic_pump,LOW);
  screen=0;
}

void Long_Dispense()
{
  if((sfil_comp==1))
  {
  tim_disp=2;//.....................................................................................................variable for long dispense
  screen=2;
  currentTime = millis();
  if(once==0)
    {
     startTime=currentTime;
     once=1;
    }
  digitalWrite(peristalitic_pump,HIGH);
  }
}

void pump_on()
{  
   if((Pump_calib==1)&&(sfil_comp==1))//changed
   {
   digitalWrite(peristalitic_pump,LOW); 
   pcalib=15;                                              //directly jump to error volume screen
   screen=1;  
   flip=1; 
   pump_Wait.stop();
   pump_on1.stop();
}

else if((Pump_calib !=1)&&(sfil_comp==1))
{
    digitalWrite(peristalitic_pump,LOW);
    if(count >= STEP)
    {
    tim_disp=4;//...............................................................................................................................dispense finish//
    buz1.buzzer_on(500);
    pump_Wait.stop();
    stop_pump=0; 
    } 
    else 
    {
      pump_Wait.interval((Disp_t)*1000);  
      pump_Wait.start();
    }                                          //if dispense delay updated  
    pump_on1.stop();
}
 } 

void Dispenseclass:: ticker_stop()
{
    pump_Wait.stop();
    pump_on1.stop(); 
}

void Dispenseclass:: pump_calculations() //...................................................................................calculations for pump
{
    float f1,f2,f3,f4,f5,f6;
    f1=Point_dispense;//......................................................................................................conversion to float
    f2=(Point_dispense+Pump_error);
    f3=(Pump1*(f1/f2));
    Pump1=round(f3);
}

void pump_off()
{ 
   if((Pump_calib==1)&&(sfil_comp==1))//changed
{  
   digitalWrite(peristalitic_pump,HIGH); 
   pump_on1.start();
   pump_Wait.stop();
}
   else if((Pump_calib !=1)&&(sfil_comp==1))
 {
   digitalWrite(peristalitic_pump,HIGH);
   count++;
   VOLUME1=(count*VOLUME);
   pump_on1.start();
   pump_Wait.stop();
 }
}

void Dispenseclass ::Pump_ticker_Update()
{
    button1.tick();
    pump_on1.update();
    pump_Wait.update();
}
 
void Long_Dispense1()
 { 
    if((sfil_comp==1)&&(Pump_calib !=1))//changed only when secondary fill off and not in calibration
    {
    dispense=1;
    digitalWrite(peristalitic_pump,HIGH);
    }
 }

Dispenseclass Dispense = Dispenseclass(); 

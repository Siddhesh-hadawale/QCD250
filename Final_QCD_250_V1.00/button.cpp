#include <Esplora.h>

#include "arduino.h"
#include "Ext_Var.h"

unsigned long start_tt;
int menu_select=0, count_inc=0,Pump_error=0;
int Probe1=0,Probe2=0;
uint8_t vol_set=0,back=0,Disp_t=2,safe_temp=55,safe_temp1=55,step1=0;   //step1 variable is for step by step execution of pump calibration
uint8_t pcalib=0;                                                       //variable for pump calibration
uint8_t emer_start_stop=0,pump_calib_inc;
bool    lock=0,but1=0,flip=0,F_override=0,Sign1,Sign2,Flow_override,L_override=0,dispense=0,Sfill=1,Stop_cycle=1;
bool    flowerror_screen_switch=0; //Flow error flag display stucking problem solve


OneButton button2(SET_KEY,    true);
OneButton button3(UP_KEY,     true);
OneButton button4(DOWN_KEY,   true); 
OneButton button5(RESET_KEY,  true); 

void LongPress5sec_fun();
int inc_time = 0;
int chk_time = 0;
Ticker LongPress5sec(LongPress5sec_fun, 1000, 0);                       //for changing long press timing,set for 5 sec

void LongPress5sec_fun()
{
   if(chk_time == 1)
  {
    inc_time ++;
  }
}

void singleUPclick1();
void longpres_Up1();
void longpres_Down1();
void SET1();
void longpress1();
void singleDownclick1();
void RESET1();
void RESET_LONG();
void longPressStart1();
void RESET_LONG_stop();

Buttonclass::Buttonclass()
{}

void Buttonclass :: Butt_Setup()
{ 
  pinMode(UP_KEY,   INPUT_PULLUP); 
  pinMode(DOWN_KEY, INPUT_PULLUP); 
  pinMode(SET_KEY,  INPUT_PULLUP); 
  pinMode(RESET_KEY,INPUT_PULLUP); 

  button2.attachClick(SET1);
  button2.attachLongPressStart(longpress1);
//  button2.attachDuringLongPress(longpress1);
  
  button3.attachClick(singleUPclick1);
  button3.attachDuringLongPress(longpres_Up1);

  button4.attachClick(singleDownclick1);
  button4.attachDuringLongPress(longpres_Down1);
  
  button5.attachClick(RESET1);  
  button5.attachLongPressStart(longPressStart1); 
  button5.attachDuringLongPress(RESET_LONG);                               //RESET_LONG_stop
  button5.attachLongPressStop(RESET_LONG_stop);
  LongPress5sec.start();
}

void Buttonclass:: button_update()
{
  button2.tick();
  button3.tick();
  button4.tick();
  button5.tick();
  LongPress5sec.update();
}

void singleUPclick1()
{ 
  if(screen == 1)  //............................................................enter into menu screen traversing
   {  
   if(back==0)     //............................................................Menu display 
   {   
    menu_select==0? menu_select=7: menu_select--;
   }
   
   if(back==1)  //...............................................................Increment and Decrement values
      {
      switch (menu_select)
      {
         case 0: //..............................................................secondory fill

         break;

         case 1:  //.............................................................safety temp
         safe_temp1>=75? safe_temp1=50: safe_temp1 +=5;
         break;

         case 2://...............................................................Probe1 Calibration
         Probe1>19? Probe1=-20: Probe1+=1;
         break;

         case 3://...............................................................Probe1 Calibration
         Probe2>19? Probe2=-20: Probe2+=1;        
         break;
         
         case 4://...............................................................Pump Calibration
         Pump_calib=1;
         break;

         case 5:
         Flow_override=!Flow_override;//.........................................flow override(No) 
         break;

         case 6:
         Disp_t>29?  Disp_t=1:  Disp_t+=1;//....................................dispense time
         break; 

         case 7:  //........................................................... tank filling
         L_override=! L_override;
         break;
        }   
      }
//..................................................................pump up section....................................................................//
        if((Pump_calib==1))
        {
        if((pcalib>=3)&&(pcalib<15)&&(flip==0))
        {
          Point_dispense>=1000?Point_dispense=10:Point_dispense+=10;
        }
        if((pcalib==15)&&(flip==1))
        {      
          Pump_error>=300? Pump_error=-300: Pump_error+=1; 
        }
      }
 }
 //..................................................................end of screen1....................................................................//
 
    if(screen==0) //home screen
     {
     if(vol_set==1 && screen!=1)
       {
        VOLUME<10000?VOLUME+=10:VOLUME=10;//increase  by 10 ml
       }
    if(vol_set==2 && screen!=1)//repeate dispensing setting
       {
       STEP<10? STEP+=1: STEP=1;
       }
  }//..................................................................end of screen 0.................................................................//
}  

 void singleDownclick1()
 { 
       if(screen==1)
        {
       digitalWrite(Green_LED,HIGH);
       if(back==0)//menu traverse
        {          
          menu_select==7? menu_select=0: menu_select++; 
        }
   
       if(back==1)//...............................................Increment and Decrement values
        {
        switch (menu_select)
        {
        case 0: //.................................................secondory fill

        break;

        case 1:  //safety temp
        safe_temp1==50? safe_temp1=75: safe_temp1-=5;//............for maximum safe temp to which temp error should not generated
        break;

        case 2:
        Probe1==-20? Probe1=20: Probe1-=1;//.......................temprature calibration upto +- 10
        break;

        case 3:
        Probe2==-20? Probe2=20: Probe2-=1;//.......................temprature calibration upto +- 10
        break;
        
        case 4://..................................................Pump Calibration
        Pump_calib=1;
        break;

        case 5://............................................. ... Flow Override
        Flow_override=!Flow_override; 
        break;

        case 6://................................................. Dispense wait time
        Disp_t==1?  Disp_t=30:  Disp_t-=1;
        break; 

        case 7://..................................................level override
        L_override = ! L_override; //Yes
        break;
      
     }   
  }
//................................................................for pump calib...........................................................................//
  }
   if((Pump_calib==1))
   { 
        if((pcalib>=3)&&(pcalib<15)&&(flip==0))
        {
          Point_dispense<=10?Point_dispense=1000:Point_dispense-=10;
        }
        if((pcalib==15) &&(flip==1))
        {
        Pump_error<=-300? Pump_error=300: Pump_error-=1; 
        }
   }//.............................................................end of pump down..........................................................................//
   
  if(screen==0)//.................................................screen 0
  {
  if(vol_set==1)//................................................to dispense volume only when in screen 0
  {
    VOLUME<11?VOLUME=10000:VOLUME-=10; //10
  }
  if(vol_set==2)//................................................volume dispense steping
  {
     STEP<=1? STEP=10: STEP-=1;
  }
  }//..............................................................end screen 0................................................................................//
}

void longpress1()//to enter into menu
{               
   if(stop_pump) //New updated changes 
   {
     screen=2; 
   }
   
   else if(!sfil_comp)
   {
    screen=0;
    digitalWrite(Green_LED,LOW);
   }
      
   else if(screen==0 && emer_start_stop==0)
  {
   flowerror_screen_switch=1;
   screen=1;                   //.....................................................for menu setting
   vol_set=3;
   back=0; 
   menu_select=0;       
   EEPROM1.Read_memory();
   Probe1=Probe1_actual;
   Probe2=Probe2_actual;
   safe_temp1=safe_temp;
   buz1.buzzer_on(500);
   digitalWrite(Green_LED,HIGH);
  }
} 
    
void longpres_Up1()   //to increase the volume by 10 rapidly
{
  if(screen==0)
   {
  count_inc++;
   if(count_inc >=(50))
    {
    VOLUME+=10;
    count_inc=0; 
    }
   if(VOLUME>10000)
    {
    VOLUME=10;          
    }
   }
//...............................................fast increment for pump calib......................................................................................//
 if((Pump_calib==1))
   { 
        if((pcalib>=3)&&(pcalib<15)&&(flip==0))
        {

         count_inc++;
         if(count_inc==(80))
         {
         Point_dispense+=10;
         count_inc=0; 
         }
         if(Point_dispense>=1000)
         {
         Point_dispense=10;          
         }   
        }
        if((pcalib==15) &&(flip==1))
        {
           count_inc++;
           if(count_inc==(80))
           {
           Pump_error+=1;
           count_inc=0; 
           }
           if(Pump_error>=300)
           {
           Pump_error=-300;          
           }    
        }
   }
}
        
void longpres_Down1()//to decrease the volume by 10 rapidly
{ 
  if(screen==0)
      {
      count_inc++;
      if(count_inc >= (50))
      {
      VOLUME-=10;//...............................................incremented by 10
      count_inc=0; 
      }
      if( VOLUME<9)
      {
      VOLUME=10000;          
      }
    }

//...............................................fast Decrement for pump calib.............................................//
 if((Pump_calib==1)) 
   { 
        if((pcalib>=3)&&(pcalib<15)&&(flip==0))
        {

         count_inc++;
         if(count_inc==(80))
         {
         Point_dispense-=10;
         count_inc=0; 
         }
         if(Point_dispense<=10)
         {
         Point_dispense=1000;          
         }   
        }
        if((pcalib==15) &&(flip==1))
        {
           count_inc++;
           if(count_inc==(80))
           {
           Pump_error-=1;
           count_inc=0; 
           }
           if(Pump_error<=-300)
           {
           Pump_error=300;          
           }    
        }
   }
}
 
 void SET1() 
{ 
  if(screen==0)
  {
    if(sfil_comp==1)
    {
         vol_set++;                                       //vol_set=1...volume enter
         if(vol_set>2)
         {
         vol_set=0;
         }   
    }       
  }
         
  if (screen==1)//..........................................................After menu Selection process set
  { 
    back++;
    digitalWrite(Green_LED,HIGH);
    if(back == 2) // problem detect back==2
    { 
     switch (menu_select)
     { 
      case 0 :                                          
      if(Sfill==1)
      {         
      sfil=1;      //.......................................................Start process for secondary biler filling 
      sfil_comp=0; //.......................................................variable to start and stop the proces
      sfil_on=0;
      boil2_var=1;
      sec_Heater=0;//.......................................................to turn on heater
//      EEPROM.write(10,sfil_comp); //........................................when secondory filled completed
      back =0;
      vol_set=0;
      screen=0;     
      }
      break;
         
      case 1:  //..........................................................safety temp
      safe_temp=safe_temp1;     
      EEPROM1.Write_memory();
      back = 0;
      break;
  
      case 2:
      Probe1_actual=Probe1;  
      EEPROM1.Write_memory(); 
      back = 0;
      break;

      case 3:
      Probe2_actual=Probe2; 
      EEPROM1.Write_memory();   
      back = 0;
      break;
      
      case 4:

      break;
      
      case 5:
      F_override=Flow_override;                            //flow overide
      back =0;
      break;

      case 6:
      back =0; //go to home screen 
      break;
           
      case 7:
      back =0;
      break;
           
      }
      //....................................................................end of switch.....................................................................//
    }
     if( menu_select==4)                                  //only if pump calibration
     {
     Butt.calibration_steps(); 
     }
 }
}                                                         //end set function

void Buttonclass ::calibration_steps()
{
    if(pcalib==0)
    {
    Pump_calib=1;
    flip=0;
    }
    pcalib++;
    if(pcalib==15) flip=1;
    if( pcalib==2)
    {
    calib_count=0;                                       //to start priming counter
    }
    else if(pcalib>=3 && pcalib<15)                      //for false press of set key
    {
    pcalib=3; 
    flip=0;
    }
    else if(pcalib==16)                                  //extra safety
    {     
    Dispense.pump_calculations();
    EEPROM1.Write_memory();
    back=0;
    Pump_calib=0;
    pcalib=0;
    }           
}

void RESET_LONG() 
{ 
  if(inc_time >= 3 && chk_time == 1)                    //3 sec.
  {    
    if(emer_start_stop==0)                              //to stop the process again after 5 sec
    {
     Butt.Emergency_Procees_Halt();
    }
    
    if(emer_start_stop==1)                              //to start the process again after 5 sec
    {
      buz1.buzzer_on(1000);
      Flow_Err=1;
      sec_Heater=0;
      Level_var=0; 
      Stop_cycle=1;
      Input_checks.Flow_Timer_Stop();
    } 
  }
}
void Buttonclass ::Emergency_Procees_Halt()               //stand by mode
{
  buz1.buzzer_on(1000);
  digitalWrite(Red_LED,LOW);
  digitalWrite(Green_LED,LOW); 
  digitalWrite(solonoid,LOW);
  digitalWrite(Contacter1,LOW);
  digitalWrite(peristalitic_pump,LOW);
  sec_Heater=0;
  Level_var=2;                                            //process stand by mode
  Stop_cycle=0;
  Pros.process_stop();
  screen=0;
  vol_set=0;
  Process1=0;                                             //to stop the process

  Dispense.ticker_stop(); 
  Flow_Err=0;
  Temp_Err=0;
  sfil_comp=1;                                            //when secondory filled completed
  sfil=0;
  sfil_on=0;
//  EEPROM.write(10,sfil_comp);                             //when secondory filled completed
  once1=0;                                                //to start process again  
}

void RESET_LONG_stop() 
{
  inc_time = 0;
  chk_time =0;
  emer_start_stop++;
  if(emer_start_stop>=2)emer_start_stop=0;
}

void longPressStart1()                                    //when one presses long press key
{
  chk_time =1;
}

void RESET1()                                             //single press back button
  {
  if (screen==2)                                          //Back press stops the pump
  {
    digitalWrite(peristalitic_pump,LOW);
    stop_pump=0;
    Dispense.ticker_stop(); 
  }
  if(screen==1)                                           //Menu screen
  { 
   flowerror_screen_switch=0;
  if((back==0) && (Pump_calib !=1))                       //menu to home screen
  {
  screen=0;
  tim_disp=0;
  vol_set=0;
  menu_select=0;
  digitalWrite(Green_LED,LOW);  
  }         
  else if(back==1)
  {
  back=0;
  
  switch (menu_select)
  {
  case 0: //............................................................secondory fill

  break;

  case 1:  //.............................................................safety temp
  EEPROM1.Read_memory();
  safe_temp1=safe_temp; //...............................................for maximum safe temp to which temp error should not generated
  break;

  case 2:
  Probe1=Probe1_actual;//.................................................temprature calibration upto +- 10
  EEPROM1.Read_memory();
  break;

  case 3:
  Probe2=Probe2_actual;
  EEPROM1.Read_memory();                                                                                    //..........................................................temprature calibration upto +- 10
  break;
        
  case 4:   
  EEPROM1.Read_memory();  
  pcalib=0;
  Pump_calib=0;
  break;

  case 5:
  Flow_override=0;
  break;

  case 6:
               //..........................................................Dispense wait default
  break; 
         
  case 7:      //..........................................................level override no
  L_override=0;
  break;
      
      
  } //................................................................end of switch............................................................................//       
 }//..................................................................end of back 1............................................................................//
 }//..................................................................end of screen 1..........................................................................//
 
 else //..............................................................back from any screen
 if(Flow_Err!=1 && Temp_Err!=1 && warning_generate!=1)screen=0;   
 if(Stop_cycle==0)Level_var=2;//changed
// digitalWrite(Green_LED,LOW);                 
 vol_set=0;      
 if(menu_select==4)//.................................................only for pump calibration
 {
 Pump_calib=0;//......................................................make low that calibration is completed
 pcalib=0;//..........................................................to start calibration again
 back=0; 
 }      
}   

Buttonclass Butt = Buttonclass();  

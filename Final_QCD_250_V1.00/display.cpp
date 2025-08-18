#include "Arduino.h"
#include "Ext_Var.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // ...................................................Set the LCD address to 0x27 

byte calib_count=6;//....................................................................Pump calibraton increment var
bool sfil=0,Sfil=0,Pump_calib=0,Once1,thermopile=0,clear1=0,temp_clear=0;
uint8_t i=0,check=0,Level_var=0;
uint8_t Probe_blink=0;
const char* YN[2]={"NO","YES"};
const String Level_status[3]={"  TANK FILLING  ","   TANK FULL    ","PROCESS STANDBY "};
const String boilor2_status[2]={"FILLING BOILER 2","FILLING BOILER 2"};
const String screens[8]={"> BOIL 2 FILL   ", 
                         "> SAFETY TEMP   ",
                         "> CALIB PROBE 1 ",
                         "> CALIB PROBE 2 ",
                         "> CALIB PUMP    ",
                         "> FLOW OVERRIDE ",
                         "> DISPENSE WAIT ",
                         "> LEVEL OVERRIDE"
                        };
                                
Displayclass::Displayclass()
{}

void Displayclass :: Disp_Setup()
      {
       Wire.begin();              // Initialize the I2C bus
       Wire.setClock(100000);     // Set I2C clock speed to 100 kHz (standard speed)
       lcd.init();                // Initialize the LCD
       lcd.clear();               // Clear the LCD display
       lcd.backlight();           // Turn on the LCD backlight
       lcd.begin(16,2);           // Set up the LCD size (16 columns, 2 rows)
       lcd.noAutoscroll();        // Disable auto-scrolling
             
       buz1.buzzer_on(1000);
       lcd.setCursor(0, 0); lcd.print("LabQuest Borosil");   
       lcd.setCursor(0, 1); lcd.print("   QCD V 1.00   ");
       delay(1500); 
      }

void Displayclass :: disp_menu(int a)
     {
       lcd.setCursor(0, 0); lcd.print(screens[a]);
       lcd.setCursor(0, 1); lcd.print(a == 7 ? screens[0] : screens[a+1]); //Ternary Operator condition ? value_if_true : value_if_false; i)If a is equal to 7, the code prints screens[0].
       lcd.setCursor(0, 1); lcd.print(" ");                                                                                            //ii)If a is not equal to 7, the code prints screens[a+1].
     }
     
void Displayclass :: LCD()
{
//...........................................................start of LCD display...............................................................................//
      if(screen==2)//........................................dispensing....................................................
      {   
      if((tim_disp==5) && (Pump_calib==1)&&(pcalib>=2))//........only for pump calibration
      {
      lcd.setCursor(0,1);  lcd.print("DISPENSING...   ");
      lcd.setCursor(0, 0); lcd.print("VOLUME:         ");
      lcd.setCursor(8,0);  lcd.print(Point_dispense);  
      }
      
      else if((tim_disp==1) && (level_detect!=3))//..............single step dispense
      { 
      lcd.setCursor(0, 0); lcd.print("VOLUME:         ");
      lcd.setCursor(8,0);  lcd.print(VOLUME);
      lcd.setCursor(0,1);  lcd.print("DISPENSE STEP:  ");
      lcd.setCursor(15,1); lcd.print(count);
      }
          
      else if((tim_disp==2) && (level_detect!=3))//..............long/continuos dispense
      {
      lcd.setCursor(0, 0);  lcd.print("                ");
      lcd.setCursor(0,1);   lcd.print("DISPENSING...   ");
      }
      
      else if( tim_disp==4)//....................................single step dispensing/after completion
      {
      if(i<20)
      {
      lcd.setCursor(0,0);  lcd.print("       ");
      lcd.setCursor(0,0);  lcd.print(VOLUME1);
      lcd.setCursor(6,0);  lcd.print("mL  ");
      lcd.setCursor(9,0);  lcd.print("STEP:");
      lcd.setCursor(14,0); lcd.print(count);//step
      lcd.setCursor(0,1);  lcd.print("DISPENSE COMPLETE"); 
      i++;
      }
      else
      {
      screen=0; 
      }    
      }//......................................................end of tim_disp 4 i.e finish dispense.....................................................................//
      

      else if(tim_disp==3)                                     //for long dispense/capacity
      {
      if(i<20)
      {
      lcd.setCursor(0,0);  lcd.print("   DISPENSING   ");
      lcd.setCursor(0,1);  lcd.print("   COMPLETED    "); 
      i++;
      }
      else
      {
      screen=0;  
      }
      }//......................................................end of tim_disp 3 i.e finish long dispense.................................................................//
      
      }//.......................................................end of screen 2...........................................................................................//
      
      //.............................................................start of screen 0....................................................................................// 
        
      if(screen==0 )                                    //start screen show level,temp,volum and step
      {
      if(vol_set==0)
      {          
      lcd.setCursor(0, 0);  lcd.print("BOILER TEMP:");
      lcd.setCursor(12,0);  lcd.print(calib_Heater1);
//.......................................................................Commented.................................................................
              if(sfil_comp==1)                                           // Not fill secondary boiler 
              {
               if(Stop_cycle==0)Level_var=2;
               lcd.setCursor(0, 1); lcd.print(Level_status[Level_var]);   //Level status displays
               digitalWrite(Green_LED,LOW); 
              }

      else if((sfil_comp==0) && (Stop_cycle==1))
      {
      Probe_blink++;
      digitalWrite(Red_LED,HIGH);       
      if (Probe_blink >= 5 && Probe_blink <= 6)
      {
      lcd.setCursor(0, 1);  lcd.print("                ");
      Probe_blink=0;
      digitalWrite(Red_LED,LOW); 
      digitalWrite(Green_LED,LOW);  
      }
      
      else  if (Probe_blink >= 1 && Probe_blink <= 4)
      {
      lcd.setCursor(0, 1);  lcd.print(boilor2_status[boil2_var]); //Boilor status displays
      }
      } 
      }
      
      else if((vol_set==1))//...............................................set volume...................................................................//
      {  
      lcd.setCursor(0,0);  lcd.print("VOLUME:         ");
      lcd.setCursor(12,0); lcd.print("mL  ");
      Probe_blink++;
      
      if (Probe_blink >= 5 && Probe_blink <= 6)
      {
      lcd.setCursor(7, 0);  lcd.print("       ");
      Probe_blink=0;
      }
      
      else  if (Probe_blink >= 1 && Probe_blink <= 4)
      {
      lcd.setCursor(7,0);  lcd.print(VOLUME);
      }
      lcd.setCursor(0,1);  lcd.print("DISP STEP:      ");
      lcd.setCursor(10,1); lcd.print(STEP);
      }
      
      else if((vol_set==2))//.....................................set step..................................................................................//
      {  
           lcd.setCursor(0,0);  lcd.print("VOLUME:     mL  ");
           lcd.setCursor(7,0);  lcd.print(VOLUME);
           lcd.setCursor(0,1);  lcd.print("DISP STEP:");
           lcd.setCursor(10,1);  lcd.print("           ");
           Probe_blink++;
           
           if (Probe_blink >= 5 && Probe_blink <= 6)
           {
           lcd.setCursor(10, 1);  lcd.print("          ");
           Probe_blink=0;
           }
           else  if (Probe_blink >= 1 && Probe_blink <= 4)
           {
           lcd.setCursor(10,1); lcd.print(STEP);
           }
         }
      }//......................................................................end of screen 0....................................................................//
      
       //......................................................................start of screen 1..................................................................//   
    if(screen==1)                                           //its shows menu
     {
      if(back==0)
      {
      disp_menu(menu_select);
      } 
      else if(back==1)
      {
      switch(menu_select)
      {  
      case 0://.................................................................................................secondory boilor filling
      lcd.setCursor(0, 0);  lcd.print("BOILER 2 Fill   ");
      lcd.setCursor(0, 1);  lcd.print("> ");
      lcd.setCursor(2, 1);  lcd.print("START           ");
      break;
       
      case 1://.................................................................................................Safe temperature
      lcd.setCursor(0, 0);  lcd.print("SAFETY TEMP     "); //enter safty temperature
      lcd.setCursor(0, 1);  lcd.print("MAX TEMP:       ");
      Probe_blink++;
      
      if (Probe_blink >= 5 && Probe_blink <= 6)
        {
          lcd.setCursor(9, 1);
          lcd.print("      ");
          Probe_blink=0;
        }
        else  if (Probe_blink >= 1 && Probe_blink <= 4)
        {
           lcd.setCursor(9, 1);lcd.print(safe_temp1); 
           lcd.setCursor(11, 1);lcd.print("   "); 
        }
      break;

      case 2://................................................................................................probe calibration
      
      EEPROM1.Read_memory();
      Pros.process_stop();                              //process stop till calibration
      sec_Heater=0;
      lcd.setCursor(0, 0);  lcd.print("PROBE1:");
      lcd.setCursor(7, 0);  lcd.print(Heater_temp);
      lcd.setCursor(11, 0); lcd.print(" ");
      lcd.setCursor(12, 0); lcd.print(calib_Heater1);
      lcd.setCursor(0, 1);  lcd.print("ERROR :        ");
      Probe_blink++;
      
      if (Probe_blink >= 5 && Probe_blink <= 6)
      {
          lcd.setCursor(8, 1);
          lcd.print("                ");
          Probe_blink=0;
      }
      else  if (Probe_blink >= 1 && Probe_blink <= 4)
      { 
         lcd.setCursor(8, 1);lcd.print(Probe1); 
         lcd.setCursor(11, 1);lcd.print("                "); 
      }
      break;

      case 3://................................................................................................probe calibration
       
      EEPROM1.Read_memory();
      Pros.process_stop();                            //process stop till calibration
      sec_Heater=0;
      lcd.setCursor(0, 0);  lcd.print("PROBE2:");
      lcd.setCursor(7, 0);  lcd.print(Heater_temp2);
      lcd.setCursor(11, 0); lcd.print(" ");
      lcd.setCursor(12, 0); lcd.print(calib_Heater2);
      lcd.setCursor(0, 1);  lcd.print("ERROR :         ");
      Probe_blink++;
      
      if (Probe_blink >= 5 && Probe_blink <= 6)
      {
          lcd.setCursor(8, 1);
          lcd.print("                ");
          Probe_blink=0;
      }
      else  if (Probe_blink >= 1 && Probe_blink <= 4)
      { 
         lcd.setCursor(8, 1);lcd.print(Probe2); 
         lcd.setCursor(11, 1);lcd.print("                ");
      }
      break;

      case 4://....................................................................................................pump calibration
      pump_disp();
      break;

      case 5:///...................................................................................................flowoverride
      lcd.setCursor(0, 0);  lcd.print("Flow Override   ");
      lcd.setCursor(0, 1);  lcd.print(">        ");
      lcd.setCursor(2, 1);  lcd.print(YN[Flow_override]);   
      lcd.setCursor(5, 1);  lcd.print("                ");  
      break; 

      case 6:
      lcd.setCursor(0, 0);  lcd.print("DISP WAIT TIME  ");
      lcd.setCursor(0, 1);  lcd.print("TIME:      SEC  ");
      Probe_blink++;
      
      if (Probe_blink >= 5 && Probe_blink <= 6)
      {
          lcd.setCursor(6, 1);
          lcd.print("    ");
          Probe_blink=0;
      }
      else  if (Probe_blink >= 1 && Probe_blink <= 4)
      {
          lcd.setCursor(6, 1);lcd.print(Disp_t); 
      }
      break;

      case 7://...................................................................................................................Level override
      lcd.setCursor(0, 0);  lcd.print("Level Override  ");
      lcd.setCursor(0, 1);  lcd.print(">       ");
      lcd.setCursor(2, 1);  lcd.print(YN[L_override]);
      lcd.setCursor(5, 1);  lcd.print("                ");
      break;
         
     }//........................................................................end switch case...................................................................//

  }//end if
      
     else if((back >=2 && back<17 )&& menu_select==4)//.....................this loop is to ensure the user can press set switch again it will not affect the operation
     {
      pump_disp();
     }
 
    }// 1 end
//............................................................................Error Display screen................................................................//
     
     if(screen==3)// this screen is for errors display
     {  
      if( Temp_Err==1)//..........................................................................for temperature error display for primary heater
       {
        Probe_blink++;
        digitalWrite(buzzer,LOW); 
        digitalWrite(Red_LED,HIGH);
        if (Probe_blink >= 6 && Probe_blink <= 7)
        {
        lcd.setCursor(0, 0);  lcd.print("                ");      //.............................only when temp raised and water is not preset
        lcd.setCursor(0, 1);  lcd.print("                ");
        Probe_blink=0;
        digitalWrite(buzzer,HIGH);  
        digitalWrite(Red_LED,LOW);
        }
        
        else  if (Probe_blink >= 1 && Probe_blink <= 5)
        {
        lcd.setCursor(0, 0);  lcd.print("No Primary Inlet");//..............................................only when temp raised and water is not preset
        lcd.setCursor(0, 1);  lcd.print("     Found      ");
        digitalWrite(Green_LED,LOW);  
        }
      }//................................................................end temp error1.............................................................................//

       else  if(Flow_Err==1)//........................................................................................ok when which is not press i.e.for flow error display
      {
          Probe_blink++;
          digitalWrite(buzzer,LOW); 
          digitalWrite(Red_LED,HIGH); 
          if (Probe_blink >= 6 && Probe_blink <= 7)
          {   
          lcd.setCursor(5, 0);  lcd.print("                ");
          lcd.setCursor(5, 1);  lcd.print("                ");
          Probe_blink=0;
          digitalWrite(buzzer,HIGH); 
          digitalWrite(Red_LED,LOW);  
          }
          else  if (Probe_blink >= 1 && Probe_blink <= 5)
          {  
          lcd.setCursor(0, 0);  lcd.print("      FLOW      "); 
          lcd.setCursor(0, 1);  lcd.print("      ERROR     "); 
          digitalWrite(Green_LED,LOW); 
          }
      }//.............................................................end of flow error...............................................................................//

//....................................................................Relay fail safety...............................................................................//

 else  if(warning_generate==1)//.warning to shut down the main supply
      {
          Probe_blink++;
          digitalWrite(Red_LED,HIGH);
          digitalWrite(buzzer,LOW); 
          if (Probe_blink >= 6 && Probe_blink <= 7)
          {    
          lcd.setCursor(0, 0);  lcd.print("                ");
          lcd.setCursor(0, 1);  lcd.print("                ");
          Probe_blink=0;
          digitalWrite(Red_LED,LOW);   
          digitalWrite(buzzer,HIGH);     
          }
          else  if (Probe_blink >= 1 && Probe_blink <= 5)
          {   
          lcd.setCursor(0, 0);  lcd.print("   SHUT DOWN    "); 
          lcd.setCursor(0, 1);  lcd.print("  POWER SUPPLY  "); 
          }
      }//.................end of relay fail safty......................................................................
      
     }//end of screen 3   
  }//.....................................................................end of lcd............................................................
      
   
   void Displayclass :: pump_disp()//........................................................................................for diplaying calibrated pump 
     {
      if(!sfil_comp)
      {
       screen=0;
      }
      
      if(sfil_comp)                //.................................................................pump calibration not possible till secondary filling
      {
      switch(pcalib) //Pump calibration
      { 
        case 0:
        lcd.setCursor(0, 0);  lcd.print("PRESS SET TO    ");
        lcd.setCursor(0, 1);  lcd.print("START PRIMING   ");
        break;
            
        case 1:
        lcd.setCursor(0, 0);  lcd.print("PRESS SET TO    ");
        lcd.setCursor(0, 1);  lcd.print("START PRIMING   ");
        break;       
        
        case 2:
        if((pcalib==2)&&(calib_count<5))//...........................................................................................To see the count down
        {
         lcd.setCursor(0, 0); lcd.print("      WAIT      ");
         lcd.setCursor(0, 1); lcd.print("                ");
         lcd.setCursor(8, 1); lcd.print((5-calib_count));
         lcd.setCursor(9, 1); lcd.print("          ");
         Once1=0;
        }
        
        if((calib_count>5))//........................................................................................................After count down
        {
        if(Once1==0)
        {
        lcd.setCursor(0, 0); lcd.print("                ");
        lcd.setCursor(0, 1); lcd.print("PRIMING...      ");
        digitalWrite(peristalitic_pump,HIGH);
        delay(7000);
        digitalWrite(peristalitic_pump,LOW);
        Once1=1;
        }
       
        lcd.setCursor(0, 0);  lcd.print("PRESS SET TO   ");
        lcd.setCursor(0, 1);  lcd.print("SET POINT OF DIS");
        }      
        break ;
      
        case 3:
        lcd.setCursor(0, 0);  lcd.print("VOLUME:         ");
        lcd.setCursor(8, 0);  lcd.print(Point_dispense);
        lcd.setCursor(0, 1);  lcd.print("PRESS DISPENSE  ");
        Probe_blink++;
        if (Probe_blink >= 5 && Probe_blink <6)
        {
          lcd.setCursor(8, 0);
          lcd.print("             ");
          Probe_blink=0;
         }
        else  if(Probe_blink >= 1 && Probe_blink <= 4)
         {
          lcd.setCursor(8, 0);lcd.print(Point_dispense); 
         }
        break ;
     
        case 15:
        lcd.setCursor(0, 0);  lcd.print("ENTER VOLUME     ");
        lcd.setCursor(0, 1);  lcd.print("ERROR:          ");
        Probe_blink++;
        if (Probe_blink >= 5 && Probe_blink <= 6)
         {
          lcd.setCursor(6, 1);
          lcd.print("   ");
          Probe_blink=0;
         }
        else  if (Probe_blink >= 1 && Probe_blink <= 4)
         {
          int error=Pump_error+Point_dispense;
          lcd.setCursor(6, 1);lcd.print(error); 
         }
        break;      
    }
      }//....end sfil_comp   
 }//.......................................................................end of pump display.............................................

Displayclass disp = Displayclass();

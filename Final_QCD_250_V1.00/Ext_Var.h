
#ifndef Ext_Var_h
#define Ext_Var_h

#include "OneButton.h"
#include "arduino.h"
#include "display.h"
#include "button.h"
#include "Ticker.h"
#include "LiquidCrystal.h"
#include "Temp_sensing.h"
#include "Dispensing.h"
#include "Input_Checking.h"
#include "Process.h"
#include "EEPROM_Funct.h"
#include "buzzer_onoff.h"


extern const int     buzzer;
extern unsigned long capcity1;  // delete after use
extern unsigned long currentTime;
extern int           menu_select,Pump_error,Probe1,Probe2,Probe1_actual,Probe2_actual,Heater_safety_timer,Flow_Err_INC;
extern uint32_t      capacity,VOLUME,VOLUME1,Point_dispense,sfil_on;
extern uint8_t       safe_temp1,Heater_safety,STEP,emer_start_stop,count,tim_disp,i,Level_var,level_detect,Pump1;
extern uint8_t       vol_set,pcalib,back,Disp_t,safe_temp,screen,sec_Heater;
extern bool          flip,F_override,warning_generate,Sign1,Sign2,Flow_override,thermopile,L_override,temp_clear,stop_pump; 
extern bool          dispense,boil2_var,Temp_Err,Flow_Err,once1,Process1,Sfill,Stop_cycle,sfil,Pump_calib,a,b,sfil_comp,flowerror_screen_switch;
extern float         Heater_temp,Stop_C,calib_Heater1,Heater_temp2,calib_Heater2;
//extern const String  boilor2_status[];

extern void button_update();
extern void pump_off();
extern void pump_on();
extern void disp_count();
extern void process_stop(); 
extern byte calib_count;

class Vclass
{
  public:
  Vclass();
};

extern Vclass var;

#endif

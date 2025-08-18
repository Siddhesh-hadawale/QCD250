#include "arduino.h"
#include "Ext_Var.h"
#include "EEPROM_Funct.h"

eepromclass::eepromclass()
{}

uint8_t probe1_calibvalue=0,probe2_calibvalue=0;

void eepromclass::Write_memory()
{
   switch(menu_select)
   {
    case 0:
    
    break;
    
    case 1:
    safe_temp=safe_temp1;
//    writeUnsignedIntIntoEEPROM(safe_tempMemory,safe_temp );
    EEPROM.put(safe_tempMemory, safe_temp);  
           
    break;
    
    case 2://probe 1 value save
    if(Probe1_actual<0)//.............................................................................................save the value into another var and then it it again
    {           //.............................................................................................recover it and then tranfer to another var 
       Sign1=1;                                                                                               //added now
       probe1_calibvalue=Probe1_actual;
       probe1_calibvalue=probe1_calibvalue*(-1);
    }
    
    else 
    {
      Sign1=0;
      probe1_calibvalue=Probe1_actual;
    }
      writeUnsignedIntIntoEEPROM(probe1_calibvalueMemory,probe1_calibvalue );
      EEPROM.write(SignMemory1, Sign1);   
    break;

    case 3://probe 2 value save
    if(Probe2_actual<0)//.............................................................................................save the value into another var and then it it again
    {   
       Sign2=1;        //added now....................................................................................recover it and then tranfer to another var 
       probe2_calibvalue=Probe2_actual;
       probe2_calibvalue=probe2_calibvalue*(-1);
    }
    
    else
    {
      Sign2=0;
      probe2_calibvalue=Probe2_actual;
    }  
      writeUnsignedIntIntoEEPROM(probe2_calibvalueMemory,probe2_calibvalue ); 
      EEPROM.write(SignMemory2, Sign2);   
    break;
    
    case 4:
      writeUnsignedIntIntoEEPROM(Pump1Memory  ,Pump1);   
    break;
   }
}

void eepromclass::Read_memory()
{
//    safe_temp= readUnsignedIntFromEEPROM(safe_tempMemory); 
      EEPROM.get(safe_tempMemory, safe_temp);
//    sfil_comp=EEPROM.read(sfil_compMemory ); 
    probe1_calibvalue= readUnsignedIntFromEEPROM(probe1_calibvalueMemory);   
    probe2_calibvalue= readUnsignedIntFromEEPROM(probe2_calibvalueMemory);   
    Sign1=EEPROM.read(SignMemory1); 
    Sign2=EEPROM.read(SignMemory2);
     
    if(Sign1)
    {
      Probe1_actual=probe1_calibvalue*(-1);
    }
    else Probe1_actual=probe1_calibvalue;

   if(Sign2)
    {
      Probe2_actual=probe2_calibvalue*(-1);
    }
    else Probe2_actual=probe2_calibvalue;
    
   
    Pump1= readUnsignedIntFromEEPROM(Pump1Memory); 
    delay(100);
}

void eepromclass:: writeUnsignedIntIntoEEPROM(int address, unsigned int number)
{ 
    EEPROM.write(address, number >> 8);
    EEPROM.write(address + 1, number & 0xFF);
}

unsigned int  eepromclass::readUnsignedIntFromEEPROM(int address)
{
    return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}
 
eepromclass EEPROM1 = eepromclass(); 

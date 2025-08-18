#ifndef EEPROM_Funct_h
#define EEPROM_Funct_h
#include <EEPROM.h>

#define safe_tempMemory             14
#define probe1_calibvalueMemory     6
#define probe2_calibvalueMemory     18
//#define sfil_compMemory             10
#define Pump1Memory                 1
#define SignMemory1                 4
#define SignMemory2                 22


class eepromclass
{
  public:
  eepromclass();
  void Write_memory();
  void Read_memory();
  void writeUnsignedIntIntoEEPROM(int address, unsigned int number);
  unsigned int readUnsignedIntFromEEPROM(int address);
};

extern eepromclass EEPROM1;

#endif

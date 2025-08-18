
 #ifndef Temp_sensing_h
 #define Temp_sensing_h

 #define Temp_sense3 13     //connector J23  
 #define Temp_sense1 14     //connector J22                                                   //boilor temperature probe
  
class Tempclass
{
  public:
  Tempclass();
  void Temp_Setup();
  void TempRead_Boilor();
  void TempRead_Inletwater();
  void TempRead_Boilor2();
 };

extern Tempclass Temperature1;

#endif

  

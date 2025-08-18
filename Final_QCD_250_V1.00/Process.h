#ifndef Process_h
#define Process_h

#define solonoid   20 
#define Contacter1 19
#define Contacter2 18 
#define Heater2 21                      //middle connector(J4)
#define Heater1 22                      //connector(J3)
#define Red_LED 24                      //active high
#define Green_LED 25 

class Processclass
{
  public:
  Processclass();
  void process_Setup();
  void process_start();
  void Resume_t();
  void primary_check();
  void Tank_Level_Status();
  void process_stop();
};

extern Processclass Pros;

#endif

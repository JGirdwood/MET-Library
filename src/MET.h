#ifndef _met
#define _met
#endif

#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>

class MET
{
public:
  //Constructor
  MET(); 

  //Variables
  String filename;            //filename (altered in 'createlog' function)
  RTC_DS1307 RTC;             //RTC class defined in 'RTClib.h'
  File Log;                   //class for file
  int METout[2];

  //Functions
  void rtcprint(DateTime inst, Stream &out, char del);
  bool createlog(Stream &out);
  bool startRTC(Stream &out);
  bool startSD(const char SDcs, const char LEDpin, Stream &out);
  void error(bool error_indicator, Stream &out, const char LEDpin);
  void setupMonitor(Stream &out);
  void METread(const char humpin, const char brhipin, const char brlopin);
  void openLog(Stream &out);
  void METprint(Stream &out, char del);
  void closeLog(const char _CS);
  
private:

};

























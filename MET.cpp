#include "MET.h"
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>

MET::MET(){
  filename = "DATA00.csv";          //filename (altered in 'createlog' function)
}

void MET::rtcprint(DateTime inst, Stream &out, char del){
  out.print(inst.year(), DEC);
  out.print("/");
  out.print(inst.month(), DEC);
  out.print("/");
  out.print(inst.day(), DEC);
  out.print(del);
  out.print(inst.hour(), DEC);
  out.print(":");
  out.print(inst.minute(), DEC);
  out.print(":");
  out.println(inst.second(), DEC);
}

bool MET::createlog(Stream &out){
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i/10 + '0';
    filename[5] = i%10 + '0';
    if (! SD.exists(filename)) {
      break;
    }
  }
  openLog(out);
  if(!Log){
    out.println("!!!Could Not Create Log File!!!");
    return false;
  }else{
    out.print("Log File: ");
    out.println(filename);
    Log.flush();
    Log.close();
    return true;
  }
}

bool MET::startRTC(Stream &out){
  Wire.begin();  
  if (!RTC.begin()) {
    out.println("!!!RTC failed!!!");
    return false;
  }
  //RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  if (! RTC.isrunning()) {
    out.println("RTC is NOT running!");
	RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    return false;
  }else{
    out.println("RTC initialised");
    return true;
  }
}

bool MET::startSD(const char SDcs, const char LEDpin, Stream &out){
  if(!SD.begin(SDcs)){
    out.println("!!!SD card error!!!");
    return false;
  }else{
    digitalWrite(LEDpin,HIGH);
    out.println("SD card detected");
    return true;
  }
}

void MET::error(bool error_indicator, Stream &out, const char LEDpin){
  if(error_indicator == false){
    while(true){
      digitalWrite(LEDpin, HIGH);
      delay(500);
      digitalWrite(LEDpin, LOW);
      delay(500);
    }
  }else{
    out.println("No Errors");
  }
}

void MET::setupMonitor(Stream &out){
  Log = SD.open(filename, FILE_WRITE);
  DateTime now = RTC.now();
  rtcprint(now, Log, ',');
  rtcprint(now, out, '\t');
  Log.flush();
  Log.close();
}

void MET::METread(const char humpin, const char brhipin, const char brlopin){
  int HIHadc = analogRead(humpin);
  int TEMPadc = abs(analogRead(brhipin) - analogRead(brlopin));
  METout[0] = TEMPadc;
  METout[1] = HIHadc;
}

void MET::METprint(Stream &out, char del){
  out.print(METout[0]);
  out.print(del);
  out.print(METout[1]);
}

void MET::openLog(Stream &out){
  uint8_t timeout = 0;
  Log = SD.open(filename, FILE_WRITE);
  if (!Log){
    do{
      Log = SD.open(filename, FILE_WRITE);
      out.print("Waiting for Log to open: ");
      out.print(timeout);
      out.println(" Seconds");
      delay(1000);
      timeout ++;
    }while ((timeout<=20)&&(!Log)); 
  }
}

void MET::closeLog(const char _CS){
  Log.flush();
  Log.close();
  while(_CS == LOW){}
}


















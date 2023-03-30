
#include <Wire.h>
#include "Adafruit_HTU31D.h"
Adafruit_HTU31D htu = Adafruit_HTU31D();
uint32_t timestamp;
bool heaterEnabled = false;

bool init_temp_humid_sensor(){
  if (!htu.begin(0x40)) {
    return false;
    // Serial.println("Couldn't find sensor!");
    // while (1);
  }
  timestamp = millis();
  return true;
}

String read_temp_humid(){
  sensors_event_t humidity, temp;

  String outstr="";


  
  htu.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  // outstr+="temperature:"+String(temp.temperature);
  // outstr+=" relative_humidity:"+String(humidity.relative_humidity);
  // outstr+=" heater:"+String(heaterEnabled);
  // outstr+=" h_res:"+String(-1);
  // outstr+=" t_res:"+String(-1);

  float temp_fahrenheit=(temp.temperature*1.8)+32;

  outstr+=String(temp.temperature)+"C\n"+String(temp_fahrenheit)+"F";
  outstr+="\n"+String(humidity.relative_humidity)+"%";


  return outstr;
}


String serial_temp_humid(){

  sensors_event_t humidity, temp;
  htu.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  String outstr="temperature:";
  outstr+=String(temp.temperature);
  outstr+=" humid:";
  outstr+=String(humidity.relative_humidity);

  outstr+=" heater:0 h_res:0 t_res:0";

  return outstr;
}
#include "Adafruit_LC709203F.h"

Adafruit_LC709203F lc;



String read_battery_temperature(){
  return String((int)lc.getCellTemperature())+"C";
}


bool init_battery_sensor() {

  if (!lc.begin())
    return false;

  lc.setThermistorB(3950);

  lc.setPackSize(LC709203F_APA_3000MAH);

  lc.setAlarmVoltage(3.8);

  lc.setTemperatureMode(LC709203F_TEMPERATURE_THERMISTOR);

  return true;
}


String read_battery_sensor() {
  String outstr="";

  outstr+="Batt Voltage: "  +(String) lc.cellVoltage();
  outstr+=", Batt Percent: "+(String) lc.cellPercent();
  return outstr;
}

String read_battery_sensor_formatted() {
  String outstr="";

  outstr+=(String) lc.cellVoltage()+"V";
  outstr+="\n"+(String) lc.cellPercent()+"%";
  outstr+="\n"+(String) read_battery_temperature();

  return outstr;
}

String serial_read_battery() {
  String outstr="volt:";

  outstr+=(String) lc.cellVoltage();
  outstr+=" pct:"+(String) lc.cellPercent();
  outstr+=" temp:"+(String) lc.getCellTemperature();

  return outstr;
}


String read_battery_voltage() {
  return String (lc.cellVoltage())+"V";
}
float read_battery_voltage_raw() {
  return lc.cellVoltage();
}



String read_battery_percent() {
  return String (lc.cellPercent())+"%";
}


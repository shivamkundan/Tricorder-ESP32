#include "Adafruit_PM25AQI.h"

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

bool init_pm25() {
  if (! aqi.begin_I2C())
	return false;
  return true;
}

String read_pm25() {

  String outstr="";
  PM25_AQI_Data data;

  if (! aqi.read(&data)) {
	Serial.println("Could not read from AQI");
	delay(500);  // try again in a bit!
	return "!";
  }
  // Serial.println("AQI reading success");

  // Serial.println();
  // Serial.println(F("---------------------------------------"));
  // Serial.println(F("Concentration Units (standard)"));
  // Serial.println(F("---------------------------------------"));
  // Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  // Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  // Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  // Serial.println(F("Concentration Units (environmental)"));
  // Serial.println(F("---------------------------------------"));
  // Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  // Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  // Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  // Serial.println(F("---------------------------------------"));
  // Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  // Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  // Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  // Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  // Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  // Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  // Serial.println(F("---------------------------------------"));

  outstr+="03um:"+String(data.particles_03um);
  outstr+="  05um:"+String(data.particles_05um);
  outstr+="\n10um:"+String(data.particles_10um);
  outstr+="  25um:"+String(data.particles_25um);
  outstr+="\n50um:"+String(data.particles_50um);
  outstr+="  100um:"+String(data.particles_100um);
  // Serial.println(outstr);
  delay(400);
  return outstr;
// '03um','05um','10um','25um','50um','100um'

}
String serial_pm25() {

  String outstr="";
  PM25_AQI_Data data;

  if (! aqi.read(&data)) {
  Serial.println("Could not read from AQI");
  delay(500);  // try again in a bit!
  return "!";
  }

  outstr+="03um:"+String(data.particles_03um);
  outstr+=" 05um:"+String(data.particles_05um);
  outstr+=" 10um:"+String(data.particles_10um);
  outstr+=" 25um:"+String(data.particles_25um);
  outstr+=" 50um:"+String(data.particles_50um);
  outstr+=" 100um:"+String(data.particles_100um);
  // Serial.println(outstr);
  delay(400);
  return outstr;
// '03um','05um','10um','25um','50um','100um'

}

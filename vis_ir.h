/* TSL2591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)

uint32_t t_prev=0;
String outstr="";

String gain_labels[]={"1x (Low)","25x (Medium)","428x (High)","9876x (Max)"};

void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}


bool configureSensor(void)
{


  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  // tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  // tsl2591Gain_t gain = tsl.getGain();

  // options: TSL2591_INTEGRATIONTIME_{i}MS for i=200,400,500,600 (bright to light)
  tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // tsl2591IntegrationTime_t integrationTime = tsl.getTiming();
  // Serial.println(integrationTime);


  // Serial.print  (F("Timing:       "));
  // Serial.print((tsl.getTiming() + 1) * 100, DEC);
  // Serial.println(F(" ms"));
  // Serial.println(F("------------------------------------"));
  // Serial.println(F(""));
  return true;
}


/**************************************************************************/
/*
	Program entry point for the Arduino sketch
*/
/**************************************************************************/
bool init_vis_ir(void){
	if (tsl.begin()) {
		configureSensor();
		return true;
	}
	else
		return false;
}

void set_vis_ir_gain(char new_gain){

  if (new_gain=='A'){
      tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
      tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
    }
  else if (new_gain=='B'){
      tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
      tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  }
  else if (new_gain=='C'){
      tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
      tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  }
  else if (new_gain=='D'){
      tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
      tsl.setGain(TSL2591_GAIN_MAX);   // 9876x gain
    }
}
/**************************************************************************/
/*
	Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
String read_vis_ir(void)
{
  String vis_ir_outstr=" ";
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full,vis;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  vis=full - ir;


  // vis_ir_outstr="infrared:"+String(ir)+" full_spectrum:"+String(full)+ " visible:"+String(vis)+" lux:"+String(tsl.calculateLux(full, ir)) + " tsl2591_gain:-1";
  // vis_ir_outstr="IR:"+String(ir)+"\nFull:"+String(full)+ " \nVis:"+String(vis)+"\nLux:"+String(tsl.calculateLux(full, ir));
  vis_ir_outstr=String(ir)+"/"+String(vis)+"\nFull:"+String(full)+"\nLux:"+String(tsl.calculateLux(full, ir));

  return vis_ir_outstr;

}


String serial_vis_ir(void)
{

  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full,vis;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  vis=full - ir;

  String outstr="lux:";
  outstr+=String(tsl.calculateLux(full, ir));
  outstr+=" infrared:";
  outstr+=String(ir);
  outstr+=" visible:";
  outstr+=String(vis);
  outstr+=" full_spectrum:";
  outstr+=String(full);
  outstr+=" tsl2591_gain:";
  outstr+=String(tsl.getGain());
  outstr+=" tsl2591_timing:";
  outstr+=String(tsl.getTiming());

  return outstr;

}


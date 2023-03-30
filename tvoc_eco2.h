
#include "Adafruit_SGP30.h"
Adafruit_SGP30 sgp;
String labels[]={"eCO2:"," TVOC:"," baseline_eCO2:"," baseline_TVOC:"," abs_humid:"};
const uint8_t RESET_COUNT=30;
int sgp30_counter = 0;
uint16_t TVOC_base, eCO2_base;
/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
void reset();
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
	// approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
	const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
	const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
	return absoluteHumidityScaled;
}

bool init_tvoc_eco2() {
  if (! sgp.begin())
	return false;

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
  return true;
}


String read_tvoc_eco2() {
	// If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
	//float temperature = 22.1; // [°C]
	//float humidity = 45.2; // [%RH]
	//sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

	String outstr="";

	if (! sgp.IAQmeasure()) {
		// Serial.println("Measurement failed");
		return "!";}

	outstr+="eCO2:"+String(sgp.TVOC);	//ppb
	outstr+="\nTVOC:"+String(sgp.eCO2);	//ppm

	// Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
	// Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

	if (! sgp.IAQmeasureRaw()) {
		Serial.println("Raw Measurement failed");
		return "!";}

	// Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
	// Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

	outstr+="\nH2:"+String(sgp.rawH2);
	outstr+="\neth:"+String(sgp.rawEthanol);

	sgp.getIAQBaseline(&eCO2_base, &TVOC_base);
	// outstr+=" baseline_eCO2:"+String(eCO2_base);
	// outstr+=" baseline_TVOC:"+String(TVOC_base);
	// Serial.println(outstr);

	sgp30_counter++;
	if (sgp30_counter == RESET_COUNT)
		reset();

	return outstr;
}

String serial_tvoc_eco2() {
	// If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
	//float temperature = 22.1; // [°C]
	//float humidity = 45.2; // [%RH]
	//sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

	// ['eCO2','TVOC','raw_H2','raw_ethanol','baseline_eCO2','baseline_TVOC']

	String outstr="";

	if (! sgp.IAQmeasure()) {
		Serial.println("Measurement failed");
		return "!";}

	outstr+="eCO2:"+String(sgp.TVOC);	//ppb
	outstr+=" TVOC:"+String(sgp.eCO2);	//ppm

	// Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
	// Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

	if (! sgp.IAQmeasureRaw()) {
		Serial.println("Raw Measurement failed");
		return "!";}

	// Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
	// Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

	outstr+=" raw_H2:"+String(sgp.rawH2);
	outstr+=" raw_ethanol:"+String(sgp.rawEthanol);

	sgp.getIAQBaseline(&eCO2_base, &TVOC_base);
	outstr+=" baseline_eCO2:"+String(eCO2_base);
	outstr+=" baseline_TVOC:"+String(TVOC_base);
	// Serial.println(outstr);

	sgp30_counter++;
	if (sgp30_counter == RESET_COUNT)
		reset();

	return outstr;
}


// void read_tvoc_eco2_continuous(){
// 	while (!touch2detected){


// 		if (! sgp.IAQmeasure()) {
// 				Serial.println("Measurement failed");
// 				return "!";}

// 		uint32_t TVOC=sgp.TVOC
// 		uint32_t eCO2=sgp.eCO2

// 		outstr+="eCO2:"+String();	//ppb
// 			outstr+="\nTVOC:"+String(sgp.eCO2);	//ppm
// 		if (! sgp.IAQmeasureRaw()) {
// 				Serial.println("Raw Measurement failed");
// 				return "!";}
// 		outstr+="\nraw_H2:"+String(sgp.rawH2);
// 			outstr+="\nraw_ethanol:"+String(sgp.rawEthanol);
// 		sgp30_counter++;
// 			if (sgp30_counter == RESET_COUNT)
// 				reset();


// 		send_msg=read_tvoc_eco2();delay(10);
// 		display.clearDisplay();
// 		testdrawstyles(send_msg,0,0,2);
// 		// status_bar();
// 	}

// }

void reset()
{

	sgp30_counter=0;
	if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
		Serial.println("Failed to get baseline readings");
		return;}

//	Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
//	Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
}

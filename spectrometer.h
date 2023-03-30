/*
 * This file handles all functions for AS7341 spectrometer
 */
#include <Adafruit_AS7341.h>
Adafruit_AS7341 as7341;

// String channels[]={"c_415nm:"," c_445nm:"," c_480nm:"," c_515nm:"," c_555nm:"," c_590nm:"," c_630nm:"," c_680nm:"," Clear:"," NIR:"};
// const uint8_t indexes[]={0,1,2,3,6,7,8,9,10,11};
// const uint8_t len_labels=10;
// const uint8_t array_size=12;

String channels[]={"c_415nm:","c_445nm:","c_480nm:","c_515nm:","blank","blank","c_555nm:","c_590nm:","c_630nm:","c_680nm:","clear:","NIR:"};
const uint8_t indexes[]={0,1,2,3,6,7,8,9,10,11};
const uint8_t len_labels=8;
const uint8_t array_size=12;

bool init_spectrometer(){
	if (!as7341.begin())
		return false;

	as7341.setATIME(100);
	as7341.setASTEP(999);
	as7341.setGain(AS7341_GAIN_256X);
	return true;
}

String read_spectrometer(){

	String outstr="";
	uint16_t readings[array_size];
	if (!as7341.readAllChannels(readings))
		return "1";

	for (int i=0;i<len_labels;i+=2){
		outstr+=channels[i]+String(readings[indexes[i]]);
		outstr+="  "+channels[i+1]+String(readings[indexes[i+1]]);
		outstr+"\n";}

	// outstr+="gain:"+String(as7341.getGain());
	// Serial.println(outstr);

	return outstr;
}

String serial_read_spectrometer(){
	String outstr="";
	uint16_t counts[array_size];
	uint16_t readings[array_size];
	if (!as7341.readAllChannels(readings))
		return "1";

	// for (int i=0;i<array_size;i++){
	// 	Serial.print(readings[i]);
	// 	Serial.print(" ");
	// }
	// Serial.println("");


	for (int i=0;i<12;i+=1){
		if (i!=4 && i!=5)
		outstr+=channels[i]+String(readings[i])+" ";
		// outstr+=" "+channels[i+1]+String(readings[indexes[i+1]]);
	}

	// outstr+="gain:"+String(as7341.getGain());
	// Serial.println(outstr);

	return outstr;
}

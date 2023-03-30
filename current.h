#include <Adafruit_INA260.h>
#include <Wire.h>

Adafruit_INA260 ina260 = Adafruit_INA260();


float voltage=-1;
String voltage_suffix="mV";


bool init_current_sensor() {

	Serial.println("Adafruit INA260 Test");

	if (!ina260.begin(0x45,&Wire)) {
		return false;
	}
	// Serial.println("Found INA260 chip");

	ina260.setMode(INA260_MODE_CONTINUOUS); // continuously update the current
	ina260.setAlertType(INA260_ALERT_UNDERCURRENT); // set alert mode to overcurrent
	// Ina260_5V.setAlertLimit(700); // set alert current threshold to 700mA
	// Ina260_5V.setAlertPolarity(INA260_ALERT_POLARITY_NORMAL); // set alert polarity to active high
	// Ina260_5V.setAlertLatch(INA260_ALERT_LATCH_ENABLED); // latch the alert pin until mask/enable register is read


	return true;
}


String read_current(){
	return (String((int) ina260.readCurrent())+"mA");
}

String read_voltage(){
	voltage=ina260.readBusVoltage();
	if (voltage>1000){
		voltage=voltage/1000;
		voltage_suffix="V";
	}
	else
		voltage_suffix="mV";

	return String( voltage+voltage_suffix);
}

String read_power(){
	return (String((int)ina260.readPower())+"mW");
}

String serial_read_current_sensor() {

	String outstr="";


	// outstr+=("Current: ");
	// outstr+=("I:");
	outstr+=(String(ina260.readCurrent()));
	outstr+=" "+String(ina260.readBusVoltage());

	outstr+=(" ");
	outstr+=(String(ina260.readPower()));


	return outstr;
}

String read_current_sensor() {

	String outstr="";


	// outstr+=("Current: ");
	// outstr+=("I: ");
	outstr+=(String((int)ina260.readCurrent()));
	outstr+=("mA\n");


	voltage=ina260.readBusVoltage();
	if (voltage>1000){
		voltage=voltage/1000;
		voltage_suffix="V\n";
	}
	else
		voltage_suffix="mV\n";

	// outstr+=("Bus Voltage: ");
	// outstr+=("V: ");
	outstr+=(voltage);
	outstr+=(voltage_suffix);

	// outstr+=("Power: ");
	// outstr+=("P: ");
	outstr+=(String((int)ina260.readPower()));
	outstr+=("mW");

	return outstr;
}

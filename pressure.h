#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;

bool init_pressure() {

	if (!bmp.begin_I2C())
		return false;

	// Set up oversampling and filter initialization
	bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
	bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
	bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
	bmp.setOutputDataRate(BMP3_ODR_50_HZ);
	return true;
}

String read_pressure() {
	if (! bmp.performReading())
		return "!";

	String outstr="";

	outstr+="pressure:"+String(bmp.pressure / 100.0);
	outstr+="\nbmp_temp:"+String(bmp.temperature);
	// outstr+=" p_over:"+String(-1);
	// outstr+=" t_over:"+String(-1);
	outstr+="\nalt:"+String(bmp.readAltitude(SEALEVELPRESSURE_HPA));

	// Serial.println(outstr);
	return outstr;



	// Serial.print("Temperature = ");
	// Serial.print(bmp.temperature);
	// Serial.println(" *C");

	// Serial.print("Pressure = ");
	// Serial.print(bmp.pressure / 100.0);
	// Serial.println(" hPa");

	// Serial.print("Approx. Altitude = ");
	// Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
	// Serial.println(" m");


	// delay(500);
}

String serial_read_pressure() {
	if (! bmp.performReading())
		return "!";

	String outstr="";

	outstr+="pressure:"+String(bmp.pressure / 100.0);
	outstr+=" bmp_temp:"+String(bmp.temperature);
	outstr+=" p_over:"+String(-1);
	outstr+=" t_over:"+String(-1);
	outstr+=" alt:"+String(bmp.readAltitude(SEALEVELPRESSURE_HPA));

	// Serial.println(outstr);
	return outstr;

}

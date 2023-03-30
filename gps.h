#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define TASK_SERIAL_RATE 100
#define TIMEDIFF_HRS 6

struct GpsDataState_t {
    double originLat = 0;
    double originLon = 0;
    double originAlt = 0;
    double distMax = 0;
    double dist = 0;
    double altMax = -999999;
    double altMin = 999999;
    double spdMax = 0;
    double prevDist = 0;
};


TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

GpsDataState_t gpsState = {};
uint32_t nextSerialTaskTs = 0;
uint8_t num_satellites = -1;



uint8_t get_satellite_count(){
	while (SerialGPS.available() > 0)
			gps.encode(SerialGPS.read());

	return( gps.satellites.value());
}

void gps_screen(uint8_t num_satellites){

		while (SerialGPS.available() > 0) {
			gps.encode(SerialGPS.read());
		}

		if (num_satellites > 4) {
			gpsState.dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), gpsState.originLat, gpsState.originLon);

			if (gpsState.dist > gpsState.distMax && abs(gpsState.prevDist - gpsState.dist) < 50) {
				gpsState.distMax = gpsState.dist;
			}

			gpsState.prevDist = gpsState.dist;

			if (gps.altitude.meters() > gpsState.altMax) {
				gpsState.altMax = gps.altitude.meters();
			}

			if (gps.speed.mps() > gpsState.spdMax) {
				gpsState.spdMax = gps.speed.mps();
			}

			if (gps.altitude.meters() < gpsState.altMin) {
				gpsState.altMin = gps.altitude.meters();
			}
		}

		if (nextSerialTaskTs < millis()) {

			display_R.setTextColor(SSD1306_WHITE); // normal text
			display_R.setTextSize(2);


			uint8_t start_col=0;
			uint8_t start_row=0;


			display_R.setCursor(0,16);
			// display_R.print("LAT  = ");
			display_R.print(gps.location.lat(), 6);
			display_R.print("\n");

			// display_R.setCursor(3,11+15);
			// display_R.print("LONG = ");
			display_R.print(gps.location.lng(), 6);
			display_R.print("\n");

			// display_R.setCursor(3,22+15);
			// display_R.print("ALT  = ");
			display_R.print(gps.altitude.meters());
			display_R.print("m\n");

			display_R.print(gpsState.dist,1);

			// Serial.println(wd(int year, int month, int day));
			nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
		}

	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("GPS", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("GPS");
}




String serial_gps(){

		while (SerialGPS.available() > 0) {
			gps.encode(SerialGPS.read());
		}

		if (num_satellites > 4) {
			gpsState.dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), gpsState.originLat, gpsState.originLon);

			if (gpsState.dist > gpsState.distMax && abs(gpsState.prevDist - gpsState.dist) < 50) {
				gpsState.distMax = gpsState.dist;
			}

			gpsState.prevDist = gpsState.dist;

			if (gps.altitude.meters() > gpsState.altMax) {
				gpsState.altMax = gps.altitude.meters();
			}

			if (gps.speed.mps() > gpsState.spdMax) {
				gpsState.spdMax = gps.speed.mps();
			}

			if (gps.altitude.meters() < gpsState.altMin) {
				gpsState.altMin = gps.altitude.meters();
			}
		}

		if (nextSerialTaskTs < millis()) {

			// Serial.println(wd(int year, int month, int day));
			nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
		}


	String outstr="lat:";
	outstr+=String(gps.location.lat());
	outstr+=" lng:";
	outstr+=String(gps.location.lng());
	outstr+=" alt:";
	outstr+=String(gps.altitude.meters());
	outstr+=" spd:";
	outstr+=String(gps.speed.mps());
	outstr+=" sat:";
	outstr+=String(get_satellite_count());

	return outstr;

}
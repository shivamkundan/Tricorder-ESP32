/*
This file contains code for displaying each sensor's "page(s)".
Each function defines the layout, text size, etc., depending upon the best way to view the given sensor's readings.
*/

// ---------------------- GPS ---------------------- //
void gps_screen(uint8_t num_satellites){


	if (gps_en){
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
	}
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("GPS", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("GPS");
}

void gps_magnetometer_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("GPS/MAGNETOMETER", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("GPS/MAGNETOMETER");
	display_R.setTextSize(1);
	display_R.setCursor(0, 16);
	display_R.print(read_gps_magnetometer());
}
// ------------------------------------------------- //

void multimeter_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("MULTIMETER", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("MULTIMETER");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	// display_R.setCursor(1,17);
	display_R.print(read_current_sensor());
}

void battery_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("BATTERY", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("BATTERY");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	// display_R.setCursor(1,17);
	display_R.print(read_battery_sensor_formatted());
}

void wind_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("WIND SPEED", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("WIND SPEED");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);

	int wind_rv_val=analogRead(wind_rv_pin);
	// display_R.setCursor(1,17);
	display_R.print(wind_rv_val);
}

void noise_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("NOISE", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("NOISE");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);

	int noise_val=analogRead(noise_pin);
	// display_R.setCursor(1,17);
	display_R.print(noise_val);
}

void vis_ir_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("VIS/IR", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("VIS/IR");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_vis_ir());
}

void uv_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("UV", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("UV");
	display_R.setTextSize(1);
	display_R.setCursor(0, 16);
	String uv_str=read_uv();
	display_R.print(uv_str);
}

void spectrometer_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("SPECTROMETER", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("SPECTROMETER");
	display_R.setTextSize(1);
	display_R.setCursor(0, 16);
	display_R.print(read_spectrometer());
}

void temp_humid_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("TEMP/HUMID", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("TEMP/HUMID");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_temp_humid());
}

void pressure_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("PRESSURE", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("PRESSURE");
	display_R.setTextSize(1);
	display_R.setCursor(0, 16);
	display_R.print(read_pressure());
}

void tvoc_eco2_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("TVOC/CO2", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("TVOC/CO2");
	display_R.setTextSize(1);
	display_R.setCursor(0, 16);
	display_R.print(read_tvoc_eco2());
}

void pm25_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("PM25", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("PM25");
	display_R.setTextSize(1);
	display_R.setCursor(0, 16);
	display_R.print(read_pm25());
}

void thermal_cam_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("THERMAL CAM", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("THERMAL CAM");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_thermal_cam_stats());
}

void radiation_screen(){
	display_R.setTextSize(1);
	display_R.print("       ");
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("RADIATION", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("RADIATION");


	uint16_t CPM=0;
	float running_average=0.0;

	String cpm_str="";
	String avg_str="";

	CPM=geiger.getCPM();

	if (CPM>0){
		geiger.pause();
		if (i==array_size)
		i=0;

		running_average=0.0;
		//Pause the count, turn off the external interrupt trigger, the CPM and radiation intensity values remain in the state before the pause
		//geiger.pause();
		//Get the current CPM, if it has been paused, the CPM is the last value before the pause
		//Predict CPM by falling edge pulse within 3 seconds, the error is ±3CPM

		vals[i]=CPM;

		//   Serial.print(i);
		//   Serial.print(": ");
		for (int k=0;k<array_size;k++){
		//    if (k==i)
		//      Serial.print("*");
		  running_average+=vals[k];
		//    Serial.print(vals[k]);
		//    Serial.print (" ");
		}
		running_average/=array_size;

		cpm_str=String(CPM);
		avg_str=String(running_average);

		i++;
		geiger.start();
		delay(3000);
	}
	else
	{
		cpm_str="XX";
		avg_str=String("XX");
	}

	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	String geiger_output=String("CPM: "+String(CPM)+"\n"+"AVG: "+String(running_average));
	display_R.print(geiger_output);

	// display_R.setCursor(50,50);
	// display_R.setTextSize(1);
	// display_R.print("waiting");
}

// -------------------- IMU ------------------------ //
void imu_orientation_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("IMU/ORIENTATION", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("IMU/ORIENTATION");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_orientation());
}

void imu_lin_acc_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("IMU/LINEAR ACC", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("IMU/LINEAR ACC");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_lin_acc());
}

void imu_magnetometer_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("IMU/MAGNETOMETER", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("IMU/MAGNETOMETER");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_magnetometer());
}

void imu_accelerometer_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("IMU/ACCELEROMETER", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("IMU/ACCELEROMETER");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_accelerometer());
}

void imu_gravity_screen(){
	display_R.setTextSize(1);
	int16_t x1, y1;
	uint16_t w, h;
	display_R.getTextBounds("IMU/GRAVITY", 0, 0, &x1, &y1, &w, &h);
	display_R.setCursor(display_L.width()/2-w/2, 4);
	display_R.print("IMU/GRAVITY");
	display_R.setTextSize(2);
	display_R.setCursor(0, 16);
	display_R.print(read_gravity());
}
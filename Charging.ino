#include <Wire.h>
#include "types.h"
#include "oled.h"
#include "gps.h"
#include "time_funcs.h"
#include "current.h"
#include "battery.h"
#include "nav_handler.h"
#include "info_display.h"
// #include "magnetometer.h"
#include "imu.h"



#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include "bitmaps.h"

#include "vis_ir.h"
#include "tvoc_eco2.h"
#include "uv.h"
#include "temp_humid.h"
#include "pressure.h"
#include "spectrometer.h"
#include "thermal_cam.h"
#include "pm25.h"
#include "magnetometer.h"
// #include "imu.h"


#include <Adafruit_NeoPixel.h>
#define BRIGHTNESS 2
#define LED_COUNT  8
Adafruit_NeoPixel strip_big(LED_COUNT, A5, NEO_GRBW + NEO_KHZ800);

#include "led.h"

#include <DFRobot_Geiger.h>

#define geiger_detect_pin 37
#define array_size 20
DFRobot_Geiger  geiger(geiger_detect_pin);
uint16_t vals[array_size];
uint8_t i=0;

bool INDEPENDENT_MODE=true;

connected_sensors CS;


int prog_timer=0;
unsigned long prev_millis=0;
unsigned long curr_millis=0;

bool GPS_EN=true;
bool SENSORS_EN=false;
bool GEIGER_EN=false;
bool TSL_CLK_EN=false;


uint8_t hours_internal=0;
uint8_t minutes_internal=0;
uint8_t seconds_internal=0;
bool am_pm_internal=false;



bool my_imu=false;






// ------- timer interrupt ------- //
// volatile int interruptCounter;
// int totalInterruptCounter;

// hw_timer_t * timer = NULL;
// portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// void IRAM_ATTR onTimer() {
//   portENTER_CRITICAL_ISR(&timerMux);
//   interruptCounter++;
//   portEXIT_CRITICAL_ISR(&timerMux);

// }
// ----------------------------- //

void toggle_mosfets(){
	if (PM25_EN)
		digitalWrite(PM25_EN_PIN,HIGH);
	else
		digitalWrite(PM25_EN_PIN,LOW);

	if (GEIGER_EN)
		digitalWrite(GEIGER_EN_PIN,HIGH);
	else
		digitalWrite(GEIGER_EN_PIN,LOW);

	if (TSL_CLK_EN)
		digitalWrite(TSL_CLK_EN_PIN,HIGH);
	else
		digitalWrite(TSL_CLK_EN_PIN,LOW);
}


void my_rainbow() {

	int whiteSpeed=75;
	int whiteLength=1;

  if(whiteLength >= 8) whiteLength = 8 - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<8; i++) {  // For each pixel in strip...
      // if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
      //    ((tail > head) && ((i >= tail) || (i <= head)))) {
      //   strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      // } else {                                             // else set rainbow
      //   int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      //   strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      // }
    	int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
    	strip_big.setPixelColor(i, strip_big.gamma32(strip_big.ColorHSV(pixelHue)));
    }

    strip_big.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip_big.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip_big.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void setup() {


	Serial.begin(115200);

	oled=init_oled();

	strip_big.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
	strip_big.show();            // Turn OFF all pixels ASAP
	// strip_big.setBrightness(BRIGHTNESS);

	// my_rainbow();

	// colorWipe(strip_big.Color(0,   0,   0)     , 0); // off
	for(int i=8; i>=0; i--) { // For each pixel in strip...
	  strip_big.setPixelColor(i, (255,0,0));         //  Set pixel's color (in RAM)
	                           //  Update strip to match
	  strip_big.show();
	  delay(30);                           //  Pause for a moment
	}
	strip_big.show();

	// colorWipe(strip_big.Color(0,   0,   0)     , 0); // off
	for(int i=8; i>=0; i--) { // For each pixel in strip...
	  strip_big.setPixelColor(i, 0);         //  Set pixel's color (in RAM)
	                           //  Update strip to match
	  strip_big.show();
	  delay(30);                           //  Pause for a moment
	}
	strip_big.show();



	init_sensors();
	// digitalWrite(NEOPIXEL_I2C_POWER, HIGH);



	pinMode(buttonPin, INPUT);

	pinMode(FIVE_V_EN_PIN, OUTPUT);
	digitalWrite(FIVE_V_EN_PIN,HIGH);

	gpio_pullup_dis(PM25_EN_PIN);
	gpio_pulldown_en(PM25_EN_PIN);
	gpio_pullup_dis(GEIGER_EN_PIN);
	gpio_pulldown_en(GEIGER_EN_PIN);

	gpio_pulldown_dis(TSL_CLK_EN_PIN);
	gpio_pullup_en(TSL_CLK_EN_PIN);

	gpio_pullup_dis(GPS_EN_PIN);
	gpio_pulldown_en(GPS_EN_PIN);


	pinMode(PM25_EN_PIN, OUTPUT);
	pinMode(GEIGER_EN_PIN, OUTPUT);
	pinMode(TSL_CLK_EN_PIN, OUTPUT);
	pinMode(GPS_EN_PIN, OUTPUT);

	pinMode(NEOPIXEL_I2C_POWER, OUTPUT);


	toggle_mosfets();




	if (GPS_EN){
		digitalWrite(GPS_EN_PIN,HIGH);

		// magnetometer=init_magnetometer();
		SerialGPS.begin(9600, SERIAL_8N1, 7, 8);

		gpsState.originLat = gps.location.lat();
		gpsState.originLon = gps.location.lng();
		gpsState.originAlt = gps.altitude.meters();

		gpsState.distMax = 0;
		gpsState.altMax = -999999;
		gpsState.spdMax = 0;
		gpsState.altMin = 999999;
		while (SerialGPS.available() > 0)
			gps.encode(SerialGPS.read());
	}
	else{
		digitalWrite(GPS_EN_PIN,LOW);
		gps_initialized=true;
	}



	// print_wakeup_reason();
	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
	gpio_pullup_dis(GPIO_NUM_38);
	gpio_pulldown_en(GPIO_NUM_38);
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_38,0); //1 = High, 0 = Low


	current_sensor=init_current_sensor();
	battery_sensor=init_battery_sensor();

	for (int i=0;i<current_array_size;i++)
		current_vals[i]=0;


	touchAttachInterrupt(LEFT_TOUCH_PIN, left_touch, threshold);
	touchAttachInterrupt(RIGHT_TOUCH_PIN, right_touch, threshold);
	touchAttachInterrupt(UP_TOUCH_PIN, up_touch, threshold);
	touchAttachInterrupt(DOWN_TOUCH_PIN, down_touch, threshold);

	// timer = timerBegin(0, 80, true);
	// timerAttachInterrupt(timer, &onTimer, true);
	// timerAlarmWrite(timer, 1000000, true);
	// timerAlarmEnable(timer);


	my_imu=init_imu();

	// ----------- Geiger -----------  //
	for (i=0;i<array_size;i++){
	  vals[i]=0;
	}

	i=0;

	//Start counting, enable external interrupt
	geiger.start();

	pinMode(wind_out_pin, INPUT);
	pinMode(noise_out_pin, INPUT);

	oled_clear();
	display_L.display();
	display_R.display();

}


void handle_usb_serial_input(){
	if (Serial.available()){

		String teststr = Serial.readString();  //read until timeout
		teststr.trim();                        // remove any \r \n whitespace at the end of the String

		char c=teststr[0];
		// Serial.println(c);

		if (teststr.length()>1){
			if (c=='L')
				set_vis_ir_gain(teststr[1]);
		}

		// char c = Serial.read();
		else
		{// ----------- Mode  ----------- //
				if (c=='<'){
					INDEPENDENT_MODE=false;
					oled_clear();
					display_L.display();
					display_R.display();

				}
				else if (c=='>'){
					INDEPENDENT_MODE=true;
				}
				// ----------- Power ----------- //
				else if (c=='A')
					digitalWrite(TSL_CLK_EN_PIN,HIGH);
				else if (c=='&')
					digitalWrite(TSL_CLK_EN_PIN,LOW);
				else if (c=='C')
					digitalWrite(GPS_EN_PIN,HIGH);
				else if (c=='E')
					digitalWrite(GPS_EN_PIN,LOW);
				else if (c=='F')
					digitalWrite(PM25_EN_PIN,HIGH);
				else if (c=='H')
					digitalWrite(PM25_EN_PIN,LOW);
				else if (c=='I')
					digitalWrite(GEIGER_EN_PIN,HIGH);
				else if (c=='J')
					digitalWrite(GEIGER_EN_PIN,LOW);
				else if (c=='~')
					ESP.restart();

				else if (c=='^')
					go_to_sleep();
				else if (c=='('){
					digitalWrite(NEOPIXEL_I2C_POWER, HIGH);
					digitalWrite(GPS_EN_PIN,HIGH);
					digitalWrite(FIVE_V_EN_PIN,HIGH);
					SLEEP_EN=false;
				}

				// ----------- Sensors ----------- //
				else if (c=='L')
					Serial.println(serial_vis_ir());
				else if (c=='U')
					Serial.println(read_uv());
				else if (c=='T')
					Serial.println(serial_temp_humid());
				else if (c=='P')
					Serial.println(serial_read_pressure());
				else if (c=='M')
					Serial.println(serial_pm25());
				else if (c=='S')
					Serial.println(serial_read_spectrometer());
				else if (c=='V')
					Serial.println(serial_tvoc_eco2());
				else if (c=='R')
					Serial.println(read_thermal_cam());
				else if (c=='B')
					Serial.println(serial_read_battery());
				else if (c=='c')
					Serial.println(serial_read_current_sensor());
				else if (c=='g')
					Serial.println("CPM:"+String(geiger.getCPM()));
				else if (c=='G')
					Serial.println(serial_gps());


				else if (c=='#'){
					// String w="wind_out:"+String(analogRead(wind_out_pin));
					String w="";
					w+=String(analogRead(wind_out_pin))+" ";
					// w+=String(analogRead(wind_out_pin))+" ";
					// w+=String(analogRead(wind_out_pin));
					Serial.println(w);
				}
				else if (c=='$'){
					String w="noise_out:"+String(analogRead(noise_out_pin));
					Serial.println(w);
				}

				// ----------- IMU ----------- //
				else if (c=='O')
					Serial.println(serial_read_orientation());
				else if (c=='Q')
					Serial.println(serial_read_angular_velocity());
				else if (c=='W')
					Serial.println(serial_read_lin_acc());
				else if (c=='X')
					Serial.println(serial_read_magnetometer());
				else if (c=='Y')
					Serial.println(serial_read_accelerometer());
				else if (c=='Z')
					Serial.println(serial_read_gravity());}
	}
}


void loop() {

	handle_usb_serial_input();


	if (INDEPENDENT_MODE){
		while (countdown_t_remain>0){
			countdown_t_remain-=1;
			// Serial.println(countdown_t_remain);
			delay(20);
		}


		oled_clear();

		if (curr_screen_name=="menu")
			draw_menu();

		else if (curr_screen_name=="gps"){
			GPS_EN=true;
			if (GPS_EN){
				digitalWrite(GPS_EN_PIN,HIGH);

				// magnetometer=init_magnetometer();
				SerialGPS.begin(9600, SERIAL_8N1, 7, 8);

				gpsState.originLat = gps.location.lat();
				gpsState.originLon = gps.location.lng();
				gpsState.originAlt = gps.altitude.meters();

				gpsState.distMax = 0;
				gpsState.altMax = -999999;
				gpsState.spdMax = 0;
				gpsState.altMin = 999999;
				while (SerialGPS.available() > 0)
					gps.encode(SerialGPS.read());
			}
			gps_screen(num_satellites);
		}

		else if (curr_screen_name=="sleep"){
			// cur
			go_to_sleep();
		}


		else if (curr_screen_name=="multimeter"){
			multimeter_screen();
		}
		else if (curr_screen_name=="battery"){
			battery_screen();
		}

		else if (curr_screen_name=="vis_ir"){
			vis_ir_screen();
		}
		else if (curr_screen_name=="spectrometer"){
			spectrometer_screen();
		}

		else if (curr_screen_name=="temp_humid"){
			temp_humid_screen();
		}
		else if (curr_screen_name=="pressure"){
			pressure_screen();
		}
		else if (curr_screen_name=="tvoc_eco2"){
			tvoc_eco2_screen();
		}
		else if (curr_screen_name=="pm25"){
			pm25_screen();
		}
		else if (curr_screen_name=="thermal_cam"){
			thermal_cam_screen();
		}

		// else if (curr_screen_name=="imu")

		if (curr_screen_name=="radiation"){

			radiation_screen();
		}

		if (curr_screen_name=="imu_orientation"){
			imu_orientation_screen();
		}
		if (curr_screen_name=="imu_orientation"){
			imu_orientation_screen();
		}

		if (curr_screen_name=="imu_lin_acc"){
			imu_lin_acc_screen();
		}
		if (curr_screen_name=="imu_magnetometer"){
			imu_magnetometer_screen();
		}

		if (curr_screen_name=="imu_accelerometer"){
			imu_accelerometer_screen();
		}
		if (curr_screen_name=="imu_gravity"){
			imu_gravity_screen();
		}



		frame_num+=1;

		curr_millis=millis();
		prog_timer=curr_millis-prev_input_time;


		// digitalWrite(TSL_CLK_EN_PIN,HIGH);
		// delay(50);
		// Serial.println(read_vis_ir());
		// Serial.println("---");

		// // digitalWrite(TSL_CLK_EN_PIN,LOW);
		// // delay(10);
		// digitalWrite(TSL_CLK_EN_PIN,LOW);
		// delay(50);
		// Serial.println(read_orientation());
		// Serial.println();

		// if (prog_timer>SLEEP_T && !Serial.available() && gps_initialized)
		// 	go_to_sleep();



		if (!SLEEP_EN){


			if (GPS_EN && !gps_initialized){
				if (get_satellite_count()>MIN_SATELLITES)
					gps_initialized=true;

					hours_internal=gps.time.hour();
					minutes_internal=gps.time.minute();
					seconds_internal=gps.time.second();
				display_R.setCursor(110, 5);
				display_R.print("NO");
			}

			int16_t x1, y1;
			uint16_t w, h;

			String t=get_time();
			display_L.getTextBounds(t, 0, 0, &x1, &y1, &w, &h);
			display_L.setCursor(display_L.width()/2-w/2, 5);
			display_L.print(t);

			// ------------------------ battery ------------------------ //
			float batt_v=read_battery_voltage_raw();
			display_battery_stats(batt_v);

			// ------------------------ current ------------------------ //
			float curr=ina260.readCurrent();
			display_current_sensor_stats(curr);

			// ---------------------- charging status ------------------ //
			display_charging_stats(batt_v,curr);

			// ----------------------- sensor count -------------------- //
			uint8_t row=41;
			String L3="9";
			display_L.setCursor(99,row+10);
			display_L.print(L3);

			// ------------------------- GPS -------------------------- //
			if (GPS_EN){
				// read_magnetometer();
				uint8_t num_satellites = get_satellite_count();
				// gps_screen(num_satellites);
				display_L.setCursor(20, 5);
				display_L.print(num_satellites);
			}

			// ---------------------- gruntwork ----------------------- //


			// display_R.setCursor(0, 5);
			// display_R.print((int)(SLEEP_T-prog_timer)/1000);


			display_L.setCursor(110, 5);
			// display_L.print(frame_num);
			display_L.print((int)(SLEEP_T-prog_timer)/1000);

			if (current_array_index==current_array_size-1)
				current_array_index=0;
			current_array_index+=1;
			oled_update();
			delay(200);



			if (millis()-prev_millis>1000){
				prev_millis=millis();}


			// 	if (seconds_internal==59){
			// 		seconds_internal=0;
			// 		minutes_internal+=1;

			// 		if (minutes_internal==59)
			// 			hours_internal+=1;
			// 	}
			// 	else
			// 		seconds_internal+=1;


			// 	Serial.println(String(get_hour_internal(hours_internal))+" "+String(minutes_internal)+" "+String(seconds_internal));
			// }

			// if (interruptCounter > 0) {

			//   portENTER_CRITICAL(&timerMux);
			//   interruptCounter--;
			//   portEXIT_CRITICAL(&timerMux);

			//   totalInterruptCounter++;

			//   Serial.print("An interrupt as occurred. Total number: ");
			//   Serial.println(totalInterruptCounter);
			//   Serial.println(millis()-curr_millis);
			//   Serial.println("");

			// }
		}
	}

}


// ----------------------------------------------------------------------- //
void draw_menu(){
	// display_R.getTextBounds("VIS/IR", 0, 0, &x1, &y1, &w, &h);
	display_R.setTextSize(1);
	display_R.setTextColor(SSD1306_WHITE); // Draw 'inverse' text
	display_R.drawRect(0, 35, display_R.width()-12, 17, SSD1306_WHITE);

	display_R.setCursor(7, R1);
	display_R.print(menu_items[curr_selection-1]);

	display_R.setCursor(5, R2);
	display_R.print(menu_items[curr_selection]);


	display_R.setCursor(5, R3);
	display_R.print(menu_items[curr_selection+1]);
}

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void go_to_sleep(){
	SLEEP_EN=true;
	oled_clear();
	oled_update();

	digitalWrite(PM25_EN_PIN,LOW);
	digitalWrite(GPS_EN_PIN,LOW);
	digitalWrite(GEIGER_EN_PIN,LOW);
	digitalWrite(FIVE_V_EN_PIN,LOW);
	digitalWrite(NEOPIXEL_I2C_POWER, LOW);
	// esp_deep_sleep_start();
}

void init_sensors(){
	CS.current_sensor=init_current_sensor();
	CS.battery_sensor=init_battery_sensor();
	CS.vis_ir=init_vis_ir();
	CS.tvoc_eco2=init_tvoc_eco2();
	CS.uv=init_uv();
	CS.temp_humid=init_temp_humid_sensor();
	CS.pressure=init_pressure();
	CS.spectrometer=init_spectrometer();
	CS.thermal_cam=init_thermal_cam();
	CS.pm25=init_pm25();
	CS.gps_magnetometer=init_magnetometer();
	CS.imu=init_imu();
}


// ------------------------------------- //
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
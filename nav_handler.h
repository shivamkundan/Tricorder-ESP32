#define LEFT_TOUCH_PIN 27
#define RIGHT_TOUCH_PIN 33
#define UP_TOUCH_PIN 12
#define DOWN_TOUCH_PIN 15

#define COUNT_TIME 10
int countdown_t_remain = COUNT_TIME;
uint32_t prev_input_time=0;
volatile uint8_t curr_selection=1;
volatile uint8_t curr_screen=curr_selection;

// ---- touch pin stuff --- //
int threshold = 30;
int touch_count=0;


String curr_screen_name="menu";

bool PM25_EN=false;

void nav_interrupt_handler(uint8_t pin_num){
	prev_input_time=millis();

	// // Serial.println(pin_num);
	// Serial.println(curr_selection);
	// // Serial.println("");

	if (countdown_t_remain==0){
		countdown_t_remain=COUNT_TIME;

		switch(pin_num) {
			// case LEFT_TOUCH_PIN:
			// 	if (curr_selection>0)
			// 			curr_selection-=1;
			// 	break;

			case UP_TOUCH_PIN:
				if (curr_selection>1)
						curr_selection-=1;
				break;
			case DOWN_TOUCH_PIN:
				if (curr_selection<num_menu_items-1)
						curr_selection+=1;
				break;

	// 		case RIGHT_TOUCH_PIN:
	// 			curr_screen=curr_selection;
	// 			Serial.println(curr_screen);
	// 			break;
		}
	}
}

void nav_interrupt_handler_screen_select(uint8_t pin_num){
	prev_input_time=millis();
	// prog_timer=0;
	if (countdown_t_remain==0){
		countdown_t_remain=COUNT_TIME;


		if (curr_screen_name=="menu"){
			if (curr_selection==2)
				curr_screen_name="gps";
		}


		curr_screen=curr_selection;
		Serial.println(curr_screen);

	}
}

void left_touch(){
	// nav_interrupt_handler(LEFT_TOUCH_PIN);

	if (curr_screen_name=="radiation")
		digitalWrite(GEIGER_EN_PIN,LOW);
	else if (curr_screen_name=="vis_ir"){
		digitalWrite(TSL_CLK_EN_PIN,LOW);
	}
	if (curr_screen_name=="pm25_screen"){
		// digitalWrite(PM25_EN_PIN,LOW);
		PM25_EN=false;
	}
	// digitalWrite(PM25_EN_PIN,LOW);
	// digitalWrite(TSL_CLK_EN_PIN,LOW);
	// digitalWrite(NEOPIXEL_I2C_POWER, LOW);

	curr_screen_name="menu";
}

void right_touch(){
	// nav_interrupt_handler_screen_select(RIGHT_TOUCH_PIN);
	prev_input_time=millis();
	// prog_timer=0;
	if (countdown_t_remain==0){
		countdown_t_remain=COUNT_TIME;


		if (curr_screen_name=="menu"){
			if (curr_selection==1)
				curr_screen_name="sleep";
			if (curr_selection==2)
				curr_screen_name="gps";

			if (curr_selection==3)
				curr_screen_name="multimeter";
			if (curr_selection==4)
				curr_screen_name="battery";


			if (curr_selection==7){
				digitalWrite(TSL_CLK_EN_PIN,HIGH);
				curr_screen_name="vis_ir";

			}
			if (curr_selection==8)
				curr_screen_name="uv";
			if (curr_selection==9)
				curr_screen_name="spectrometer";
			if (curr_selection==10)
				curr_screen_name="temp_humid";
			if (curr_selection==11)
				curr_screen_name="pressure";
			if (curr_selection==12)
				curr_screen_name="tvoc_eco2";
			if (curr_selection==13){
				digitalWrite(PM25_EN_PIN,HIGH);
				PM25_EN=true;
				curr_screen_name="pm25";
			}
			if (curr_selection==14)
				curr_screen_name="thermal_cam";
			if (curr_selection==15)
				curr_screen_name="imu";
			if (curr_selection==16){
				digitalWrite(GEIGER_EN_PIN,HIGH);
				curr_screen_name="radiation";
			}


			if (curr_selection==19){
				digitalWrite(TSL_CLK_EN_PIN,LOW);
				curr_screen_name="imu_orientation";
			}
			if (curr_selection==20){
				digitalWrite(TSL_CLK_EN_PIN,LOW);
				curr_screen_name="imu_lin_acc";
			}
			if (curr_selection==21){
				digitalWrite(TSL_CLK_EN_PIN,LOW);
				curr_screen_name="imu_magnetometer";
			}
			if (curr_selection==22){
				digitalWrite(TSL_CLK_EN_PIN,LOW);
				curr_screen_name="imu_accelerometer";
			}
			if (curr_selection==23){
				digitalWrite(TSL_CLK_EN_PIN,LOW);
				curr_screen_name="imu_gravity";
			}

		}


		curr_screen=curr_selection;
		// Serial.println(curr_screen);

	}
}

void up_touch(){
	nav_interrupt_handler(UP_TOUCH_PIN);
}

void down_touch(){
	nav_interrupt_handler(DOWN_TOUCH_PIN);
}

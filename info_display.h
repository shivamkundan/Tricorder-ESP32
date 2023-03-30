
#define BATT_CAPACITY 2500
const uint8_t current_array_size = 20;
uint16_t current_vals[current_array_size];
uint8_t current_array_index=0;

void display_battery_stats(float batt_v){

	String batt_temp=read_battery_temperature();


	String batt_voltage=String(batt_v)+"V";
	String batt_pct=read_battery_percent();

	int row=21;
	display_L.setCursor(0, row);
	display_L.print(batt_temp);

	int16_t x1, y1;
	uint16_t w, h;
	display_L.getTextBounds(batt_voltage, 0, 0, &x1, &y1, &w, &h);
	display_L.setCursor(display_L.width()/2-w/2, row);
	display_L.print(batt_voltage);


	display_L.getTextBounds(batt_pct, 0, 0, &x1, &y1, &w, &h);
	display_L.setCursor(display_L.width()-w, row);
	display_L.print(batt_pct);
}

void display_current_sensor_stats(float curr){
	if (curr<0.0)
		current_vals[current_array_index]=-1*curr;
	else
		current_vals[current_array_index]=curr;
	String current=(String((int) curr)+"mA");


	String voltage=read_voltage();
	String power=read_power();


	int row=31;
	display_L.setCursor(0, row);
	display_L.print(power);


	int16_t x1, y1;
	uint16_t w, h;
	display_L.getTextBounds(voltage, 0, 0, &x1, &y1, &w, &h);
	display_L.setCursor(display_L.width()/2-w/2, row);
	display_L.print(voltage);


	display_L.getTextBounds(current, 0, 0, &x1, &y1, &w, &h);
	display_L.setCursor(display_L.width()-w, row);
	display_L.print(current);
}

void display_charging_stats(float batt_v,float curr){
	float batt_t_remain=-1;


	float total_current=0;
	for (int i=0;i<current_array_size;i++)
		total_current+=current_vals[i];

	float avg_current=(total_current/current_array_size);


	// testdrawbitmap_L(epd_bitmap_clock,0,27+20,16,16);

	testdrawbitmap_L(epd_bitmap_alert,112,27+20+1,16,18);
	testdrawbitmap_L(epd_bitmap_sat_signal,0,0,16,16);


	int hours=-1;
	int minutes=-1;

	String bat_msg="ERR";
	String bat_msg_l2="ERR";
	if (batt_v>99){
		bat_msg = "fully";
		bat_msg_l2="charged";
	}
	else if (curr>15){
		batt_t_remain=(BATT_CAPACITY*lc.cellPercent()/100)/avg_current;
		hours=(int)batt_t_remain;
		minutes=(int) 60* (batt_t_remain-hours);
		bat_msg=String(hours)+"h:"+String(minutes)+"m";//+ "$" + String(avg_current);
		bat_msg_l2="remaining";
		testdrawbitmap_L(epd_bitmap_clock,0,27+20,16,16);
	}
	else if (curr<=-15){
		float x=((100-lc.cellPercent())/100)*BATT_CAPACITY;
		batt_t_remain=x/avg_current;
		hours=(int)batt_t_remain;
		minutes=(int) 60* (batt_t_remain-hours);
		bat_msg=String(hours)+"h:"+String(minutes)+"m";//+ "$" + String(avg_current);
		bat_msg_l2="till full";
		// testdrawbitmap_L(epd_bitmap_clock,0,27+20,16,16);


		testdrawbitmap_L(epd_bitmap_lightning,0,27+20,10,16);
	}
	else{
		bat_msg="not";
		bat_msg_l2="charging";
		testdrawbitmap_L(epd_bitmap_alert,0,27+20+1,16,18);
	}


	uint8_t row=41;
	display_L.setCursor(21, row+5);
	display_L.print(bat_msg);
	display_L.setCursor(21, row+9+5);
	display_L.print(bat_msg_l2);
}
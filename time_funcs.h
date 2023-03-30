String PM_suffix="P";
String AM_suffix="A";
String suffix_am_pm="";
String minutes = "-1";

uint8_t get_hour(){
	uint8_t hour;
	hour=gps.time.hour();
	// Serial.println(hour);

	// if (hour>=0 && hour<=TIMEDIFF_HRS){
	// 	hour+=5;
	// 	suffix_am_pm="P";
	// }

	// else if (hour>TIMEDIFF_HRS && hour<=17){
	// 	hour-=5;
	// 	suffix_am_pm="A";
	// }

	// else if (hour>=18 && hour<=23){
	// 	hour-=5;
	// 	suffix_am_pm="P";
	// }

	// if (hour>12)
	// 	hour-=12;

	// return hour;

	switch (hour){
		case 0:
			hour=7;
			suffix_am_pm=PM_suffix;
			break;
		case 1:
			hour=8;
			suffix_am_pm=PM_suffix;
			break;
		case 2:
			hour=9;
			suffix_am_pm=PM_suffix;
			break;
		case 3:
			hour=10;
			suffix_am_pm=PM_suffix;
			break;
		case 4:
			hour=11;
			suffix_am_pm=PM_suffix;
			break;
		case 5:
			hour=12;
			suffix_am_pm=AM_suffix;
			break;
		case 6:
			hour=1;
			suffix_am_pm=AM_suffix;
			break;
		case 7:
			hour=2;
			suffix_am_pm=AM_suffix;
			break;
		case 8:
			hour=3;
			suffix_am_pm=AM_suffix;
			break;
		case 9:
			hour=4;
			suffix_am_pm=AM_suffix;
			break;
		case 10:
			hour=5;
			suffix_am_pm=AM_suffix;
			break;
		case 11:
			hour=6;
			suffix_am_pm=AM_suffix;
			break;
		case 12:
			hour=7;
			suffix_am_pm=AM_suffix;
			break;
		case 13:
			hour=8;
			suffix_am_pm=AM_suffix;
			break;
		case 14:
			hour=9;
			suffix_am_pm=AM_suffix;
			break;
		case 15:
			hour=10;
			suffix_am_pm=AM_suffix;
			break;
		case 16:
			hour=11;
			suffix_am_pm=AM_suffix;
			break;
		case 17:
			hour=12;
			suffix_am_pm=PM_suffix;
			break;
		case 18:
			hour=1;
			suffix_am_pm=PM_suffix;
			break;
		case 19:
			hour=2;
			suffix_am_pm=PM_suffix;
			break;
		case 20:
			hour=3;
			suffix_am_pm=PM_suffix;
			break;
		case 21:
			hour=4;
			suffix_am_pm=PM_suffix;
			break;
		case 22:
			hour=5;
			suffix_am_pm=PM_suffix;
			break;
		case 23:
			hour=6;
			suffix_am_pm=PM_suffix;
			break;

	}

	return hour;
}


uint8_t get_hour_internal(uint8_t hour){

	switch (hour){
		case 0:
			hour=7;
			suffix_am_pm=PM_suffix;
			break;
		case 1:
			hour=8;
			suffix_am_pm=PM_suffix;
			break;
		case 2:
			hour=9;
			suffix_am_pm=PM_suffix;
			break;
		case 3:
			hour=10;
			suffix_am_pm=PM_suffix;
			break;
		case 4:
			hour=11;
			suffix_am_pm=PM_suffix;
			break;
		case 5:
			hour=12;
			suffix_am_pm=AM_suffix;
			break;
		case 6:
			hour=1;
			suffix_am_pm=AM_suffix;
			break;
		case 7:
			hour=2;
			suffix_am_pm=AM_suffix;
			break;
		case 8:
			hour=3;
			suffix_am_pm=AM_suffix;
			break;
		case 9:
			hour=4;
			suffix_am_pm=AM_suffix;
			break;
		case 10:
			hour=5;
			suffix_am_pm=AM_suffix;
			break;
		case 11:
			hour=6;
			suffix_am_pm=AM_suffix;
			break;
		case 12:
			hour=7;
			suffix_am_pm=AM_suffix;
			break;
		case 13:
			hour=8;
			suffix_am_pm=AM_suffix;
			break;
		case 14:
			hour=9;
			suffix_am_pm=AM_suffix;
			break;
		case 15:
			hour=10;
			suffix_am_pm=AM_suffix;
			break;
		case 16:
			hour=11;
			suffix_am_pm=AM_suffix;
			break;
		case 17:
			hour=12;
			suffix_am_pm=PM_suffix;
			break;
		case 18:
			hour=1;
			suffix_am_pm=PM_suffix;
			break;
		case 19:
			hour=2;
			suffix_am_pm=PM_suffix;
			break;
		case 20:
			hour=3;
			suffix_am_pm=PM_suffix;
			break;
		case 21:
			hour=4;
			suffix_am_pm=PM_suffix;
			break;
		case 22:
			hour=5;
			suffix_am_pm=PM_suffix;
			break;
		case 23:
			hour=6;
			suffix_am_pm=PM_suffix;
			break;

	}

	return hour;
}


int get_weekday(char * str) {
  struct tm tm;
  memset((void *) &tm, 0, sizeof(tm));
  if (strptime(str, "%d-%m-%Y", &tm) != NULL) {
    time_t t = mktime(&tm);
    if (t >= 0) {
      return localtime(&t)->tm_wday; // Sunday=0, Monday=1, etc.
    }
  }
  return -1;
}

const char *wd(int year, int month, int day) {
  /* using C99 compound literals in a single line: notice the splicing */
  return ((const char *[])                                         \
          {"Monday", "Tuesday", "Wednesday",                       \
           "Thursday", "Friday", "Saturday", "Sunday"})[           \
      (                                                            \
          day                                                      \
        + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) \
        + (365 * (year + 4800 - ((14 - month) / 12)))              \
        + ((year + 4800 - ((14 - month) / 12)) / 4)                \
        - ((year + 4800 - ((14 - month) / 12)) / 100)              \
        + ((year + 4800 - ((14 - month) / 12)) / 400)              \
        - 32045                                                    \
      ) % 7];
}



String get_time(){
	String hour = String(get_hour());
	uint8_t mins = gps.time.minute();
	String seconds = String(gps.time.second());

	// String stringOne = ;


	if (mins<10)
		minutes=String("0"+String(mins));
	else
		minutes=String(mins);

	// Serial.println(minutes);

	return (String(hour + ":" +minutes + ":" + seconds + suffix_am_pm));
}

String get_date(){
	String month = String(gps.date.month());
	String day = String(gps.date.day());
	String year = String(gps.date.year());

	return (String (month+day+year));
}
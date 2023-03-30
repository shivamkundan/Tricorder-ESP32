#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


bool init_magnetometer(){
	if(!mag.begin())
	    return false;
	else
		return true;
}

void read_gps_magnetometer(){
	sensors_event_t event;
	mag.getEvent(&event);

	/* Display the results (magnetic vector values are in micro-Tesla (uT)) */
	Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
	Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
	Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

	// Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
	// Calculate heading when the magnetometer is level, then correct for signs of axis.
	float heading = atan2(event.magnetic.y, event.magnetic.x);

	// Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
	// Find yours here: http://www.magnetic-declination.com/
	// Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
	// If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
	// float declinationAngle = 0.22;
	float declinationAngle = -00.0741764932;
	heading += declinationAngle;

	// Correct for when signs are reversed.
	if(heading < 0)
	  heading += 2*PI;

	// Check for wrap due to addition of declination.
	if(heading > 2*PI)
	  heading -= 2*PI;

	// Convert radians to degrees for readability.
	float headingDegrees = heading * 180/M_PI;

	Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
}
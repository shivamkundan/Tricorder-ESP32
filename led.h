
// #include "mappings.h"

// #define DEFAULT_I2C_PORT &Wire
// #include <Adafruit_TestBed.h>
// extern Adafruit_TestBed TB;
#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN    0  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1  // Number of NeoPixels

Adafruit_NeoPixel strip(1, 0, NEO_GRB + NEO_KHZ800);

void colorWipe(uint32_t color, int wait) {
	for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
	strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
	strip.show();                          //  Update strip to match
	delay(wait);                           //  Pause for a moment
	}
}
void rainbow(int wait) {
	// Hue of first pixel runs 3 complete loops through the color wheel.
	// Color wheel has a range of 65536 but it's OK if we roll over, so
	// just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
	// means we'll make 3*65536/256 = 768 passes through this outer loop:
	for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
	for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
		// Offset pixel hue by an amount to make one full revolution of the
		// color wheel (range of 65536) along the length of the strip
		// (strip.numPixels() steps):
		int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
		// strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
		// optionally add saturation and value (brightness) (each 0 to 255).
		// Here we're using just the single-argument hue variant. The result
		// is passed through strip.gamma32() to provide 'truer' colors
		// before assigning to each pixel:
		strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
	}
	strip.show(); // Update strip with new contents
	delay(wait);  // Pause for a moment
	}
}
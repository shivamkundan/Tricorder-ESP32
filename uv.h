// 'uvs':"-1",'light':"-1",'uvi':"-1",'ltr_lux':"-1",'ltr_gain':"-1",'ltr_res':"-1",'ltr_win_fac':"-1",'ltr_mdelay':"-1",

#include "Adafruit_LTR390.h"

Adafruit_LTR390 ltr = Adafruit_LTR390();





bool init_uv() {

	if ( ! ltr.begin() )
	return false;

	// options LTR390_MODE_ALS and LTR390_MODE_UVS
	ltr.setMode(LTR390_MODE_UVS);

	// options: LTR390_GAIN_{i} for i=1,3,6,9,18
	ltr.setGain(LTR390_GAIN_3);
	// switch (ltr.getGain()) {

	// options: LTR390_RESOLUTION_{i}BIT for i=13,16,16,18,19,20
	ltr.setResolution(LTR390_RESOLUTION_16BIT);

	ltr.setThresholds(100, 1000);
	ltr.configInterrupt(true, LTR390_MODE_UVS);
	return true;
}


// self.uvs
//             / (
//                 (Gain.factor[self.gain] / 18)
//                 * (2 ** Resolution.factor[self.resolution])
//                 / (2**20)
//                 * 2300
//             )
//             * self._window_factor
// self.uvs
//             / (
//                 (3 / 18)
//                 * (2 ** Resolution.factor[self.resolution])
//                 / (2**20)
//                 * 2300
//             )
//             * self._window_factor



String read_uv() {
	if (ltr.newDataAvailable()) {

		String outstr=" ";

		outstr+="uvs:"+String(ltr.readUVS());
		outstr+=" light:"+String(-1);
		outstr+=" uvi:"+String(-1);
		outstr+=" ltr_lux:"+String(-1);
		outstr+=" ltr_gain:"+String(ltr.getGain());
		outstr+=" ltr_res:"+String(ltr.getResolution());
		outstr+=" ltr_win_fac:"+String(-1);
		outstr+=" ltr_mdelay:"+String(-1);
		delay(50);
		// Serial.println(outstr);
		return (outstr);
	}
}

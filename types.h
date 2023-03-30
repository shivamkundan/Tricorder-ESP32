
#define SLEEP_T 120000



// int threshold = 40;

uint8_t frame_num=0;
bool SLEEP_EN=false;

// ------ sensors ----- //
bool oled;
bool current_sensor;
bool battery_sensor;
bool magnetometer;

// ------- GPS -------- //
#define MIN_SATELLITES 5 	//for initializing gps
bool gps_initialized=false;

// ------ power ------ //
#define FIVE_V_EN_PIN GPIO_NUM_13
#define PM25_EN_PIN GPIO_NUM_32
#define GPS_EN_PIN GPIO_NUM_14
#define SENSORS_EN_PIN GPIO_NUM_4

#define TSL_CLK_EN_PIN GPIO_NUM_25
#define GEIGER_EN_PIN GPIO_NUM_26

#define wind_out_pin A2
#define noise_out_pin A3


//RTC_DATA_ATTR int bootCount = 0;
const int buttonPin = 38;  // the number of the pushbutton pin

typedef struct connected_sensors{
        bool vis_ir=false;
        bool uv=false;
        bool spectrometer=false;
        bool temp_humid=false;
        bool pressure=false;
        bool tvoc_eco2=false;
        bool pm25=false;
        bool thermal_cam=false;
        bool battery_sensor=false;
        bool current_sensor=false;
        bool gps_sensor=false;
        bool gps_magnetometer=false;
        bool imu=false;
    } connected_sensors;




// --------- menu items display stuff --------- //
#define COL_L_START 0
#define COL_R_START 66

#define ROW0_START 16
#define ROW1_START 28
#define ROW2_START 40

#define R1 22
#define R2 40
#define R3 55

    // --- menu layout stuff --- //
uint8_t curr_col=0;
uint8_t curr_row=0;


static uint8_t menu_pos[ 6 ][ 2 ] = { { COL_L_START,  ROW0_START}, { COL_R_START, ROW0_START },
								{ COL_L_START,  ROW1_START}, { COL_R_START, ROW1_START },
								{ COL_L_START,  ROW2_START}, { COL_R_START, ROW2_START }};



static uint8_t num_menu_items=25;
static String menu_items[]={
    "",
    "SLEEP",
    "GPS",
    "MULTIMETER",
    "BATTERY",
    "WIND",
    "NOISE",
    "VIS/IR",
    "UV",
    "SPECTROMETER",
    "TEMP/HUMID",
    "PRESSURE",
    "TVOC/CO2",
    "PM25",
    "THERMAL CAM",
    "IMU",
    "RADIATION",
    "LiDAR",
    "GPS/MAGNETOMETER",
    "IMU/ORIENTATION",
    "IMU/LINEAR ACC",
    "IMU/MAGNETOMETER",
    "IMU/ACCELEROMETER",
    "IMU/GRAVITY",
    "WEIGHT",
    "INFO"
    ""
};

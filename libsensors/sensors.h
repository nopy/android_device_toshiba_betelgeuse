/* Sensor handles */
#define HANDLE_ACCELEROMETER	(0)
#define HANDLE_MAGNETIC_FIELD	(1)
#define HANDLE_ORIENTATION		(2)
#define HANDLE_PRESSURE			(3)
#define HANDLE_TEMPERATURE		(4)
#define HANDLE_MAX			    (5)

/* dummy defines, set them to high value */
#define HANDLE_GYROSCOPE		(9999)
#define HANDLE_LIGHT			(9998)
#define HANDLE_PROXIMITY		(9997)

/* Sensor handles */
#define MINDELAY_ACCELEROMETER	(1000)
#define MINDELAY_MAGNETIC_FIELD	(1000)
#define MINDELAY_ORIENTATION	(1000)
#define MINDELAY_GYROSCOPE		(1000)
#define MINDELAY_LIGHT			(0)
#define MINDELAY_PRESSURE		(0)
#define MINDELAY_TEMPERATURE	(0)
#define MINDELAY_PROXIMITY		(0)

/* Constants */
#define LSM_M_MAX_CAL_COUNT 300
#define RADIANS_TO_DEGREES (180.0/M_PI)
#define DEGREES_TO_RADIANS (M_PI/180.0)

/* Magnetometer defines */
#define SENSOR_UX500_G_To_uT (100.0f)
#define SENSOR_UX500_MAGNETOMETER_RANGE LSM303DLH_MAGNETOMETER_RANGE_1_9G
#define AK8975_MODE_NORMAL "1"
#define AK8975_MODE_OFF "0"
#define GAIN_X -100
#define GAIN_Y -100
#define GAIN_Z -100

#if (SENSOR_UX500_MAGNETOMETER_RANGE == LSM303DLH_MAGNETOMETER_RANGE_1_9G)
#define SENSOR_UX500_MAGNETOMETER_MAX (1.9f * SENSOR_UX500_G_To_uT)
#define SENSOR_UX500_MAGNETOMETER_STEP (1.9f * SENSOR_UX500_G_To_uT / 2048.0f)
#else
#error Unknown range
#endif

/* Accelerometer defines */
#define LSM303DLH_A_RANGE_2G "0"
#define LSM303DLH_A_MODE_OFF "0"
#define LSM303DLH_A_MODE_NORMAL "1"
#define LSM303DLH_A_RATE_50 "0"
#define SENSOR_UX500_ACCELEROMETER_RANGE LSM303DLH_ACCELEROMETER_RANGE_4G

#define LSM303DLHC_A_MODE_NORMAL "4"
#define LSM303DLHC_A_RANGE_2G "0"

#if (SENSOR_UX500_ACCELEROMETER_RANGE == LSM303DLH_ACCELEROMETER_RANGE_4G)
#define SENSOR_UX500_ACCELEROMETER_MAX (4.0f)
#define SENSOR_UX500_ACCELEROMETER_STEP \
	(SENSOR_UX500_ACCELEROMETER_MAX / 4096.0f)
#else
#error Unknown range
#endif

/* Gyroscopre defines */
#define L3G4200D_MODE_ON "1"
#define L3G4200D_MODE_OFF "0"
#define L3G4200D_RATE_100 "0"
#define L3G4200D_RANGE_250 "0"

#define SIZE_OF_BUF 27
#define CONVERT_A  (GRAVITY_EARTH * (1.0f/1000.0f))


/* ambient light defines */
#define BH1780GLI_ENABLE "3"
#define BH1780GLI_DISABLE "0"

/* proximity defines */
#define SFH7741_ENABLE "1"
#define SFH7741_DISABLE "0"

/* magnetometer paths*/
char const *const PATH_MODE_MAG =
		"/sys/bus/i2c/drivers/mm_ak8975/0-000c/mode";
char const *const PATH_DATA_MAG =
		"/sys/bus/i2c/drivers/mm_ak8975/0-000c/show_flux";

/* accelerometer paths*/
char const *const PATH_MODE_ACC =
		"/sys/bus/i2c/drivers/lsm303dlh_a/0-0009/mode";
char const *const PATH_RANGE_ACC =
		"/sys/bus/i2c/drivers/lsm303dlh_a/0-0009/range";
char const *const PATH_RATE_ACC =
		"/sys/bus/i2c/drivers/lsm303dlh_a/0-0009/rate";
char const *const PATH_DATA_ACC =
		"/sys/bus/i2c/drivers/lsm303dlh_a/0-0009/data";
//char const *const PATH_ID_ACC =
//		"/sys/devices/platform/nmk-i2c.2/i2c-2/2-0019/id";

/* gyroscope paths dummy declarations, as devices are absent*/
char const *const PATH_MODE_GYR = NULL;
char const *const PATH_RANGE_GYR = NULL;
char const *const PATH_RATE_GYR = NULL;
char const *const PATH_DATA_GYR = NULL;

/* ambient light paths */
char const *const PATH_POWER_LUX = NULL;
char const *const PATH_DATA_LUX = NULL;

/* proximity paths*/
char const *const PATH_POWER_PROX = NULL;
char const *const PATH_DATA_PROX = NULL;
char const *const PATH_INTR_PROX = NULL;

/* Proximity sensor structure */
typedef struct {
    int prox_val;
    char prox_flag;
}Sensor_prox;

/* sensor API integration */

static const struct sensor_t sSensorList[] = {
	{"AK8975 3-axis Magnetic field sensor",
		"",
		1,
		HANDLE_MAGNETIC_FIELD,
		SENSOR_TYPE_MAGNETIC_FIELD,
		SENSOR_UX500_MAGNETOMETER_MAX,
		SENSOR_UX500_MAGNETOMETER_STEP,
		0.83f,
		MINDELAY_MAGNETIC_FIELD,
		{}
	},
	{"LSM303DLH 3-axis Accelerometer sensor",
		"",
		1,
		HANDLE_ACCELEROMETER,
		SENSOR_TYPE_ACCELEROMETER,
		SENSOR_UX500_ACCELEROMETER_MAX,
		SENSOR_UX500_ACCELEROMETER_STEP,
		0.83f,
		MINDELAY_ACCELEROMETER,
		{}
	},
	{"LSM303DLH 3-axis Orientation sensor",
		"",
		1,
		HANDLE_ORIENTATION,
		SENSOR_TYPE_ORIENTATION,
		360.0f,
		1.0f,
		1.66f,
		MINDELAY_ORIENTATION,
		{}
	},
};

static int acc_id;

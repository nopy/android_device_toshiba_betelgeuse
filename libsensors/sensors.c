/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This file is kind of a prototype, or proof of consept if you will.
 * It shows how to communicate with linux kernel sensors with various
 * linux kernel interfaces. Some require ioctl calls, some use
 * the linux kernel input framework and others have simple
 * sysfs interfaces.
 *
 * The code should not be considered production code as such.
 *
 * The file is indeed quite big and messy as the android framework
 * seems to be designed based on assumptions such as all sensors
 * are on the same HW chip. This is not the case in our platform,
 * where we have different HW chips, vendors and also different
 * kernel interface.
 *
 * Another solution for this would be to create a dummy
 * linux kernel driver that acts as a single point communication
 * channel towards Android. That driver will then communicate
 * with the other kernel drivers.
 */

#define LOG_TAG "Sensors"

#include <hardware/sensors.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>

#include <linux/input.h>
#include <sys/time.h>
#include <sys/mman.h>

#include <cutils/atomic.h>
#include <cutils/log.h>
#include <stdlib.h>

#include "sensors.h"

unsigned int count_mag;
unsigned int count_acc;
unsigned int count_gyr;
unsigned int count_lux;
unsigned int count_prox;
unsigned int count_orien;

float last_light_data = 70000.0f;
float last_proximity_data = 100.0f;

unsigned int delay_mag = MINDELAY_MAGNETIC_FIELD;
unsigned int delay_acc = MINDELAY_ACCELEROMETER;
unsigned int delay_gyr = MINDELAY_GYROSCOPE;
unsigned int delay_lux = MINDELAY_LIGHT;
unsigned int delay_prox = MINDELAY_PROXIMITY;
unsigned int delay_orien = MINDELAY_ORIENTATION;

int last_polled;
int continue_next;
int events = 0;

static int count_open_sensors = 0;
static int count_delay_sensors = 0;
static int lockisheld = 0;
static pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t count_threshold_cv;

Sensor_prox  stprox_val;
static pthread_mutex_t mutex_proxval = PTHREAD_MUTEX_INITIALIZER;
void *proximity_getdata();
char thread_exit;

/* sensor driver activate,de-activate and poll functions
   implementation */

static void m_determine_acc_chip();

/*pass values to kernel space*/
static int write_cmd(char const *path, char *cmd, int size)
{
ALOGD("write_cmd(%s, %s, %d )", path, cmd, size ) ;
	int fd, ret;

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		ALOGE("Cannot open %s\n", path);
		return -ENODEV;
	}

	ret = write(fd, cmd, size);
	if (ret != size)
		ALOGE("Error. Wrote: %d, should have written: %d\n", ret, size);

	close(fd);
	return ret;
}


/* implement individual sensor enable and disables */
static int activate_acc(int enable)
{
ALOGD("activate_acc(%d)",enable) ;

	int ret = 0;

	if (enable) {
ALOGD("active_acc enabling") ;
		if (count_acc == 0) {
			if (acc_id == 50) {
				ret = write_cmd(PATH_MODE_ACC, LSM303DLH_A_MODE_NORMAL, 2);
				ret = write_cmd(PATH_RATE_ACC, LSM303DLH_A_RATE_50, 2);
				ret = write_cmd(PATH_RANGE_ACC, LSM303DLH_A_RANGE_2G, 2);
			} else if (acc_id == 51) {
				ret = write_cmd(PATH_MODE_ACC, LSM303DLHC_A_MODE_NORMAL, 2);
				ret = write_cmd(PATH_RANGE_ACC, LSM303DLHC_A_RANGE_2G, 2);
			}

			if (ret != -ENODEV)
				count_acc++;
		} else {
			count_acc++;
		}
	} else {
ALOGD("active_acc disabling") ;
		if (count_acc == 0)
			return 0;
		count_acc--;
		if (count_orien == 0 && count_acc == 0)
			write_cmd(PATH_MODE_ACC, LSM303DLH_A_MODE_OFF, 2);
	}

	return ret;
}

static int activate_mag(int enable)
{
ALOGD("activate_mag(%d)", enable) ;
	int ret = 0;
	if (enable) {
		if (count_mag == 0) {
			ret = write_cmd(PATH_MODE_MAG, AK8975_MODE_NORMAL, 2);

			if (ret != -ENODEV)
				count_mag++;
		} else {
			count_mag++;
		}
	} else {
		if (count_mag == 0)
			return 0;
		count_mag--;
		if (count_orien == 0 && count_mag == 0)
			write_cmd(PATH_MODE_MAG, AK8975_MODE_OFF, 2);
	}
	return ret;
}

static int activate_gyr(int enable)
{
ALOGD("activate_gyr(%d)", enable ) ;
	int ret = 0;
	if (enable) {
		if (count_gyr == 0) {
			ret = write_cmd(PATH_MODE_GYR, L3G4200D_MODE_ON,2);
			ret = write_cmd(PATH_RATE_GYR, L3G4200D_RATE_100, 2);
			ret = write_cmd(PATH_RANGE_GYR, L3G4200D_RANGE_250, 2);

			if (ret != -ENODEV)
				count_gyr++;
		} else {
			count_gyr++;
		}
	} else {
		if (count_gyr == 0)
			return 0;
		count_gyr--;
		if (count_gyr == 0)
			write_cmd(PATH_MODE_GYR, L3G4200D_MODE_OFF,2);
	}
	return ret;
}

static int activate_lux(int enable)
{
ALOGD("activate_lux(%d)", enable ) ;
	int ret = -1;
	if (enable) {
		if (count_lux == 0) {
			ret = write_cmd(PATH_POWER_LUX, BH1780GLI_ENABLE, 2);

			if (ret != -ENODEV)
				count_lux++;
		} else {
			count_lux++;
		}
	} else {
		if (count_lux == 0)
			return 0;
		count_lux--;
		if (count_lux == 0)
			write_cmd(PATH_POWER_LUX, BH1780GLI_DISABLE, 2);
	}
	return ret;
}

static int activate_prox(int enable)
{
ALOGD("activate_prox(%d)", enable ) ;
	int ret = -1;
	static pthread_t thread = -1;

	if (enable) {
		if (count_prox == 0) {
			/*
			 * check for the file path
			 * Initialize thread_exit flag
			 * every time thread is created
			 */
			if ((ret = open(PATH_INTR_PROX, O_RDONLY)) < 0)
				return ret;
			close(ret);
			thread_exit = 0;
			ret = pthread_create(&thread, NULL,proximity_getdata,NULL);
			count_prox++;
		} else {
			count_prox++;
		}
	} else {
		if (count_prox == 0)
			return 0;
		count_prox--;
		if (count_prox == 0) {
			/*
			 * Enable thread_exit to exit the thread
			 * and call thread_join to clean thread data
			 */
			thread_exit = 1;
			ret = pthread_join(thread,NULL);
		}
	}
	return ret;
}

static int activate_orientation(int enable)
{
ALOGD("activate_orientation(%d)", enable) ;
	int ret = 0;
	if (enable) {
		if (count_orien == 0) {
			ret = write_cmd(PATH_MODE_MAG, AK8975_MODE_NORMAL, 2);
			if (acc_id == 50) {
				ret = write_cmd(PATH_MODE_ACC, LSM303DLH_A_MODE_NORMAL, 2);
				ret = write_cmd(PATH_RATE_ACC, LSM303DLH_A_RATE_50, 2);
				ret = write_cmd(PATH_RANGE_ACC, LSM303DLH_A_RANGE_2G, 2);
			} else if (acc_id == 51) {
				ret = write_cmd(PATH_MODE_ACC, LSM303DLHC_A_MODE_NORMAL, 2);
				ret = write_cmd(PATH_RANGE_ACC, LSM303DLHC_A_RANGE_2G, 2);
			}

			if (ret != -ENODEV)
				count_orien++;
		} else {
			count_orien++;
		}
	} else {
		if (count_orien == 0)
			return 0;
		count_orien--;
		if (count_orien == 0 && count_acc == 0 && count_mag == 0) {
			write_cmd(PATH_MODE_MAG, AK8975_MODE_OFF, 2);
			write_cmd(PATH_MODE_ACC, LSM303DLH_A_MODE_OFF, 2);
		}
	}
	return ret;
}

static void poll_accelerometer(sensors_event_t *values)
{
//ALOGD("## poll_accelerometer ##") ;
	int fd;
	int nread;
	int data[3];
	char buf[SIZE_OF_BUF];

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

	fd = open(PATH_DATA_ACC, O_RDONLY);

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, SIZE_OF_BUF);

//ALOGD("nread %d", nread ) ;
//	if (nread == sizeof(buf)) {
		sscanf(buf, "(%d,%d,%d)", &data[0], &data[1], &data[2]) ;
//	}
//ALOGD("polled(%d:%d:%d)", data[0], data[1], data[2] ) ;
	values->acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;
	values->acceleration.x = (float) data[0];
	values->acceleration.x *= CONVERT_A;
	values->acceleration.y = (float) data[1];
	values->acceleration.y *= CONVERT_A;
	values->acceleration.z = (float) data[2];
	values->acceleration.z *= CONVERT_A;

	values->type = SENSOR_TYPE_ACCELEROMETER;
	values->sensor = HANDLE_ACCELEROMETER;
	values->version = sizeof(struct sensors_event_t);

	close(fd);
}

static void poll_magnetometer(sensors_event_t *values)
{
	int fd;
	int data[3];
	char buf[SIZE_OF_BUF];
	int nread;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

	fd = open(PATH_DATA_MAG, O_RDONLY);

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, SIZE_OF_BUF);

	//if (nread == sizeof(buf))
	sscanf(buf, "%d %d %d", &data[2], &data[1], &data[0]);

	ALOGD("poll_magnetometer: %i %i %i", data[0], data[1], data[2]);
	values->magnetic.status = SENSOR_STATUS_ACCURACY_HIGH;
	values->magnetic.x = (data[0] * 100) / GAIN_X;
	values->magnetic.y = (data[1] * 100) / GAIN_Y;
	values->magnetic.z = (data[2] * 100) / GAIN_Z;
	values->sensor = HANDLE_MAGNETIC_FIELD;
	values->type = SENSOR_TYPE_MAGNETIC_FIELD;
	values->version = sizeof(struct sensors_event_t);
	close(fd);
}

static void poll_gyroscope(sensors_event_t *values)
{
	int fd;
	int data[3];
	char buf[SIZE_OF_BUF];
	int nread;

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

	fd = open(PATH_DATA_GYR, O_RDONLY);

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, SIZE_OF_BUF);

	if (nread == sizeof(buf))
		sscanf(buf, "%8x:%8x:%8x", &data[0], &data[1], &data[2]);

	values->magnetic.status = SENSOR_STATUS_ACCURACY_HIGH;
	values->gyro.x = (float)data[0];
	values->gyro.x *= DEGREES_TO_RADIANS;
	values->gyro.y = (float)data[1];
	values->gyro.y *= DEGREES_TO_RADIANS;
	values->gyro.z = (float)data[2];
	values->gyro.z *= DEGREES_TO_RADIANS;
	values->sensor = HANDLE_GYROSCOPE;
	values->type = SENSOR_TYPE_GYROSCOPE;
	values->version = sizeof(struct sensors_event_t);
	close(fd);
}

static void poll_orientation(sensors_event_t *values)
{
	int fd_mag;
	int fd_acc;
	int data_mag[3];
	int data_acc[3];
	char buf[SIZE_OF_BUF];
	int nread;
	double mag_x, mag_y, mag_xy;
	double acc_x, acc_y, acc_z;

	data_mag[0] = 0;
	data_mag[1] = 0;
	data_mag[2] = 0;

	data_acc[0] = 0;
	data_acc[1] = 0;
	data_acc[2] = 0;

	fd_acc = open(PATH_DATA_ACC, O_RDONLY);
	fd_mag = open(PATH_DATA_MAG, O_RDONLY);

	memset(buf, 0x00, sizeof(buf));
	lseek(fd_mag, 0, SEEK_SET);
	nread = read(fd_mag, buf, SIZE_OF_BUF);

	if (nread == sizeof(buf))
		sscanf(buf, "%8x:%8x:%8x", &data_mag[0], &data_mag[1], &data_mag[2]);

	mag_x = (data_mag[0] * 100) / GAIN_X;
	mag_y = (data_mag[1] * 100) / GAIN_Y;
	if (mag_x == 0) {
		if (mag_y < 0)
			values->orientation.azimuth = 180;
		else
			values->orientation.azimuth = 0;
	} else {
		mag_xy = mag_y / mag_x;
		if (mag_x > 0)
			values->orientation.azimuth = round(270 + (atan(mag_xy) * RADIANS_TO_DEGREES));
		else
			values->orientation.azimuth = round(90 + (atan(mag_xy) * RADIANS_TO_DEGREES));
	}

	memset(buf, 0x00, sizeof(buf));
	lseek(fd_acc, 0, SEEK_SET);
	nread = read(fd_acc, buf, SIZE_OF_BUF);
	if (nread == sizeof(buf))
		sscanf(buf, "%8x:%8x:%8x", &data_acc[0], &data_acc[1], &data_acc[2]);

	acc_x = (float) data_acc[0];
	acc_x *= CONVERT_A;
	acc_y = (float) data_acc[1];
	acc_y *= CONVERT_A;
	acc_z = (float) data_acc[2];
	acc_z *= CONVERT_A;

	values->sensor = HANDLE_ORIENTATION;
	values->type = SENSOR_TYPE_ORIENTATION;
	values->version = sizeof(struct sensors_event_t);
	values->orientation.status = SENSOR_STATUS_ACCURACY_HIGH;
	values->orientation.pitch = round(atan(acc_y / sqrt(acc_x*acc_x + acc_z*acc_z)) * RADIANS_TO_DEGREES);
	values->orientation.roll = round(atan(acc_x / sqrt(acc_y*acc_y + acc_z*acc_z)) * RADIANS_TO_DEGREES);

	close(fd_acc);
	close(fd_mag);
}

static void poll_light(sensors_event_t *values)
{
	int fd;
	char buf[8];
	float reading;

	fd = open(PATH_DATA_LUX, O_RDONLY);

	/*
	 * events should be immediate but
	 * hardware & os drivers should support it.
	 * let give chnace to self disable and reading
	 * other sensors
	 */
	usleep(1000);
	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, 8);
	reading = atof(buf);

	/* make assignment only when activated and value has changed*/
	if ((last_light_data != reading) && (count_lux)) {
		values->type = SENSOR_TYPE_LIGHT;
		values->sensor = HANDLE_LIGHT;
		values->light = reading;
		last_light_data = reading;
		values->version = sizeof(struct sensors_event_t);
		continue_next = 0;
		events = 1;
	} else{
		continue_next = 1;
		if ((count_delay_sensors==0)&&(count_open_sensors>0))
		events = 0;
	}

	close(fd);
}

void *proximity_getdata()
{
	int fd = -1,retval = -1;
	fd_set read_set;
	struct input_event ev;
	struct timeval tv;
	int size = sizeof(struct input_event);
	/* Initialize the structures */
	memset(&ev, 0x00, sizeof(ev));
	memset(&tv, 0x00, sizeof(tv));
	/* open input device */
	if ((fd = open(PATH_INTR_PROX, O_RDONLY)) > 0) {
		while(!thread_exit) {
			/* Intialize the read descriptor */
			FD_ZERO(&read_set);
			FD_SET(fd,&read_set);
			/* Wait up to 0.5 seconds. */
			tv.tv_sec = 0 ;
			tv.tv_usec = 500000;
			retval = select(fd+1, &read_set, NULL, NULL, &tv);
			if (retval > 0 && count_prox) {
				/* FD_ISSET(0, &rfds) will be true. */
				if (FD_ISSET(fd, &read_set)) {
					read(fd, &ev, size );
					if(11 == ev.code) {
						pthread_mutex_lock( &mutex_proxval );
						stprox_val.prox_flag = 1;
						stprox_val.prox_val = ev.value;
						pthread_mutex_unlock( &mutex_proxval );
					}
				}
			}
		}
		close(fd);
	}
	else
	   ALOGD("\n /dev/input/event0 is not a valid device");
	return NULL;
}

static void poll_proximity(sensors_event_t *values)
{
         pthread_mutex_lock( &mutex_proxval );
         if(stprox_val.prox_flag)
         {
                 stprox_val.prox_flag = 0;
		 /* normalize the distance */
		 if (stprox_val.prox_val == 1)
			values->distance = 0.0f;
		 else
			values->distance = 50.0f;
		 values->sensor = HANDLE_PROXIMITY;
		 values->type = SENSOR_TYPE_PROXIMITY;
		 values->version = sizeof(struct sensors_event_t);
		 continue_next = 0;
		 events = 1;
         }
	 else
	 {
		continue_next = 1;
		if ((count_delay_sensors == 0) && (count_open_sensors > 0))
			events = 0;
	 }
         pthread_mutex_unlock( &mutex_proxval );
}

static int m_open_sensors(const struct hw_module_t *module,
		const char *name, struct hw_device_t **device);

static int m_sensors_get_sensors_list(struct sensors_module_t *module,
		struct sensor_t const **list)
{
	*list = sSensorList;

	return sizeof(sSensorList) / sizeof(sSensorList[0]);
}

static struct hw_module_methods_t m_sensors_module_methods = {
	.open = m_open_sensors
};

struct sensors_module_t HAL_MODULE_INFO_SYM = {
	.common = {
		.tag = HARDWARE_MODULE_TAG,
		.version_major = 1,
		.version_minor = 0,
		.id = SENSORS_HARDWARE_MODULE_ID,
		.name = "Betelgeuse SENSORS Module",
		.author = "Nopy",
		.methods = &m_sensors_module_methods,
	},
	.get_sensors_list = m_sensors_get_sensors_list
};

/* enable and disable sensors here */
static int m_poll_activate(struct sensors_poll_device_t *dev,
		int handle, int enabled)
{
	int status = 0;
	ALOGD("libsensors: Entering function %s with handle = %i, enable = %d\n",
			__FUNCTION__, handle, enabled);

	m_determine_acc_chip();

	switch (handle) {
	case HANDLE_ORIENTATION:
		status = activate_orientation(enabled);
		break;
	case HANDLE_ACCELEROMETER:
		status = activate_acc(enabled);
		break;
	case HANDLE_MAGNETIC_FIELD:
		status = activate_mag(enabled);
		break;
	case HANDLE_GYROSCOPE:
		status = activate_gyr(enabled);
		break;
	case HANDLE_LIGHT:
		status = activate_lux(enabled);
		break;
	case HANDLE_PROXIMITY:
		status = activate_prox(enabled);
		break;
	default:
		ALOGD("libsensors:This sensor/handle is not supported %s\n",
				__FUNCTION__);
		break;
	}

	/* check if sensor is missing then exit gracefully */
	if (status != -ENODEV) {
		/* count total number of sensors open */
		count_open_sensors  = count_acc + count_mag + count_lux + count_prox + count_orien + count_gyr;
		count_delay_sensors = (count_acc?1:0) + (count_mag?1:0) + (count_orien?1:0) + (count_gyr?1:0);

		/* If this is the first sensor activation and previously the lock was held
		   unlock it now. Else if no sensor is active and the lock was not held
		   previously, lock it */
		if (count_open_sensors == 1 && lockisheld == 1) {
			pthread_mutex_unlock(&count_mutex);
			pthread_cond_signal(&count_threshold_cv);
			lockisheld = 0;
		} else if (count_open_sensors == 0 && lockisheld == 0) {
			pthread_mutex_lock(&count_mutex);
			lockisheld = 1;
		}
	}

	return 0;
}

static int m_poll_set_delay(struct sensors_poll_device_t *dev,
		int handle, int64_t ns)
{
	int microseconds = ns / 1000;

	ALOGD("libsensors: set delay = %d in microseconds\n", microseconds);

	switch (handle) {
	case HANDLE_ORIENTATION:
		if (microseconds >= MINDELAY_ORIENTATION)
			delay_orien = microseconds;
		break;
	case HANDLE_ACCELEROMETER:
		if (microseconds >= MINDELAY_ACCELEROMETER)
			delay_acc = microseconds;
		break;
	case HANDLE_MAGNETIC_FIELD:
		if (microseconds >= MINDELAY_MAGNETIC_FIELD)
			delay_mag = microseconds;
		break;
	case HANDLE_GYROSCOPE:
		if (microseconds >= MINDELAY_GYROSCOPE)
			delay_gyr = microseconds;
		break;
	case HANDLE_LIGHT:
		/* ignored */
		break;
	case HANDLE_PROXIMITY:
		/* ignored */
		break;
	default:
		ALOGD("libsensors:This sensor/handle is not supported %s\n",
				__FUNCTION__);
		break;
	}

	return 0;
}


static int m_poll(struct sensors_poll_device_t *dev,
		sensors_event_t *data, int count)
{
	int i;
	struct timeval time;
	continue_next = 0;
	events = 0;

	pthread_mutex_lock(&count_mutex);

    if (count_open_sensors == 0)
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
	events++;

	for (i = 0; i < HANDLE_MAX; i++) {
		int index = (i + last_polled + 1) % HANDLE_MAX;

		/* if there is any active sensors active */
		switch (index) {
		case HANDLE_ACCELEROMETER:
			if (count_acc > 0) {
				poll_accelerometer(data);
				if (count_delay_sensors != 0)
					usleep(delay_acc/count_delay_sensors);
				continue_next = 0;
			} else
				continue_next = 1;
			break;
		case HANDLE_MAGNETIC_FIELD:
			if (count_mag > 0) {
				poll_magnetometer(data);
				if (count_delay_sensors != 0)
					usleep(delay_mag/count_delay_sensors);
				continue_next = 0;
			} else
				continue_next = 1;
			break;
		case HANDLE_ORIENTATION:
			if (count_orien > 0) {
				poll_orientation(data);
				if (count_delay_sensors != 0)
					usleep(delay_orien/count_delay_sensors);
				continue_next = 0;
			} else
				continue_next = 1;
			break;
		case HANDLE_GYROSCOPE:
			if (count_gyr > 0) {
				poll_gyroscope(data);
				if (count_delay_sensors!=0)
					usleep(delay_gyr/count_delay_sensors);
				continue_next = 0;
			} else
				continue_next = 1;
			break;
		case HANDLE_LIGHT:
			if (count_lux > 0) {
				poll_light(data);
			} else
				continue_next = 1;
			break;
		case HANDLE_PRESSURE:
			continue_next = 1;
			break;
		case HANDLE_TEMPERATURE:
			continue_next = 1;
			break;
		case HANDLE_PROXIMITY:
			if (count_prox > 0) {
				poll_proximity(data);
			} else
				continue_next = 1;
			break;
		default:
			continue_next = 1;
			break;
		}
		if (0 == continue_next) {
			last_polled = index;
			break;
		}
	}

	/* add time stamp on last event */
	gettimeofday(&time, NULL);
	data->timestamp = (time.tv_sec * 1000000000LL) + (time.tv_usec * 1000);

	pthread_mutex_unlock(&count_mutex);
	return events;
}

/* close instace of the deevie */
static int m_poll_close(struct hw_device_t *dev)
{
	struct sensors_poll_device_t *poll_device =
		(struct sensors_poll_device_t *) dev;

	ALOGD("libsensors: Closing poll data context.\n");

	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);

	if (poll_device)
		free(poll_device);
	return 0;
}

static void m_determine_acc_chip()
{
	int fd;
	int nread;
	char buf[4];
	acc_id = 50;

/*	fd = open(PATH_ID_ACC, O_RDONLY);

	memset(buf, 0x00, sizeof(buf));
	lseek(fd, 0, SEEK_SET);
	nread = read(fd, buf, sizeof(buf));
	sscanf(buf, "%d", &acc_id);
	close(fd);*/
}

/* open a new instance of a sensor device using name */
static int m_open_sensors(const struct hw_module_t *module,
		const char *name, struct hw_device_t **device)
{
	ALOGD("libsensors: Entering function %s with param name = %s\n",
			__FUNCTION__, name);

	int status = -EINVAL;

	if (!strcmp(name, SENSORS_HARDWARE_POLL)) {
		struct sensors_poll_device_t *poll_device;
		poll_device = malloc(sizeof(*poll_device));
		if (!poll_device)
			return status;
		memset(poll_device, 0, sizeof(*poll_device));
		poll_device->common.tag = HARDWARE_DEVICE_TAG;
		poll_device->common.version = 0;
		poll_device->common.module = (struct hw_module_t *) module;
		poll_device->common.close = m_poll_close;
		poll_device->activate = m_poll_activate;
		poll_device->setDelay = m_poll_set_delay;
		poll_device->poll = m_poll;
		*device = &poll_device->common;

		pthread_mutex_init(&count_mutex, NULL);
		pthread_cond_init (&count_threshold_cv, NULL);
		pthread_mutex_init(&mutex_proxval, NULL);

		status = 0;
	}
	return status;
}

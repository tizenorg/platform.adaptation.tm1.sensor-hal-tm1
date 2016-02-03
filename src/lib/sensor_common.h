/*
 * libsensord-share
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __SENSOR_COMMON_H__
#define __SENSOR_COMMON_H__

#include <unistd.h>
#include <sensor_logs.h>
#include <string>
#include <vector>

#ifndef DEPRECATED
#define DEPRECATED __attribute__((deprecated))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup SENSOR_FRAMEWORK SensorFW
 * To support the unified API for the various sensors
 */

/**
 * @defgroup SENSOR_FRAMEWORK_COMMON Sensor Framework Common API
 * @ingroup SENSOR_FRAMEWORK
 *
 * These APIs are used to control the sensors.
 * @{
 */

typedef enum {
	ALL_SENSOR = -1,
	UNKNOWN_SENSOR = 0,
	ACCELEROMETER_SENSOR,
	GEOMAGNETIC_SENSOR,
	LIGHT_SENSOR,
	PROXIMITY_SENSOR,
	THERMOMETER_SENSOR,
	GYROSCOPE_SENSOR,
	PRESSURE_SENSOR,
	MOTION_SENSOR,
	FUSION_SENSOR,
	PEDOMETER_SENSOR,
	CONTEXT_SENSOR,
	FLAT_SENSOR,
	BIO_SENSOR,
	BIO_HRM_SENSOR,
	AUTO_ROTATION_SENSOR,
	GRAVITY_SENSOR,
	LINEAR_ACCEL_SENSOR,
	ROTATION_VECTOR_SENSOR,
	GEOMAGNETIC_RV_SENSOR,
	GAMING_RV_SENSOR,
	ORIENTATION_SENSOR,
	TILT_SENSOR,
	PIR_SENSOR,
	PIR_LONG_SENSOR,
	TEMPERATURE_SENSOR,
	HUMIDITY_SENSOR,
	ULTRAVIOLET_SENSOR,
	DUST_SENSOR,
	BIO_LED_GREEN_SENSOR,
	BIO_LED_IR_SENSOR,
	BIO_LED_RED_SENSOR,
	RV_RAW_SENSOR,
	UNCAL_GYROSCOPE_SENSOR,
	UNCAL_GEOMAGNETIC_SENSOR
} sensor_type_t;

typedef unsigned int sensor_id_t;

typedef void *sensor_t;

typedef enum {
	SENSOR_PRIVILEGE_PUBLIC,
	SENSOR_PRIVILEGE_INTERNAL,
} sensor_privilege_t;

#define SENSOR_DATA_VALUE_SIZE 16

/*
 *	When modifying it, check copy_sensor_data()
 */
typedef struct sensor_data_t {
/*
 * 	Use "accuracy" instead of "data_accuracy"
 * 	which is going to be removed soon
 */
	union {
		int accuracy;
		int data_accuracy; //deprecated
	};

	union {
		unsigned long long timestamp;
		unsigned long long time_stamp; //deprecated
	};

/*
 * 	Use "value_count" instead of "values_num"
 * 	which is going to be removed soon
 */
	union {
		int value_count;
		int values_num; //deprecated
	};

	float values[SENSOR_DATA_VALUE_SIZE];

/*
 * 	If extra_data_size > 0,
 * 	then use extra_data.
 */
	int extra_data_size;
	void *extra_data;
} sensor_data_t;

#define SENSOR_HUB_DATA_SIZE	4096

typedef struct sensorhub_data_t {
    int version;
    int sensorhub;
    int type;
    int hub_data_size;
    unsigned long long timestamp;
    char hub_data[SENSOR_HUB_DATA_SIZE];
    float data[16];
} sensorhub_data_t;

enum sensor_accuracy_t {
	SENSOR_ACCURACY_UNDEFINED = -1,
	SENSOR_ACCURACY_BAD = 0,
	SENSOR_ACCURACY_NORMAL =1,
	SENSOR_ACCURACY_GOOD = 2,
	SENSOR_ACCURACY_VERYGOOD = 3
};

/*
 *	To prevent naming confliction as using same enums as sensor CAPI use
 */
#ifndef __SENSOR_H__
enum sensor_option_t {
	SENSOR_OPTION_DEFAULT = 0,
	SENSOR_OPTION_ON_IN_SCREEN_OFF = 1,
	SENSOR_OPTION_ON_IN_POWERSAVE_MODE = 2,
	SENSOR_OPTION_ALWAYS_ON = SENSOR_OPTION_ON_IN_SCREEN_OFF | SENSOR_OPTION_ON_IN_POWERSAVE_MODE,
	SENSOR_OPTION_END
};

typedef enum sensor_option_t sensor_option_e;
#endif

/*
 *	To prevent naming confliction as using same enums as sensor CAPI use
 */
#ifndef __SENSOR_H__
enum sensor_wakeup_t {
	SENSOR_WAKEUP_UNKNOWN = -1,
	SENSOR_WAKEUP_OFF = 0,
	SENSOR_WAKEUP_ON = 1,
};

typedef enum sensor_wakeup_t sensor_wakeup_e;
#endif

enum sensor_interval_t {
	SENSOR_INTERVAL_FASTEST = 0,
	SENSOR_INTERVAL_NORMAL = 200,
};

typedef enum {
	CONDITION_NO_OP,
	CONDITION_EQUAL,
	CONDITION_GREAT_THAN,
	CONDITION_LESS_THAN,
} condition_op_t;

#define COMMAND_CHANNEL_PATH			"/tmp/sensord_command_socket"
#define EVENT_CHANNEL_PATH				"/tmp/sensord_event_socket"

#define MAX_HANDLE			256
#define MAX_HANDLE_REACHED	-2

#define CLIENT_ID_INVALID   -1

#define SENSOR_TYPE_MASK  0xFFFF

enum packet_type_t {
	CMD_NONE = 0,
	CMD_GET_ID,
	CMD_GET_SENSOR_LIST,
	CMD_HELLO,
	CMD_BYEBYE,
	CMD_DONE,
	CMD_START,
	CMD_STOP,
	CMD_REG,
	CMD_UNREG,
	CMD_SET_OPTION,
	CMD_SET_WAKEUP,
	CMD_SET_BATCH,
	CMD_UNSET_BATCH,
	CMD_SET_COMMAND,
	CMD_GET_DATA,
	CMD_SEND_SENSORHUB_DATA,
	CMD_CNT,
};

enum sensor_state_t {
	SENSOR_STATE_UNKNOWN = -1,
	SENSOR_STATE_STOPPED = 0,
	SENSOR_STATE_STARTED = 1,
	SENSOR_STATE_PAUSED = 2
};

enum poll_interval_t {
	POLL_100HZ_MS	= 10,
	POLL_50HZ_MS	= 20,
	POLL_25HZ_MS	= 40,
	POLL_20HZ_MS	= 50,
	POLL_10HZ_MS	= 100,
	POLL_5HZ_MS		= 200,
	POLL_1HZ_MS		= 1000,
	POLL_MAX_HZ_MS  = POLL_1HZ_MS,
};

typedef struct {
	char name[NAME_MAX];
} cmd_get_id_t;

typedef struct {
} cmd_get_sensor_list_t;

typedef struct {
	int client_id;
	int sensor;
} cmd_hello_t;

typedef struct {
} cmd_byebye_t;


typedef struct {
	unsigned int type;
} cmd_get_data_t;

typedef struct {
	long value;
} cmd_done_t;


typedef struct {
	int client_id;
} cmd_get_id_done_t;

typedef struct {
	int sensor_cnt;
	char data[0];
} cmd_get_sensor_list_done_t;

typedef struct {
	int state;
	sensor_data_t base_data;
} cmd_get_data_done_t;

typedef struct {
} cmd_start_t;

typedef struct {
} cmd_stop_t;

typedef struct {
	unsigned int event_type;
} cmd_reg_t;

typedef struct {
	unsigned int event_type;
} cmd_unreg_t;

typedef struct {
	unsigned int interval;
	unsigned int latency;
} cmd_set_batch_t;

typedef struct {
} cmd_unset_batch_t;

typedef struct {
	int option;
} cmd_set_option_t;

typedef struct {
	int wakeup;
} cmd_set_wakeup_t;

typedef struct  {
	unsigned int cmd;
	long value;
} cmd_set_command_t;

typedef struct  {
	int data_len;
	char data[0];
} cmd_send_sensorhub_data_t;

#define EVENT_CHANNEL_MAGIC 0xCAFECAFE

typedef struct {
	unsigned int magic;
	int client_id;
} event_channel_ready_t;

typedef struct {
	std::string name;
	std::string vendor;
	float min_range;
	float max_range;
	float resolution;
	int min_interval;
	int fifo_count;
	int max_batch_count;
	bool wakeup_supported;
} sensor_properties_s;

/*
 * When modifying it, check copy_sensor*_data()
 */
typedef struct sensor_event_t {
	unsigned int event_type;
	sensor_id_t sensor_id;
	sensor_data_t data;
} sensor_event_t;

typedef struct sensorhub_event_t {
	unsigned int event_type;
	sensor_id_t sensor_id;
	sensorhub_data_t data;
} sensorhub_event_t;

typedef struct {
	std::vector<void*> sensors;
} sensor_module;

typedef sensor_module* (*create_t)(void);

typedef void *(*cmd_func_t)(void *data, void *cb_data);

typedef std::vector<unsigned int> event_type_vector;

enum sensorhub_enable_bit {
	SENSORHUB_ACCELEROMETER_ENABLE_BIT = 0,
	SENSORHUB_GYROSCOPE_ENABLE_BIT,
	SENSORHUB_GEOMAGNETIC_UNCALIB_ENABLE_BIT,
	SENSORHUB_GEOMAGNETIC_RAW_ENABLE_BIT,
	SENSORHUB_GEOMAGNETIC_ENABLE_BIT,
	SENSORHUB_PRESSURE_ENABLE_BIT,
	SENSORHUB_GESTURE_ENABLE_BIT,
	SENSORHUB_PROXIMITY_ENABLE_BIT,
	SENSORHUB_TEMPERATURE_HUMIDITY_ENABLE_BIT,
	SENSORHUB_LIGHT_ENABLE_BIT,
	SENSORHUB_PROXIMITY_RAW_ENABLE_BIT,
	SENSORHUB_ORIENTATION_ENABLE_BIT,
	SENSORHUB_STEP_DETECTOR_ENABLE_BIT = 12,
	SENSORHUB_SIG_MOTION_ENABLE_BIT,
	SENSORHUB_GYRO_UNCALIB_ENABLE_BIT,
	SENSORHUB_GAME_ROTATION_VECTOR_ENABLE_BIT = 15,
	SENSORHUB_ROTATION_VECTOR_ENABLE_BIT,
	SENSORHUB_STEP_COUNTER_ENABLE_BIT,
	SENSORHUB_BIO_HRM_RAW_ENABLE_BIT,
	SENSORHUB_BIO_HRM_RAW_FAC_ENABLE_BIT,
	SENSORHUB_BIO_HRM_LIB_ENABLE_BIT,
	SENSORHUB_TILT_MOTION,
	SENSORHUB_UV_SENSOR,
	SENSORHUB_PIR_ENABLE_BIT,
	SENSORHUB_ENABLE_BIT_MAX,
};

enum sensor_permission_t {
	SENSOR_PERMISSION_NONE	= 0,
	SENSOR_PERMISSION_STANDARD = (1 << 0),
	SENSOR_PERMISSION_BIO	=  (1 << 1),
};

#define BIO_SENSOR_PRIVELEGE_NAME "sensord::bio"
#define BIO_SENSOR_ACCESS_RIGHT "rw"

#ifdef __cplusplus
}
#endif

#endif
//! End of a file

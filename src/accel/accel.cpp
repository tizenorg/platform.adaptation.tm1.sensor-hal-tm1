/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <util.h>
#include <sensor_logs.h>

#include "accel.h"

#define GRAVITY 9.80665
#define G_TO_MG 1000
#define RAW_DATA_TO_G_UNIT(X) (((float)(X))/((float)G_TO_MG))
#define RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(X) (GRAVITY * (RAW_DATA_TO_G_UNIT(X)))

#define MIN_RANGE(RES) (-((1 << (RES))/2))
#define MAX_RANGE(RES) (((1 << (RES))/2)-1)

#define MODEL_NAME "K2HH"
#define VENDOR "ST Microelectronics"
#define RESOLUTION 16
#define RAW_DATA_UNIT 0.122
#define MIN_INTERVAL 1
#define MAX_BATCH_COUNT 0

#define SENSORHUB_ACCELEROMETER_ENABLE_BIT 0

static const sensor_info_t sensor_info = {
	id: 0x1,
	name: "Accelerometer",
	type: SENSOR_DEVICE_ACCELEROMETER,
	event_type: (SENSOR_DEVICE_ACCELEROMETER << 16) | 0x0001,
	model_name: MODEL_NAME,
	vendor: VENDOR,
	min_range: MIN_RANGE(RESOLUTION) * RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(RAW_DATA_UNIT),
	max_range: MAX_RANGE(RESOLUTION) * RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(RAW_DATA_UNIT),
	resolution: RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(RAW_DATA_UNIT),
	min_interval: MIN_INTERVAL,
	max_batch_count: MAX_BATCH_COUNT,
	wakeup_supported: false
};

std::vector<uint32_t> accel_device::event_ids;

accel_device::accel_device()
: m_node_handle(-1)
, m_x(-1)
, m_y(-1)
, m_z(-1)
, m_polling_interval(0)
, m_fired_time(0)
, m_sensorhub_controlled(false)
{
	const std::string sensorhub_interval_node_name = "accel_poll_delay";

	node_info_query query;
	node_info info;

	query.sensorhub_controlled = m_sensorhub_controlled = util::is_sensorhub_controlled(sensorhub_interval_node_name);
	query.sensor_type = "ACCEL";
	query.key = "accelerometer_sensor";
	query.iio_enable_node_name = "accel_enable";
	query.sensorhub_interval_node_name = sensorhub_interval_node_name;

	if (!util::get_node_info(query, info)) {
		ERR("Failed to get node info");
		throw ENXIO;
	}

	util::show_node_info(info);

	m_data_node = info.data_node_path;
	m_enable_node = info.enable_node_path;
	m_interval_node = info.interval_node_path;

	if ((m_node_handle = open(m_data_node.c_str(), O_RDWR)) < 0) {
		_ERRNO(errno, _E, "accel handle open fail for accel processor");
		throw ENXIO;
	}

	INFO("accel_device is created!\n");
}

accel_device::~accel_device()
{
	close(m_node_handle);
	m_node_handle = -1;

	INFO("accel_device is destroyed!\n");
}

int accel_device::get_poll_fd()
{
	return m_node_handle;
}

int accel_device::get_sensors(const sensor_info_t **sensors)
{
	*sensors = &sensor_info;

	return 1;
}

bool accel_device::enable(uint32_t id)
{
	util::set_enable_node(m_enable_node, m_sensorhub_controlled, true, SENSORHUB_ACCELEROMETER_ENABLE_BIT);
	set_interval(id, m_polling_interval);

	m_fired_time = 0;
	INFO("Enable accelerometer sensor");
	return true;
}

bool accel_device::disable(uint32_t id)
{
	util::set_enable_node(m_enable_node, m_sensorhub_controlled, false, SENSORHUB_ACCELEROMETER_ENABLE_BIT);

	INFO("Disable accelerometer sensor");
	return true;
}

bool accel_device::set_interval(uint32_t id, unsigned long val)
{
	unsigned long long polling_interval_ns;

	polling_interval_ns = ((unsigned long long)(val) * 1000llu * 1000llu);

	if (!util::set_node_value(m_interval_node, polling_interval_ns)) {
		ERR("Failed to set polling resource: %s\n", m_interval_node.c_str());
		return false;
	}

	INFO("Interval is changed from %dms to %dms]", m_polling_interval, val);
	m_polling_interval = val;
	return true;
}

bool accel_device::set_batch_latency(uint32_t id, unsigned long val)
{
	return false;
}

bool accel_device::set_attribute_int(uint32_t id, int32_t attribute, int32_t value)
{
	return false;
}

bool accel_device::set_attribute_str(uint32_t id, int32_t attribute, char *value, int value_len)
{
	return false;
}

bool accel_device::update_value_input_event(void)
{
	int accel_raw[3] = {0,};
	bool x,y,z;
	int read_input_cnt = 0;
	const int INPUT_MAX_BEFORE_SYN = 10;
	unsigned long long fired_time = 0;
	bool syn = false;

	x = y = z = false;

	struct input_event accel_input;
	DBG("accel event detection!");

	while ((syn == false) && (read_input_cnt < INPUT_MAX_BEFORE_SYN)) {
		int len = read(m_node_handle, &accel_input, sizeof(accel_input));
		if (len != sizeof(accel_input)) {
			ERR("accel_file read fail, read_len = %d\n",len);
			return false;
		}

		++read_input_cnt;

		if (accel_input.type == EV_REL) {
			switch (accel_input.code) {
			case REL_X:
				accel_raw[0] = (int)accel_input.value;
				x = true;
				break;
			case REL_Y:
				accel_raw[1] = (int)accel_input.value;
				y = true;
				break;
			case REL_Z:
				accel_raw[2] = (int)accel_input.value;
				z = true;
				break;
			default:
				ERR("accel_input event[type = %d, code = %d] is unknown.", accel_input.type, accel_input.code);
				return false;
				break;
			}
		} else if (accel_input.type == EV_SYN) {
			syn = true;
			fired_time = util::get_timestamp(&accel_input.time);
		} else {
			ERR("accel_input event[type = %d, code = %d] is unknown.", accel_input.type, accel_input.code);
			return false;
		}
	}

	if (syn == false) {
		ERR("EV_SYN didn't come until %d inputs had come", read_input_cnt);
		return false;
	}

	if (x)
		m_x =  accel_raw[0];
	if (y)
		m_y =  accel_raw[1];
	if (z)
		m_z =  accel_raw[2];

	m_fired_time = fired_time;

	DBG("m_x = %d, m_y = %d, m_z = %d, time = %lluus", m_x, m_y, m_z, m_fired_time);

	return true;
}

int accel_device::read_fd(uint32_t **ids)
{
	if (!update_value_input_event()) {
		DBG("Failed to update value");
		return false;
	}

	event_ids.clear();
	event_ids.push_back(sensor_info.id);

	*ids = &event_ids[0];

	return event_ids.size();
}

int accel_device::get_data(uint32_t id, sensor_data_t **data, int *length)
{
	int remains = 1;
	sensor_data_t *sensor_data;
	sensor_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));

	sensor_data->accuracy = SENSOR_ACCURACY_GOOD;
	sensor_data->timestamp = m_fired_time;
	sensor_data->value_count = 3;
	sensor_data->values[0] = m_x;
	sensor_data->values[1] = m_y;
	sensor_data->values[2] = m_z;

	raw_to_base(sensor_data);

	*data = sensor_data;
	*length = sizeof(sensor_data_t);

	return --remains;
}

bool accel_device::flush(uint32_t id)
{
	return false;
}

void accel_device::raw_to_base(sensor_data_t *data)
{
	data->value_count = 3;
	data->values[0] = RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(data->values[0] * RAW_DATA_UNIT);
	data->values[1] = RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(data->values[1] * RAW_DATA_UNIT);
	data->values[2] = RAW_DATA_TO_METRE_PER_SECOND_SQUARED_UNIT(data->values[2] * RAW_DATA_UNIT);
}

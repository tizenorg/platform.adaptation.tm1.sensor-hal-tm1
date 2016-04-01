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

#include "proxi.h"

#define MODEL_NAME "IMS1911"
#define VENDOR "ITM"
#define MIN_RANGE 0
#define MAX_RANGE 5
#define RESOLUTION 1
#define MIN_INTERVAL 1
#define MAX_BATCH_COUNT 0

#define SENSORHUB_PROXIMITY_ENABLE_BIT 7

#define RAW_DATA_TO_DISTANCE(x) ((x) * 5)

static const sensor_info_t sensor_info = {
	id: 0x1,
	name: "Proximity Sensor",
	type: SENSOR_DEVICE_PROXIMITY,
	event_type: (SENSOR_DEVICE_PROXIMITY << 16) | 0x0001,
	model_name: MODEL_NAME,
	vendor: VENDOR,
	min_range: MIN_RANGE,
	max_range: MAX_RANGE,
	resolution: RESOLUTION,
	min_interval: MIN_INTERVAL,
	max_batch_count: MAX_BATCH_COUNT,
	wakeup_supported: false
};

std::vector<uint32_t> proxi_device::event_ids;

proxi_device::proxi_device()
: m_node_handle(-1)
, m_state(-1)
, m_fired_time(0)
, m_sensorhub_controlled(false)
{
	const std::string sensorhub_interval_node_name = "prox_poll_delay";

	node_info_query query;
	node_info info;

	query.sensorhub_controlled = m_sensorhub_controlled = util::is_sensorhub_controlled(sensorhub_interval_node_name);
	query.sensor_type = "PROXI";
	query.key = "proximity_sensor";
	query.iio_enable_node_name = "proximity_enable";
	query.sensorhub_interval_node_name = sensorhub_interval_node_name;

	if (!util::get_node_info(query, info)) {
		ERR("Failed to get node info");
		throw ENXIO;
	}

	util::show_node_info(info);

	m_data_node = info.data_node_path;
	m_enable_node = info.enable_node_path;

	if ((m_node_handle = open(m_data_node.c_str(), O_RDWR)) < 0) {
		_ERRNO(errno, _E, "proxi handle open fail for proxi device");
		throw ENXIO;
	}

	INFO("proxi_device is created!");
}

proxi_device::~proxi_device()
{
	close(m_node_handle);
	m_node_handle = -1;

	INFO("proxi_device is destroyed!");
}

int proxi_device::get_poll_fd()
{
	return m_node_handle;
}

int proxi_device::get_sensors(const sensor_info_t **sensors)
{
	*sensors = &sensor_info;

	return 1;
}

bool proxi_device::enable(uint32_t id)
{
	util::set_enable_node(m_enable_node, m_sensorhub_controlled, true, SENSORHUB_PROXIMITY_ENABLE_BIT);

	m_fired_time = 0;
	INFO("Enable proximity sensor");
	return true;
}

bool proxi_device::disable(uint32_t id)
{
	util::set_enable_node(m_enable_node, m_sensorhub_controlled, false, SENSORHUB_PROXIMITY_ENABLE_BIT);

	INFO("Disable proximity sensor");
	return true;
}

bool proxi_device::set_interval(uint32_t id, unsigned long interval_ms)
{
	return true;
}

bool proxi_device::set_batch_latency(uint32_t id, unsigned long val)
{
	return false;
}

bool proxi_device::set_attribute_int(uint32_t id, int32_t attribute, int32_t value)
{
	return false;
}

bool proxi_device::set_attribute_str(uint32_t id, int32_t attribute, char *value, int value_len)
{
	return false;
}

bool proxi_device::update_value_input_event(void)
{
	struct input_event proxi_event;
	DBG("proxi event detection!");

	int len = read(m_node_handle, &proxi_event, sizeof(proxi_event));

	if (len == -1) {
		_ERRNO(errno, _D, "Failed to read from m_node_handle[%d]", m_node_handle);
		return false;
	}

	if ((proxi_event.type == EV_ABS) && (proxi_event.code == ABS_DISTANCE)) {
		m_state = proxi_event.value;
		m_fired_time = util::get_timestamp(&proxi_event.time);

		DBG("m_state = %d, time = %lluus", m_state, m_fired_time);

		return true;
	}

	return false;
}

int proxi_device::read_fd(uint32_t **ids)
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

int proxi_device::get_data(uint32_t id, sensor_data_t **data, int *length)
{
	int remains = 1;
	sensor_data_t *sensor_data;
	sensor_data = (sensor_data_t *)malloc(sizeof(sensor_data_t));

	sensor_data->accuracy = SENSOR_ACCURACY_GOOD;
	sensor_data->timestamp = m_fired_time;
	sensor_data->value_count = 1;
	sensor_data->values[0] = RAW_DATA_TO_DISTANCE(m_state);

	*data = sensor_data;
	*length = sizeof(sensor_data_t);

	return --remains;
}

bool proxi_device::flush(uint32_t id)
{
	return false;
}

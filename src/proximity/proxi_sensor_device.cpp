/*
 * proxi_sensor_device
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
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/input.h>
#include "proxi_sensor_device.h"

#define MODEL_NAME "K2HH"
#define VENDOR "ST Microelectronics"
#define MIN_RANGE 0
#define MAX_RANGE 5
#define RESOLUTION 1
#define MIN_INTERVAL 1
#define FIFO_COUNT 0
#define MAX_BATCH_COUNT 0

static const sensor_properties_s proxi_properties = {
	name : MODEL_NAME,
	vendor : VENDOR,
	min_range : MIN_RANGE,
	max_range : MAX_RANGE,
	resolution : RESOLUTION,
	min_interval : MIN_INTERVAL,
	fifo_count : FIFO_COUNT,
	max_batch_count : MAX_BATCH_COUNT,
};

static const sensor_handle_t handles[] = {
	{
		id: 0x1,
		name: "Proximity Sensor",
		type: SENSOR_DEVICE_PROXIMITY,
		event_type: (SENSOR_DEVICE_PROXIMITY << 16) | 0x0001,
		properties : proxi_properties
	}
};

proxi_sensor_device::proxi_sensor_device()
: m_node_handle(-1)
, m_state(-1)
, m_fired_time(0)
, m_sensorhub_controlled(false)
{
	const std::string sensorhub_interval_node_name = "prox_poll_delay";

	node_info_query query;
	node_info info;

	query.sensorhub_controlled = m_sensorhub_controlled = is_sensorhub_controlled(sensorhub_interval_node_name);
	query.sensor_type = "PROXI";
	query.key = "proximity_sensor";
	query.iio_enable_node_name = "proximity_enable";
	query.sensorhub_interval_node_name = sensorhub_interval_node_name;

	if (!get_node_info(query, info)) {
		ERR("Failed to get node info");
		throw ENXIO;
	}

	show_node_info(info);

	m_data_node = info.data_node_path;
	m_enable_node = info.enable_node_path;

	if ((m_node_handle = open(m_data_node.c_str(), O_RDWR)) < 0) {
		ERR("accel handle open fail for accel processor, error:%s\n", strerror(errno));
		throw ENXIO;
	}

	INFO("Proxi_sensor_device is created!\n");
}

proxi_sensor_device::~proxi_sensor_device()
{
	close(m_node_handle);
	m_node_handle = -1;

	INFO("Proxi_sensor_device is destroyed!\n");
}

bool proxi_sensor_device::get_sensors(std::vector<sensor_handle_t> &sensors)
{
	int size = ARRAY_SIZE(handles);

	for (int i = 0; i < size; ++i)
		sensors.push_back(handles[i]);

	return true;
}

bool proxi_sensor_device::enable(uint32_t id)
{
	set_enable_node(m_enable_node, m_sensorhub_controlled, true, SENSORHUB_PROXIMITY_ENABLE_BIT);

	m_fired_time = 0;
	INFO("Enable proximity sensor");
	return true;
}

bool proxi_sensor_device::disable(uint32_t id)
{
	set_enable_node(m_enable_node, m_sensorhub_controlled, false, SENSORHUB_PROXIMITY_ENABLE_BIT);

	INFO("Disable proximity sensor");
	return true;
}

int proxi_sensor_device::get_poll_fd()
{
	return m_node_handle;
}

bool proxi_sensor_device::set_interval(uint32_t id, unsigned long interval_ms)
{
	return true;
}

bool proxi_sensor_device::set_batch_latency(uint32_t id, unsigned long val)
{
	return false;
}

bool proxi_sensor_device::set_command(uint32_t id, std::string command, std::string value)
{
	return false;
}

bool proxi_sensor_device::is_data_ready(void)
{
	bool ret;
	ret = update_value();
	return ret;
}

bool proxi_sensor_device::update_value(void)
{
	struct input_event proxi_event;
	DBG("proxi event detection!");

	int len = read(m_node_handle, &proxi_event, sizeof(proxi_event));

	if (len == -1) {
		DBG("read(m_node_handle) is error:%s.\n", strerror(errno));
		return false;
	}

	if ((proxi_event.type == EV_ABS) && (proxi_event.code == ABS_DISTANCE)) {
		m_state = proxi_event.value;
		m_fired_time = sensor_device_base::get_timestamp(&proxi_event.time);

		DBG("m_state = %d, time = %lluus", m_state, m_fired_time);

		return true;
	}

	return false;
}

bool proxi_sensor_device::get_sensor_data(uint32_t id, sensor_data_t &data)
{
	data.accuracy = SENSOR_ACCURACY_UNDEFINED;
	data.timestamp = m_fired_time;
	data.value_count = 1;
	data.values[0] = m_state;

	return true;
}

int proxi_sensor_device::get_sensor_event(uint32_t id, sensor_event_t **event)
{
	sensor_event_t *sensor_event;
	sensor_event = (sensor_event_t *)malloc(sizeof(sensor_event_t));

	sensor_event->data.accuracy = SENSOR_ACCURACY_GOOD;
	sensor_event->data.timestamp = m_fired_time;
	sensor_event->data.value_count = 1;
	sensor_event->data.values[0] = m_state;

	*event = sensor_event;

	return sizeof(sensor_event_t);
}

bool proxi_sensor_device::get_properties(uint32_t id, sensor_properties_s &properties)
{
	properties.name = MODEL_NAME;
	properties.vendor = VENDOR;
	properties.min_range = proxi_properties.min_range;
	properties.max_range = proxi_properties.max_range;
	properties.min_interval = proxi_properties.min_interval;
	properties.resolution = proxi_properties.resolution;
	properties.fifo_count = proxi_properties.fifo_count;
	properties.max_batch_count = proxi_properties.max_batch_count;
	return true;
}

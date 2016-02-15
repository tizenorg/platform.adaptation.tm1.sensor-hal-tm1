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

#include <algorithm>
#include <sensor_logs.h>

#include "sensorhub.h"
#include "sensorhub_controller.h"
#include "sensorhub_manager.h"
#include "system_state.h"

sensorhub_device::sensorhub_device()
{
	controller = &sensorhub_controller::get_instance();
	if (!controller) {
		ERR("Failed to allocated memory");
		throw;
	}

	manager = &sensorhub_manager::get_instance();
	if (!manager) {
		ERR("Failed to allocated memory");
		throw;
	}
	manager->set_controller(controller);
	system_state_handler::get_instance().set_controller(controller);

	INFO("sensorhub_device is created!");
}

sensorhub_device::~sensorhub_device()
{
	INFO("sensorhub_device is destroyed!");
}

int sensorhub_device::get_poll_fd(void)
{
	return controller->get_poll_fd();
}

int sensorhub_device::get_sensors(const sensor_info_t **sensors)
{
	int size;
	size = manager->get_sensors(sensors);

	return size;
}

bool sensorhub_device::enable(uint32_t id)
{
	system_state_handler::get_instance().initialize();

	controller->enable();
	sensorhub_sensor *sensor = manager->get_sensor(id);

	if (!sensor) {
		ERR("Failed to enable sensor(0x%x)", id);
		return false;
	}

	return sensor->enable();
}

bool sensorhub_device::disable(uint32_t id)
{
	system_state_handler::get_instance().finalize();

	controller->disable();
	sensorhub_sensor *sensor = manager->get_sensor(id);

	if (!sensor) {
		ERR("Failed to disable sensor(0x%x)", id);
		return false;
	}

	return sensor->disable();
}

bool sensorhub_device::set_interval(uint32_t id, unsigned long val)
{
	sensorhub_sensor *sensor = manager->get_sensor(id);

	if (!sensor) {
		ERR("Failed to set interval to sensor(0x%x)", id);
		return false;
	}

	return sensor->set_interval(val);
}

bool sensorhub_device::set_batch_latency(uint32_t id, unsigned long val)
{
	sensorhub_sensor *sensor = manager->get_sensor(id);

	if (!sensor) {
		ERR("Failed to set batch latency to sensor(0x%x)", id);
		return false;
	}

	return sensor->set_batch_latency(val);
}

bool sensorhub_device::set_attribute_int(uint32_t id, int32_t attribute, int32_t value)
{
	int ret;

	sensorhub_sensor *sensor = manager->get_sensor(id);

	if (!sensor) {
		ERR("Failed to set attribute to sensor(0x%x)", id);
		return false;
	}

	ret = sensor->set_attribute_int(attribute, value);

	if ((ret < 0) && (ret != -EBUSY)) {
		ERR("Failed to send sensorhub data");
		return false;
	}

	if (ret == -EBUSY) {
		WARN("Command is sent during sensorhub firmware update");
		return false;
	}

	return true;
}

bool sensorhub_device::set_attribute_str(uint32_t id, int32_t attribute, char *value, int value_len)
{
	int ret;

	sensorhub_sensor *sensor = manager->get_sensor(id);

	if (!sensor) {
		ERR("Failed to set attribute to sensor(0x%x)", id);
		return false;
	}

	ret = sensor->set_attribute_str(attribute, value, value_len);

	if ((ret < 0) && (ret != -EBUSY)) {
		ERR("Failed to send sensorhub data");
		return false;
	}

	if (ret == -EBUSY) {
		WARN("Command is sent during sensorhub firmware update");
		return false;
	}

	return true;
}

int sensorhub_device::read_fd(uint32_t **ids)
{
	sensorhub_data_t data;

	// step 1
	if (!controller->read_fd(data))
		return 0;

	// step 2
	const char *hub_data = data.values;
	int data_len = data.value_count;

	// step 3
	event_ids.clear();

	while (data_len > 0) {
		DBG("Remaining data length: %d", data_len);
		int parsed = parse(hub_data, data_len);
		if (parsed < 0) {
			ERR("Parsing failed");
			break;
		}

		data_len -= parsed;
		hub_data += parsed;
	}

	// step 4
	int size = event_ids.size();

	if (event_ids.empty())
		return 0;

	*ids = &event_ids[0];

	return size;
}

int sensorhub_device::get_data(uint32_t id, sensor_data_t **data, int *length)
{
	int remains = 1;

	sensorhub_sensor *sensor = manager->get_sensor(id);
	if (!sensor) {
		ERR("Failed to get data from sensor(0x%x)", id);
		return -1;
	}

	remains = sensor->get_data(data, length);

	return remains;
}

bool sensorhub_device::flush(uint32_t id)
{
	return false;
}

int sensorhub_device::parse(const char *hub_data, int data_len)
{
	return parse_data(hub_data, data_len);
}

int sensorhub_device::parse_data(const char *hub_data, int data_len)
{
	const char *cursor = hub_data;
	int32_t libtype = 0;

	sensorhub_sensor *sensor = manager->get_sensor(libtype);
	if (!sensor) {
		ERR("Unknown Sensorhub lib type: %d", libtype);
		return -1;
	}

	event_ids.push_back(sensor->get_id());

	return sensor->parse(cursor, data_len);
}

int sensorhub_device::parse_debug(const char *hub_data, int data_len)
{
	return 0;
}


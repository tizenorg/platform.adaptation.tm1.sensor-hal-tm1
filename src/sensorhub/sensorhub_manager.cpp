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

#include "sensorhub_controller.h"
#include "sensorhub_manager.h"
#include "sensorhub_sensor.h"

#define SENSORHUB_LIB_MASK 0xFF

sensorhub_manager::sensorhub_manager()
{
}

sensorhub_manager::~sensorhub_manager()
{
	for (auto &it : m_lib_sensor)
		delete it.second;

	m_lib_sensor.clear();
	m_handles.clear();
}

sensorhub_manager& sensorhub_manager::get_instance() {
	static sensorhub_manager instance;
	return instance;
}

bool sensorhub_manager::add_sensor(sensor_handle_t handle, char lib, sensorhub_sensor *sensor)
{
	m_handles.push_back(handle);
	m_lib_sensor[lib] = sensor;

	return true;
}

void sensorhub_manager::set_controller(sensorhub_controller *controller)
{
	for (auto const &it : m_lib_sensor) {
		sensorhub_sensor *sensor = it.second;
		sensor->set_controller(controller);
	}
}

sensorhub_sensor *sensorhub_manager::get_sensor(uint32_t id)
{
	return m_lib_sensor[(id & 0xFF)];
}

sensorhub_sensor *sensorhub_manager::get_sensor(char lib)
{
	return m_lib_sensor[lib];
}

int sensorhub_manager::get_sensors(const sensor_handle_t **sensors)
{
	int size;

	if (m_handles.empty()) {
		*sensors = 0;
		return 0;
	}

	size = m_handles.size();
	*sensors = &m_handles[0];

	return size;
}

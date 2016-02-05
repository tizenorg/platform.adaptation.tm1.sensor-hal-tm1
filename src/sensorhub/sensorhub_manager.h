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

#ifndef _SENSORHUB_MANAGER_H_
#define _SENSORHUB_MANAGER_H_

#include <map>
#include <vector>

#include "sensorhub_controller.h"
#include "sensorhub_sensor.h"

#define REGISTER_SENSOR(handle, key, sensor_class) \
static void __attribute__((constructor)) add_sensorhub_sensor(void) \
{ \
	sensorhub_sensor *sensor = new(std::nothrow) (sensor_class)(); \
	if (!sensor) { \
		_E("ERROR: Failed to allocate memory(%s)", #sensor_class); \
		return; \
	} \
	sensorhub_manager::add_sensor((handle), (key), (sensor)); \
}

class sensorhub_manager {
public:
	sensorhub_manager();
	virtual ~sensorhub_manager();

	sensorhub_sensor *get_sensor(uint16_t id);
	sensorhub_sensor *get_sensor(char key);
	int get_sensors(const sensor_handle_t **sensors);

	void set_controller(sensorhub_controller *controller);

	static bool add_sensor(sensor_handle_t handle, char key, sensorhub_sensor *sensor);
private:
	static std::map<uint16_t, sensorhub_sensor *> m_id_sensor;
	static std::map<char, sensorhub_sensor *> m_key_sensor;
	static std::vector<sensor_handle_t> m_handles;
};
#endif /* _SENSORHUB_MANAGER_H_ */

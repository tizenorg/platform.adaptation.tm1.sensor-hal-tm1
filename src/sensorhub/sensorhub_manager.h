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
#include <sensor_logs.h>

#include "sensorhub_controller.h"
#include "sensorhub_sensor.h"

#define REGISTER_SENSORHUB_LIB(handle, key, sensor_class) \
	static sensor_initializer<sensor_class> initializer((handle), (key)); \

class sensorhub_manager {
public:
	static sensorhub_manager& get_instance();
	virtual ~sensorhub_manager();

	sensorhub_sensor *get_sensor(uint16_t id);
	sensorhub_sensor *get_sensor(char key);
	int get_sensors(const sensor_handle_t **sensors);

	void set_controller(sensorhub_controller *controller);
	bool add_sensor(sensor_handle_t handle, char key, sensorhub_sensor *sensor);
private:
	sensorhub_manager();

	std::map<uint16_t, sensorhub_sensor *> m_id_sensor;
	std::map<char, sensorhub_sensor *> m_key_sensor;
	std::vector<sensor_handle_t> m_handles;
};

template <typename T>
class sensor_initializer {
public:
	sensor_initializer(sensor_handle_t handle, char key)
	{
		T *sensor = new(std::nothrow) T();
		if (!sensor) {
			ERR("Failed to allocate memory");
			return;
		}
		sensorhub_manager::get_instance().add_sensor(handle, key, sensor);
	}
	~sensor_initializer() {}
};

#endif /* _SENSORHUB_MANAGER_H_ */

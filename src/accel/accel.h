/*
 * accel_sensor_device
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

#ifndef _ACCEL_SENSOR_DEVICE_H_
#define _ACCEL_SENSOR_DEVICE_H_

#include <sensor_hal.h>

class accel_sensor_device : public sensor_device
{
public:
	accel_sensor_device();
	virtual ~accel_sensor_device();

	int get_poll_fd(void);
	bool get_sensors(std::vector<sensor_handle_t> &sensors);
	bool enable(uint32_t id);
	bool disable(uint32_t id);
	bool set_interval(uint32_t id, unsigned long val);
	bool set_batch_latency(uint32_t id, unsigned long val);
	bool set_command(uint32_t id, std::string command, std::string value);
	bool is_data_ready(void);
	bool get_sensor_data(uint32_t id, sensor_data_t &data);
	int get_sensor_event(uint32_t id, sensor_event_t **event);
	bool get_properties(uint32_t id, sensor_properties_s &properties);

private:
	int m_node_handle;
	int m_x;
	int m_y;
	int m_z;
	unsigned long m_polling_interval;
	unsigned long long m_fired_time;
	bool m_sensorhub_controlled;

	std::string m_data_node;
	std::string m_enable_node;
	std::string m_interval_node;

	bool update_value_input_event(void);
	void raw_to_base(sensor_data_t &data);
};
#endif /*_ACCEL_SENSOR_DEVICE_CLASS_H_*/

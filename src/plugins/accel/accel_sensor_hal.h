/*
 * accel_sensor_hal
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

#ifndef _ACCEL_SENSOR_HAL_H_
#define _ACCEL_SENSOR_HAL_H_

#include <sensor_hal_base.h>
#include <functional>

class accel_sensor_hal : public sensor_hal_base
{
public:
	accel_sensor_hal();
	virtual ~accel_sensor_hal();

	int get_poll_fd(void);
	bool get_sensors(std::vector<sensor_handle_t> &sensors);
	bool enable(uint32_t id);
	bool disable(uint32_t id);
	bool set_interval(uint32_t id, unsigned long val);
	bool set_batch_latency(uint32_t id, unsigned long val);
	bool set_command(uint32_t id, std::string command, std::string value);
	bool is_data_ready(void);
	bool get_sensor_data(uint32_t id, sensor_data_t &data);
	bool get_properties(uint32_t id, sensor_properties_s &properties);

private:
	int m_node_handle;
	int m_x;
	int m_y;
	int m_z;
	unsigned long m_polling_interval;
	unsigned long long m_fired_time;

	std::string m_model_id;
	std::string m_vendor;
	std::string m_chip_name;

	int m_resolution;
	float m_raw_data_unit;

	int m_method;
	std::string m_data_node;
	std::string m_enable_node;
	std::string m_interval_node;

	std::function<bool ()> update_value;

	bool m_sensorhub_controlled;

	bool update_value_input_event(void);
	bool update_value_iio(void);

	void raw_to_base(sensor_data_t &data);
};
#endif /*_ACCEL_SENSOR_HAL_CLASS_H_*/

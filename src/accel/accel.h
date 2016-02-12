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

#ifndef _ACCEL_DEVICE_H_
#define _ACCEL_DEVICE_H_

#include <sensor_hal.h>
#include <string>
#include <vector>

class accel_device : public sensor_device {
public:
	accel_device();
	virtual ~accel_device();

	int get_poll_fd(void);
	int get_sensors(const sensor_handle_t **sensors);

	bool enable(uint32_t id);
	bool disable(uint32_t id);

	bool set_interval(uint32_t id, unsigned long val);
	bool set_batch_latency(uint32_t id, unsigned long val);
	bool set_attribute_int(uint32_t id, int32_t attribute, int32_t value);
	bool set_attribute_str(uint32_t id, int32_t attribute, char *value, int value_len);

	int read_fd(uint32_t **ids);
	int get_data(uint32_t id, sensor_data_t **data, int *length);

	bool flush(uint32_t id);

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

	static std::vector<uint32_t> event_ids;

	bool update_value_input_event(void);
	void raw_to_base(sensor_data_t *data);
};
#endif /*_ACCEL_DEVICE_H_*/

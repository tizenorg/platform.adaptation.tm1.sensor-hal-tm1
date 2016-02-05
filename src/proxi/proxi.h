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

#ifndef _PROXI_DEVICE_H_
#define _PROXI_DEVICE_H_

#include <sensor_hal.h>
#include <string>
#include <vector>

class proxi_device : public sensor_device
{
public:
	proxi_device();
	virtual ~proxi_device();

	int get_poll_fd(void);
	int get_sensors(const sensor_handle_t **sensors);

	bool enable(uint16_t id);
	bool disable(uint16_t id);

	bool set_interval(uint16_t id, unsigned long val);
	bool set_batch_latency(uint16_t id, unsigned long val);
	bool set_attribute(uint16_t id, int32_t attribute, int32_t value);
	bool set_attribute_str(uint16_t id, char *attribute, char *value, int value_len);

	int read_fd(uint16_t **ids);
	int get_data(uint16_t id, sensor_data_t **data, int *length);

	bool flush(uint16_t id);

private:
	int m_node_handle;
	unsigned int m_state;
	unsigned long long m_fired_time;
	bool m_sensorhub_controlled;

	std::string m_data_node;
	std::string m_enable_node;

	static std::vector<uint16_t> event_ids;

	bool update_value_input_event(void);
};
#endif /*_PROXI_DEVICE_H_*/

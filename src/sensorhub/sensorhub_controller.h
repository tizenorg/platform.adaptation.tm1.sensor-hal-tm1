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

#ifndef _SENSORHUB_CONTROLLER_H_
#define _SENSORHUB_CONTROLLER_H_

#include <sensor/sensor_hal.h>

class sensorhub_controller {
public:
	static sensorhub_controller& get_instance();
	virtual ~sensorhub_controller();

	int open_input_node(const char* input_node);
	int get_poll_fd(void);

	bool enable(void);
	bool disable(void);

	bool read_fd(sensorhub_data_t &data);

	int read_sensorhub_data(void);
	int read_large_sensorhub_data(void);
	int send_sensorhub_data(const char *data, int data_len);

private:
	sensorhub_controller();

	bool m_enabled;
	int m_poll_node;
	int m_data_node;

	int print_sensorhub_data(const char* name, const char *data, int length);
};

#endif /* _SENSORHUB_CONTROLLER_H_ */

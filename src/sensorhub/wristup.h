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

#ifndef _WRISTUP_SENSOR_H_
#define _WRISTUP_SENSOR_H_

#include "sensorhub_sensor.h"

class wristup_sensor : public sensorhub_sensor {
public:
	wristup_sensor();
	virtual ~wristup_sensor();

	int32_t get_id(void);

	bool enable(void);
	bool disable(void);

	int parse(const char *data, int data_len);
	int get_data(sensor_data_t **data, int *length);

	bool set_attribute_int(int32_t attribute, int32_t value);
	bool set_attribute_str(int32_t attribute, char *value, int value_len);

private:
	sensorhub_data_t m_data;
};

#endif /* _WRISTUP_SENSOR_H_ */

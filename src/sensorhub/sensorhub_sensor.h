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

#ifndef _SENSORHUB_SENSOR_H_
#define _SENSORHUB_SENSOR_H_

class sensorhub_sensor {
public:
	sensorhub_sensor() {}
	virtual ~sensorhub_sensor() {}

	virtual int32_t get_id(void) = 0;

	virtual bool enable(void) = 0;
	virtual bool disable(void) = 0;
	virtual int parse(const char *hub_data, int data_len) = 0;
	virtual int get_data(sensor_data_t **data, int *length) = 0;

	void set_controller(sensorhub_controller *controller);

	virtual bool set_interval(unsigned long val);
	virtual bool set_batch_latency(unsigned long val);
	virtual bool set_attribute_int(int32_t attribute, int32_t value);
	virtual bool set_attribute_str(int32_t key, char *value, int value_len);

	virtual bool flush(void);
protected:
	sensorhub_controller *m_controller;

	unsigned long long get_timestamp(void);
};

#endif /* _SENSORHUB_SENSOR_H_ */

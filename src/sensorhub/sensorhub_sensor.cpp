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

#include <sensor_logs.h>
#include <time.h>
#include "sensorhub_controller.h"
#include "sensorhub_sensor.h"

void sensorhub_sensor::set_controller(sensorhub_controller *controller)
{
	m_controller = controller;
}

bool sensorhub_sensor::set_interval(unsigned long val)
{
	return false;
}

bool sensorhub_sensor::set_batch_latency(unsigned long val)
{
	return false;
}

bool sensorhub_sensor::set_attribute(int32_t attribute, int32_t value)
{
	return false;
}

bool sensorhub_sensor::set_attribute_str(char *attribute, char *value, int value_len)
{
	return false;
}

bool sensorhub_sensor::flush(void)
{
	return false;
}

unsigned long long sensorhub_sensor::get_timestamp(void)
{
	struct timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	return ((unsigned long long)(t.tv_sec)*1000000000LL + t.tv_nsec) / 1000;
}


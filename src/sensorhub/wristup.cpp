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

#include <algorithm>
#include <sensor_hal.h>
#include <sensor_logs.h>

#include "sensorhub_manager.h"
#include "system_state.h"
#include "wristup.h"

#define SENSOR_NAME "WRIST_UP_SENSOR"
#define SHUB_LIB_WRIST_UP	0
#define WRIST_UP_PACKET_SIZE	1

static const sensor_handle_t handle = {
	id: 0x1,
	name: SENSOR_NAME,
	type: SENSOR_DEVICE_GESTURE_WRIST_UP,
	event_type: (SENSOR_DEVICE_GESTURE_WRIST_UP << 16) | 0x0001,
	model_name: "Wristup",
	vendor: "Samsung Electronics",
	min_range: 0,
	max_range: 1,
	resolution: 1,
	min_interval: 0,
	max_batch_count: 0,
	wakeup_supported: true
};

wristup_sensor::wristup_sensor()
{
}

wristup_sensor::~wristup_sensor()
{
	INFO("wristup_sensor is destroyed!");
}

int16_t wristup_sensor::get_id(void)
{
	return handle.id;
}

bool wristup_sensor::enable(void)
{
	return false;
}

bool wristup_sensor::disable(void)
{
	return false;
}

int wristup_sensor::parse(const char *data, int data_len)
{
	return WRIST_UP_PACKET_SIZE;
}

int wristup_sensor::get_data(sensor_data_t **data, int *length)
{
	return -1;
}

bool wristup_sensor::set_attribute(int32_t attribute, int32_t value)
{
	return false;
}

bool wristup_sensor::set_attribute_str(char *key, char *value, int value_len)
{
	return false;
}

REGISTER_SENSOR(handle, (char)SHUB_LIB_WRIST_UP, wristup_sensor)

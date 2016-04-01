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

#include <sensor/sensor_hal.h>
#include <sensor_logs.h>
#include <vector>

#include "accel/accel.h"
#include "proxi/proxi.h"
#include "sensorhub/sensorhub.h"

static std::vector<sensor_device_t> devs;

template<typename _sensor>
void create_sensor(const char *name)
{
	sensor_device *instance = NULL;
	try {
		instance = new _sensor;
	} catch (std::exception &e) {
		ERR("Failed to create %s sensor device, exception: %s", name, e.what());
		return;
	} catch (int err) {
		ERR("Failed to create %s sensor device, err: %d, cause: %s", name, err, _STRERR(err));
		return;
	}

	devs.push_back(instance);
}

extern "C" int create(sensor_device_t **devices)
{
#ifdef ENABLE_ACCEL
	create_sensor<accel_device>("Accel");
#endif

#ifdef ENABLE_PROXIMITY
	create_sensor<proxi_device>("Proximity");
#endif

#ifdef ENABLE_SENSORHUB
	create_sensor<sensorhub_device>("Sensorhub");
#endif

	*devices = &devs[0];

	return devs.size();
}

/*
 * sensor-plugins-tm1
 *
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

#include <sensor_hal.h>
#include <sensor_logs.h>

#ifdef ENABLE_ACCEL
#include "accel/accel.h"
#endif
//#include "gyro/gyro_sensor_device.h"
//#include "magnetic/geo_sensor_device.h"
#ifdef ENABLE_PROXIMITY
#include "proximity/proxi.h"
#endif
//#include "light/light_sensor_device.h"
//#include "rotation_vector/rv_raw_sensor_device.h"
//#include "pressure/pressure_sensor_device.h"
//#include "temperature/temperature_sensor_device.h"
//#include "ultraviolet/ultraviolet_sensor_device.h"
//#include "hrm_led_red/bio_led_red_sensor_device.h"

static sensor_devices *devices = NULL;

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
		ERR("Failed to create %s sensor device, err: %d, cause: %s", name, err, strerror(err));
		return;
	}

	devices->devices.push_back(instance);
}

extern "C" sensor_devices* create(void)
{
	devices = new(std::nothrow) sensor_devices;
	retvm_if(!devices, NULL, "Failed to allocate memory");

#ifdef ENABLE_ACCEL
	create_sensor<accel_device>("Accel");
#endif

#ifdef ENABLE_GYRO
	create_sensor<gyro_sensor_device>("Gyro");
#endif

#ifdef ENABLE_MAGNETIC
	create_sensor<geo_sensor_device>("Magnetic");
#endif

#ifdef ENABLE_PROXIMITY
	create_sensor<proxi_sensor_device>("Proximity");
#endif

#ifdef ENABLE_LIGHT
	create_sensor<light_sensor_device>("Light");
#endif

#ifdef ENABLE_ROTATION_VECTOR
	create_sensor<rv_raw_sensor_device>("Rotation Vector");
#endif

#ifdef ENABLE_PRESSURE
	create_sensor<pressure_sensor_device>("Pressure");
#endif

#ifdef ENABLE_TEMPERATURE
	create_sensor<temperature_sensor_device>("Temperature");
#endif

#ifdef ENABLE_ULTRAVIOLET
	create_sensor<ultraviolet_sensor_device>("Ultraviolet");
#endif

#ifdef ENABLE_HRM_LED_RED
	create_sensor<bio_led_red_sensor_device>("HRM Led Red");
#endif

	return devices;
}

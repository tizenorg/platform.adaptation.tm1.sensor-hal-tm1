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

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <fstream>

#include <sensor_logs.h>
#include <util.h>
#include "sensorhub_controller.h"

sensorhub_controller::sensorhub_controller()
: m_enabled(false)
, m_poll_node(-1)
, m_data_node(-1)
{
}

sensorhub_controller::~sensorhub_controller()
{
}

sensorhub_controller& sensorhub_controller::get_instance(void)
{
	static sensorhub_controller instance;
	return instance;
}

int sensorhub_controller::get_poll_fd(void)
{
	return -1;
}

bool sensorhub_controller::enable(void)
{
	m_enabled = true;
	INFO("Enable Sensorhub");
	return true;
}

bool sensorhub_controller::disable(void)
{
	m_enabled = false;
	INFO("Disable Sensorhub");
	return true;
}

int sensorhub_controller::open_input_node(const char* input_node)
{
	return -1;
}

bool sensorhub_controller::read_fd(sensorhub_data_t &data)
{
	return false;
}

int sensorhub_controller::read_sensorhub_data(void)
{
	return -1;
}

int sensorhub_controller::read_large_sensorhub_data(void)
{
	return -1;
}

int sensorhub_controller::send_sensorhub_data(const char *data, int data_len)
{
	return -1;
}

int sensorhub_controller::print_sensorhub_data(const char* name, const char *data, int length)
{
	return 0;
}


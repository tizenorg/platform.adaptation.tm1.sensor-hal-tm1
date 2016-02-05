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
#include "dbus_util.h"
#include "system_state.h"

#define CPU_BOOST_DURATION 400
#define WAKEUP_LCD_CUSTOM_PARAM 7000

system_state_handler::system_state_handler()
{
}

system_state_handler::~system_state_handler()
{
	INFO("system_state_handler is destroyed!\n");
}

system_state_handler& system_state_handler::get_instance()
{
	static system_state_handler instance;
	return instance;
}

void system_state_handler::set_controller(sensorhub_controller *controller)
{
	m_controller = controller;
}

bool system_state_handler::initialize()
{
	return true;
}

void system_state_handler::finalize(void)
{
}

void system_state_handler::set_poweroff_state(void)
{
}

void system_state_handler::display_signal_handler(GDBusConnection *conn,
		const gchar *name, const gchar *path, const gchar *interface,
		const gchar *sig, GVariant *param, gpointer user_data)
{
}

bool system_state_handler::start_listen_display_state()
{
	return true;
}

void system_state_handler::stop_listen_display_state()
{

}

void system_state_handler::poweroff_signal_handler(GDBusConnection *conn, const gchar *name, const gchar *path, const gchar *interface,
		const gchar *sig, GVariant *param, gpointer user_data)
{
}

bool system_state_handler::start_listen_poweroff_state(void)
{
	return true;
}

void system_state_handler::stop_listen_poweroff_state(void)
{
}

void system_state_handler::charger_state_cb(keynode_t *node, void *user_data)
{
}

bool system_state_handler::is_display_on(void)
{
	return true;
}

bool system_state_handler::inform_display_state(bool display_on)
{
	return inform_display_state();
}

bool system_state_handler::inform_display_state()
{
	return true;
}

bool system_state_handler::is_charger_connected(void)
{
	return false;
}

bool system_state_handler::is_powering_off()
{
	return false;
}

bool system_state_handler::inform_charger_state()
{
	return true;
}

bool system_state_handler::inform_ssp_sleep(void)
{
	return true;
}

bool system_state_handler::inform_ssp_wakeup(void)
{
	return true;
}

bool system_state_handler::inform_current_time(void)
{
	return true;
}

void system_state_handler::request_display_lock(int msec)
{
	return;
}

void system_state_handler::request_display_wakeup()
{
	return;
}

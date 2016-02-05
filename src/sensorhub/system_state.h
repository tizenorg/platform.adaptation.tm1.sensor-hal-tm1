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

#ifndef _SYSTEM_STATE_HANDLER_H_
#define _SYSTEM_STATE_HANDLER_H_

#include <glib.h>
#include <vconf.h>
#include <gio/gio.h>

#include "sensorhub_controller.h"

class system_state_handler {
private:
	system_state_handler();
	~system_state_handler();

	void set_poweroff_state(void);

	bool start_listen_display_state(void);
	bool start_listen_poweroff_state(void);
	void stop_listen_display_state(void);
	void stop_listen_poweroff_state(void);

	bool inform_display_state(bool display_on);
	bool inform_display_state(void);
	bool inform_ssp_sleep(void);
	bool inform_ssp_wakeup(void);

	static void display_signal_handler(GDBusConnection *conn, const gchar *name, const gchar *path, const gchar *interface,
		const gchar *sig, GVariant *param, gpointer user_data);

	static void poweroff_signal_handler(GDBusConnection *conn, const gchar *name, const gchar *path, const gchar *interface,
		const gchar *sig, GVariant *param, gpointer user_data);

	static void display_state_cb(keynode_t *node, void *user_data);
	static void charger_state_cb(keynode_t *node, void *user_data);
	static void sleep_state_cb(keynode_t *node, void *user_data);

	sensorhub_controller *m_controller;

public:
	void set_controller(sensorhub_controller *controller);
	bool initialize(void);
	void finalize(void);

	bool is_display_on(void);
	bool is_charger_connected(void);
	bool is_powering_off(void);

	bool inform_charger_state(void);
	bool inform_current_time(void);
	void request_display_lock(int msec);
	void request_display_wakeup(void);

	static system_state_handler& get_instance(void);
};

#endif

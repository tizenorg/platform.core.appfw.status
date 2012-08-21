/*
 *  libstatus
 *
 * Copyright (c) 2000 - 2012 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Youngjoo Park <yjoo93.park@samsung.com>,
 *	Seungtaek Chung <seungtaek.chung@samsung.com>, Youngsub Ko <ys4610.ko@samsung.com>
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
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <vconf.h>
#include <aul.h>

#include "status-type.h"
#include "status_log.h"
#include "status-monitor.h"
#include "status-internal.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "status-monitor"

#ifndef EXPORT_API
#define EXPORT_API __attribute__ ((visibility("default")))
#endif

struct _message_cb_data {
	status_message_cb callback;
	void *data;
};

struct _state_cb_data {
	status_background_state_cb callback;
	void *data;
};

static struct _message_cb_data md;
static struct _state_cb_data sd[STATUS_BACKGROUND_TYPE_NUM];
static int state_pid[STATUS_BACKGROUND_TYPE_NUM] = {
	[STATUS_BACKGROUND_TYPE_MUSIC] = -1,
	[STATUS_BACKGROUND_TYPE_RADIO] = -1,
	[STATUS_BACKGROUND_TYPE_VOICE_RECORDING] = -1,
};
static int dead_tracker_initialize;

static void __status_message_change_cb(keynode_t *node, void *user_data)
{
	int type;
	char *message = NULL;

	if (!node) {
		ERR("key node is NULL");
		return;
	}

	if (!md.callback)
		return;

	type = vconf_keynode_get_type(node);
	if (type != VCONF_TYPE_STRING) {
		ERR("invail node type : %d", type);
		return;
	}

	message = vconf_keynode_get_str(node);
	if (!message) {
		ERR("fail to get message");
		return;
	}

	if (strlen(message) <= 0){
		DBG("message has only NULL");
		return;
	}

	DBG("message : %s", message);
	md.callback(message, md.data);
}

EXPORT_API
int status_monitor_message_cb_set(status_message_cb callback, void *user_data)
{
	int ret = 0;
	if (!callback)
		return STATUS_ERROR_INVALID_PARAMETER;

	ret = vconf_notify_key_changed(STATUS_MESSAGE_KEY,
				__status_message_change_cb, NULL);
	if (ret && errno != EALREADY) {
		ERR("fail to set message cb");
		return STATUS_ERROR_IO_ERROR;
	}


	md.callback = callback;
	md.data = user_data;

	return STATUS_ERROR_NONE;
}

EXPORT_API
int status_monitor_message_cb_unset(void)
{
	vconf_ignore_key_changed(STATUS_MESSAGE_KEY,
				__status_message_change_cb);
	md.callback = NULL;
	md.data = NULL;

	return STATUS_ERROR_NONE;
}

static int __status_state_reset(status_background_type_e type)
{
	int ret = 0;
	const int pid = -1;
	const int state = STATUS_BACKGROUND_STATE_STOP;

	if (type < STATUS_BACKGROUND_TYPE_MUSIC
		|| type >= STATUS_BACKGROUND_TYPE_NUM) {
		ERR("invaild type : %d", type);
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	ret = vconf_set_int(background_pid_keys[type], pid);
	if (ret) {
		ERR("fail to reset [%s]", background_pid_keys[type]);
		return STATUS_ERROR_IO_ERROR;
	}
	ret = vconf_set_int(background_state_keys[type], state);
	if (ret) {
		ERR("fail to reset [%s]", background_state_keys[type]);
		return STATUS_ERROR_IO_ERROR;
	}

	return STATUS_ERROR_NONE;
}


static int __state_get_val(const status_background_type_e type,
		status_background_state_e *state)
{
	int k_state = 0;
	int ret = 0;

	if (!state)
		return STATUS_ERROR_INVALID_PARAMETER;

	ret = vconf_get_int(background_state_keys[type], &k_state);
	if (ret) {
		ERR("fail to get state of [%s]", background_state_keys[type]);
		return STATUS_ERROR_IO_ERROR;
	}

	*state = k_state;

	return STATUS_ERROR_NONE;
}

static inline int __state_cb_call(keynode_t *node,
			const status_background_type_e type)
{
	status_background_state_e state = STATUS_BACKGROUND_STATE_STOP;
	int ret = 0;

	if (!sd[type].callback)
		return 0;

	if (node == NULL) {
		ret = __state_get_val(type, &state);
		if (ret)
			return ret;
	} else {
		int type;
		type = vconf_keynode_get_type(node);
		if (type != VCONF_TYPE_INT) {
			ERR("invail node type : %d", type);
			return -1;
		}
		state = vconf_keynode_get_int(node);
		if (state < 0) {
			ERR("fail to get state : %d", state);
			return -1;
		}
	}

	DBG("type[%d], state[%d]", type, state);
	sd[type].callback(type, state, sd[type].data);

	return 0;
}

static void __status_music_change_cb(keynode_t *node, void *user_data)
{
	__state_cb_call(node, STATUS_BACKGROUND_TYPE_MUSIC);
}

static void __status_radio_change_cb(keynode_t *node, void *user_data)
{
	__state_cb_call(node, STATUS_BACKGROUND_TYPE_RADIO);
}

static void __status_vr_change_cb(keynode_t *node, void *user_data)
{
	__state_cb_call(node, STATUS_BACKGROUND_TYPE_VOICE_RECORDING);
}

static vconf_callback_fn state_cbs[STATUS_BACKGROUND_TYPE_NUM] = {
	[STATUS_BACKGROUND_TYPE_MUSIC] = __status_music_change_cb,
	[STATUS_BACKGROUND_TYPE_RADIO] = __status_radio_change_cb,
	[STATUS_BACKGROUND_TYPE_VOICE_RECORDING] = __status_vr_change_cb,

};

static int _dead_tracker(const int pid, void *data)
{
	int i = 0;

	DBG("pid[%d] is dead", pid);
	for (i = 0; i < STATUS_BACKGROUND_TYPE_NUM; i++) {
		if (state_pid[i] == pid) {
			DBG("type[%d] - pid[%d] is dead", i, pid);
			__status_state_reset(i);
		}
	}

	return 0;
}

static inline void __update_stored_pid(status_background_type_e type,
				keynode_t *node)
{
	int pid = -1;
	if (!node)
		return;

	pid = vconf_keynode_get_int(node);

	state_pid[type] = pid;
	DBG("pid[%d], type[%d]", pid, type);
}

static void _music_pid_changed_cb(keynode_t *node, void *user_data)
{
	__update_stored_pid(STATUS_BACKGROUND_TYPE_MUSIC, node);
}

static void _radio_pid_changed_cb(keynode_t *node, void *user_data)
{
	__update_stored_pid(STATUS_BACKGROUND_TYPE_RADIO, node);
}

static void _vr_pid_changed_cb(keynode_t *node, void *user_data)
{
	__update_stored_pid(STATUS_BACKGROUND_TYPE_VOICE_RECORDING, node);
}

static vconf_callback_fn _pid_cbs[STATUS_BACKGROUND_TYPE_NUM] = {
	[STATUS_BACKGROUND_TYPE_MUSIC] = _music_pid_changed_cb,
	[STATUS_BACKGROUND_TYPE_RADIO] = _radio_pid_changed_cb,
	[STATUS_BACKGROUND_TYPE_VOICE_RECORDING] = _vr_pid_changed_cb,
};

static int __dead_tracker_init(void)
{
	int ret = 0;

	if (dead_tracker_initialize)
		return 0;

	ret = aul_listen_app_dead_signal(_dead_tracker, NULL);
	if (ret != AUL_R_OK) {
		ERR("fail to aul_listen_app_dead_signal() - %d", ret);
		return -1;
	}

	dead_tracker_initialize = 1;

	return 0;
}

static void __dead_tracker_fini(void)
{
	int i = 0;
	int all_cb_removed = 1;

	if (!dead_tracker_initialize)
		return;

	for (i = 0; i < STATUS_BACKGROUND_TYPE_NUM; i++)
		all_cb_removed &= !(sd[i].callback);

	if (all_cb_removed) {
		aul_listen_app_dead_signal(NULL, NULL);
		dead_tracker_initialize = 0;
	}

	return;
}

EXPORT_API
int status_monitor_state_cb_set(const status_background_type_e type,
		status_background_state_cb callback, void *user_data)
{
	int ret = 0;
	int pid = -1;

	if (type < STATUS_BACKGROUND_TYPE_MUSIC
		|| type >= STATUS_BACKGROUND_TYPE_NUM) {
		ERR("invaild type : %d", type);
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	if (!callback)
		return STATUS_ERROR_INVALID_PARAMETER;

	ret = vconf_notify_key_changed(background_state_keys[type],
				state_cbs[type], NULL);
	if (ret && errno != EALREADY) {
		ERR("fail to cb set for [%d]", type);
		return STATUS_ERROR_IO_ERROR;
	}

	ret = vconf_notify_key_changed(background_pid_keys[type],
			_pid_cbs[type], NULL);
	if (ret && errno != EALREADY)
		ERR("fail to set pid cb for [%d]", type);

	vconf_get_int(background_pid_keys[type], &pid);
	state_pid[type] = pid;

	__dead_tracker_init();

	sd[type].callback = callback;
	sd[type].data = user_data;

	return STATUS_ERROR_NONE;
}

EXPORT_API
int status_monitor_state_cb_unset(const status_background_type_e type)
{
	if (type < STATUS_BACKGROUND_TYPE_MUSIC
		|| type >= STATUS_BACKGROUND_TYPE_NUM) {
		ERR("invaild type : %d", type);
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	vconf_ignore_key_changed(background_state_keys[type], state_cbs[type]);
	vconf_ignore_key_changed(background_pid_keys[type], _pid_cbs[type]);
	sd[type].callback = NULL;
	sd[type].data = NULL;
	state_pid[type] = -1;

	__dead_tracker_fini();

	return STATUS_ERROR_NONE;
}

EXPORT_API
int status_monitor_state_get(const status_background_type_e type,
			status_background_state_e *state)
{
	if (type < STATUS_BACKGROUND_TYPE_MUSIC
		|| type >= STATUS_BACKGROUND_TYPE_NUM) {
		ERR("invaild type : %d", type);
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	return __state_get_val(type, state);
}


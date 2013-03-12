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
#include <string.h>
#include <vconf.h>

#include "status-type.h"
#include "status_log.h"
#include "status.h"
#include "status-internal.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "status"

#ifndef EXPORT_API
#define EXPORT_API __attribute__ ((visibility("default")))
#endif

EXPORT_API
int status_message_post(const char *message)
{
	int ret = 0;

	if (!message) {
		ERR("message is NULL");
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	if (strlen(message) <= 0) {
		ERR("message has only NULL");
		return STATUS_ERROR_INVALID_PARAMETER;
	}


	ret = vconf_set_str(STATUS_MESSAGE_KEY, message);
	if (ret) {
		ERR("fail to set message [%s]", message);
		return STATUS_ERROR_IO_ERROR;
	}

	return STATUS_ERROR_NONE;
}

EXPORT_API
int status_background_status_post(const status_background_type_e type,
		const status_background_state_e state)
{
	int pid = -1;
	int ret = 0;

	if (type < STATUS_BACKGROUND_TYPE_MUSIC
		|| type >= STATUS_BACKGROUND_TYPE_NUM) {
		ERR("invaild type [%d]", type);
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	if (state < STATUS_BACKGROUND_STATE_STOP
		|| state >= STATUS_BACKGROUND_STATE_NUM) {
		ERR("invaild state [%d]", state);
		return STATUS_ERROR_INVALID_PARAMETER;
	}

	if (state != STATUS_BACKGROUND_STATE_STOP)
		pid = getpid();

	ret = vconf_set_int(background_pid_keys[type], pid);
	if (ret) {
		ERR("fail to set [%s], pid[%d]",
			background_pid_keys[type], pid);
		return STATUS_ERROR_IO_ERROR;
	}

	ret = vconf_set_int(background_state_keys[type], state);
	if (ret) {
		ERR("fail to set [%s], state[%d]",
			background_state_keys[type], state);
		return STATUS_ERROR_IO_ERROR;
	}

	return STATUS_ERROR_NONE;
}


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

#ifndef __STATUS_MONITOR_DEF_H__
#define __STATUS_MONITOR_DEF_H__

#include <status-type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup STATUS_MONITOR_LIBRARY notification status monitor Library
 * @brief This notification status monitor library used to display simple plain message on indicator
 */

/**
 * @ingroup STATUS_MONITOR_LIBRARY
 * @defgroup STATUS_MONITOR_TYPE notification status monitor type
 * @brief notification status monitor side type
 */

/**
 * @addtogroup STATUS_MONITOR_TYPE
 * @{
 */

 /**
 * @brief Called when new message is posted
 * @param[in] message The message posted
 * @param[in] data user data
 * @pre status_monitor_message_cb_set() used to register this callback
 * @see status_monitor_message_cb_set()
 */
typedef void (*status_message_cb)(const char *message, void *data);

/**
* @brief This callback type will be deprecated
*/
typedef void (*status_background_state_cb)(const status_background_type_e type,
			status_background_state_e state, void *data);

/**
 * @addtogroup STATUS_MONITOR_LIBRARY
 * @{
 */

/**
* @brief register callback for receiving message
* @param[in] callback The callback function
* @param[in] data user_data
*/
int status_monitor_message_cb_set(status_message_cb callback, void *user_data);

/**
* @brief This function will be deprecated
*/
int status_monitor_state_cb_set(const status_background_type_e type,
		status_background_state_cb callback, void *user_data);

/**
* @brief unregister callback for receiving message
* @param[in] callback The callback function
* @param[in] data user_data
*/
int status_monitor_message_cb_unset(void);

/**
* @brief This function will be deprecated
*/
int status_monitor_state_cb_unset(const status_background_type_e type);

/**
* @brief This function will be deprecated
*/
int status_monitor_state_get(const status_background_type_e type,
		status_background_state_e *state);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STATUS_MONITOR_DEF_H__ */


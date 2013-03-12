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

#ifndef __STATUS_DEF_H__
#define __STATUS_DEF_H__

#include <status-type.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup STATUS_CLIENT_LIBRARY notification status client Library
 * @brief This notification status client library used to post simple plain message on indicator
 */

/**
 * @addtogroup STATUS_CLIENT_LIBRARY
 * @{
 */

/**
 * @brief This function send string(message) to status monitor
 * @details Send the message to notification status monitor.
 * @param[in] message messages you want to post
 * @return #STATUS_ERROR_NONE if success, other value if failure
 * @see #_status_error_e
 */
int status_message_post(const char *message);

/**
* @brief This function will be deprecated
*/
int status_background_status_post(const status_background_type_e type,
		const status_background_state_e state);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STATUS_DEF_H__ */


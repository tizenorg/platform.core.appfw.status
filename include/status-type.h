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

#ifndef __STATUS_TYPE_DEF_H__
#define __STATUS_TYPE_DEF_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup STATUS_LIBRARY_TYPE notification status type
 * @brief Tnotification status type.
 */

/**
 * @addtogroup STATUS_LIBRARY_TYPE
 * @{
 */

/**
 * @breief This enumeration type will be deprecated
 */
typedef enum _status_background_type_e {
	STATUS_BACKGROUND_TYPE_MUSIC = 0,
	STATUS_BACKGROUND_TYPE_RADIO,
	STATUS_BACKGROUND_TYPE_VOICE_RECORDING,
	STATUS_BACKGROUND_TYPE_NUM,
} status_background_type_e;

/**
 * @breief This enumeration type will be deprecated
 */
typedef enum _status_background_state_e {
	STATUS_BACKGROUND_STATE_STOP = 0,
	STATUS_BACKGROUND_STATE_PAUSE,
	STATUS_BACKGROUND_STATE_RUN,
	STATUS_BACKGROUND_STATE_NUM,
} status_background_state_e;

/**
 * @breief Enumeration describing error code of notification library
 */
enum _status_error_e {
	STATUS_ERROR_NONE = 0,			/**< Success */
	STATUS_ERROR_INVALID_PARAMETER = -1,	/**< Invalid parameter */
	STATUS_ERROR_IO_ERROR = -2,		/**< I/O error */
};

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STATUS_TYPE_DEF_H__ */


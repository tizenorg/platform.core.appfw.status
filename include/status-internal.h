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

#ifndef __STATUS_INTERNAL_DEF_H__
#define __STATUS_INTERNAL_DEF_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define STATUS_MESSAGE_KEY "memory/private/libstatus/message"
#define STATUS_BG_MUSIC_STATE_KEY "memory/private/libstatus/music/state"
#define STATUS_BG_MUSIC_PID_KEY "memory/private/libstatus/music/pid"
#define STATUS_BG_RADIO_STATE_KEY "memory/private/libstatus/radio/state"
#define STATUS_BG_RADIO_PID_KEY "memory/private/libstatus/radio/pid"
#define STATUS_BG_VR_STATE_KEY "memory/private/libstatus/vr/state"
#define STATUS_BG_VR_PID_KEY "memory/private/libstatus/vr/pid"

static const char *background_state_keys[STATUS_BACKGROUND_TYPE_NUM] = {
	[STATUS_BACKGROUND_TYPE_MUSIC] = STATUS_BG_MUSIC_STATE_KEY,
	[STATUS_BACKGROUND_TYPE_RADIO] = STATUS_BG_RADIO_STATE_KEY,
	[STATUS_BACKGROUND_TYPE_VOICE_RECORDING] = STATUS_BG_VR_STATE_KEY,
};

static const char *background_pid_keys[STATUS_BACKGROUND_TYPE_NUM] = {
	[STATUS_BACKGROUND_TYPE_MUSIC] = STATUS_BG_MUSIC_PID_KEY,
	[STATUS_BACKGROUND_TYPE_RADIO] = STATUS_BG_RADIO_PID_KEY,
	[STATUS_BACKGROUND_TYPE_VOICE_RECORDING] = STATUS_BG_VR_PID_KEY,
};

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STATUS_INTERNAL_DEF_H__ */


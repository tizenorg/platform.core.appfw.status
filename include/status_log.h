/*
 *  libstatus
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Youngjoo Park <yjoo93.park@samsung.com>,
 * 	Seungtaek Chung <seungtaek.chung@samsung.com>, Youngsub Ko <ys4610.ko@samsung.com>
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


#ifndef __STATUS_LOG_DEF_H__
#define __STATUS_LOG_DEF_H__

#ifdef STATUS_USE_DLOG

#include <dlog.h>

#define DBG(fmt , args...) \
	do{ \
		LOGD("[%s : %d] "fmt"\n",__func__,__LINE__,##args ); \
	} while(0)

#define INFO(fmt , args...) \
	do{ \
		LOGI("[%s : %d] "fmt"\n",__func__,__LINE__,##args ); \
	} while(0)

#define WARN(fmt , args...) \
	do{ \
		LOGI("[%s : %d] "fmt"\n",__func__,__LINE__,##args ); \
	} while(0)

#define ERR(fmt , args...) \
	do{ \
		LOGI("[%s : %d] "fmt"\n",__func__,__LINE__,##args ); \
	} while(0)

#else /* STATUS_USE_DLOG */

#include <stdio.h>

#define DBG(fmt , args...) \
	do{ \
		printf("[D][%s : %d] "fmt"\n", __func__,__LINE__,##args ); \
	} while(0)

#define INFO(fmt , args...) \
	do{ \
		printf("[I][%s : %d] "fmt"\n", __func__,__LINE__,##args ); \
	} while(0)

#define WARN(fmt , args...) \
	do{ \
		printf("[W][%s : %d] "fmt"\n", __func__,__LINE__,##args ); \
	} while(0)

#define ERR(fmt , args...) \
	do{ \
		printf("[E][%s : %d] "fmt"\n", __func__,__LINE__,##args ); \
	} while(0)

#endif /* STATUS_USE_DLOG */

#endif /* __STATUS_LOG_DEF_H__ */

/*!
 * @section LICENSE
 *
 * (C) Copyright 2011~2015 Bosch Sensortec GmbH All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * @file         channel_g.c
 * @date         "Wed Dec 31 15:33:20 2014 +0800"
 * @commit       "7b2e44c"
 *
 * @brief
 *
 * @detail
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define LOG_TAG_MODULE "<channel_g>"
#include "sensord.h"

int get_data_g(void *pbuf, int n)
{
	struct exchange *pmsg = (struct exchange *)pbuf;

	UNUSED_PARAM(n);
	/* fill pmsg->data.gyro with real value */
	algo_get_proc_data_g(&pmsg->data);

	return 1;
}

int channel_init_g(struct channel *ch)
{
	int usecase_id;

	if (NULL == ch)
	{
		return -1;
	}
	usecase_id = algo_get_current_usecase();
	if (usecase_id == -1)
	{
		PERR("unknown usecase id!");
		return -1;
	}

	if (usecase_id == ALGO_FUSION_MODE_COMPASS)
	{
		ch->cfg.availability = UNAVAILABLE;
	}
	else if (usecase_id == ALGO_FUSION_MODE_M4G)
	{
		ch->cfg.availability = VIRTUAL;
		ch->cfg.interval_min = CFG_DELAY_VIRTUAL_G_MIN;
		ch->cfg.interval_max = CFG_DELAY_VIRTUAL_G_MAX;
		ch->cfg.dep_hw       = CFG_HW_DEP_A | CFG_HW_DEP_M;
	}
	else
	{
		ch->cfg.no_delay     = 0;
		ch->cfg.interval_min = CFG_DELAY_G_MIN;
		ch->cfg.interval_max = CFG_DELAY_G_MAX;
		ch->cfg.dep_hw       = CFG_HW_DEP_G;
	}
	return 0;
}

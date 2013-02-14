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

#include <stdio.h>
#include <glib.h>
#include <status.h>

#define MAX_STRING_LEN	256
#define CLEAR_STDIN {gint ch; while((ch=getchar())!=EOF&&ch!='\n'); }

static GIOChannel *stdin_channel;
static GMainLoop *loop;

typedef void (*menu_cb)(void);

struct menu_item {
	gchar *cmd;
	gchar *desc;
	menu_cb func;
};

static void do_quit(void)
{
	g_main_loop_quit(loop);
}


static void do_message_post(void)
{
	int ret;

	ret = status_message_post("test status message post && aaa");
	if (ret) {
		g_print("fail to status_message_post() : %d\n", ret);
		return;
	}

	g_print("success to status_message_post()\n");
}

static int _get_type(status_background_type_e *type)
{
	int input_type;
	int ret;
	int pass = 0;
	do {
		g_print("Select type : \n");
		g_print("0. Music\n");
		g_print("1. Radio\n");
		g_print("2. Voice recorder\n");
		ret = scanf("%d", &input_type);
		if (ret <= 0) {
			g_print("invaild input\n\n");
			input_type = -1;
		}
		CLEAR_STDIN;

		if (input_type >= 0 && input_type <= 2)
			pass = 1;
	} while (pass != 1);

	if (type)
		*type = (status_background_type_e)input_type;

	return 0;
}

static int _get_state(status_background_state_e *state)
{
	int input_state;
	int ret;
	int pass = 0;
	do {
		g_print("Select state : \n");
		g_print("0. STOP \n");
		g_print("1. PAUSE \n");
		g_print("2. RUN \n");
		ret = scanf("%d", &input_state);
		if (ret <= 0) {
			g_print("invaild input\n\n");
			input_state = -1;
		}
		CLEAR_STDIN;

		if (input_state >= 0 && input_state <= 2)
			pass = 1;
	} while (pass != 1);

	if (state)
		*state = (status_background_state_e)input_state;

	return 0;
}


static void do_background_state_post(void)
{
	int ret;
	status_background_type_e type;
	status_background_state_e state;

	if (_get_type(&type)) {
		g_print("fail to get type \n");
		return;
	}

	if (_get_state(&state)) {
		g_print("fail to get state \n");
		return;
	}

	ret = status_background_status_post(type, state);
	if (ret) {
		g_print("fail to status_background_status_post() "\
			"[%d][%d]: %d\n", type, state, ret);
		return;
	}

	g_print("success to status_background_status_post()\n");
}


static struct menu_item g_post_menu[] = {
	{"q", "quit", do_quit},
	{"m", "test message post", do_message_post},
	{"s", "test background state post", do_background_state_post},
	{NULL, NULL, NULL},
};

static void
post_display_menu(void)
{
	gint i = 0;

	g_print("\n");

	g_print("select commands \n");
	while (g_post_menu[i].cmd) {
		g_print("[%s] %s\n", g_post_menu[i].cmd, g_post_menu[i].desc);
		i++;
	}

	g_print("\n");
	g_print(">>> ");

}

static void
_post_parse_command(gchar *cmd)
{
	gint i = 0;

	while (g_post_menu[i].cmd) {
		if (g_strcmp0(g_post_menu[i].cmd, cmd) == 0) {
			if (g_post_menu[i].func)
				g_post_menu[i].func();
			return;
		}
		i++;
	}
	post_display_menu();

}


static gboolean
_post_input_cb(GIOChannel *channel)
{
	gchar buf[MAX_STRING_LEN+3];
	gsize len;

	g_io_channel_read(channel, buf, MAX_STRING_LEN, &len);
	buf[len] = 0x00;
	g_strstrip(buf);

	_post_parse_command(buf);

	return TRUE;

}

int main(int argc, char **argv)
{
	int i = 0;
	status_background_state_e state = STATUS_BACKGROUND_STATE_STOP;
	loop = g_main_loop_new(NULL, FALSE);

	stdin_channel = g_io_channel_unix_new (0);
	g_io_add_watch(stdin_channel, G_IO_IN, (GIOFunc)_post_input_cb, NULL);

	post_display_menu();

	g_main_loop_run(loop);

	for (i = 0; i < STATUS_BACKGROUND_TYPE_NUM; i++)
		status_background_status_post(i, state);

	g_io_channel_unref(stdin_channel);
	g_main_loop_unref(loop);

	return 0;
}



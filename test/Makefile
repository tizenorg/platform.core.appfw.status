all:
	gcc -Wall post_test.c -o post_test `pkg-config status glib-2.0 --libs --cflags` -I.
	gcc -Wall monitor_test.c -o monitor_test `pkg-config status-monitor glib-2.0 --libs --cflags` -I.

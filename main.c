#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lv_examples/lv_examples.h"
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

void sigroutine(int signo)
{
	switch (signo)
	{
		case SIGALRM:
			//printf("Catch a signal -- SIGALRM \n");
			signal(SIGALRM, sigroutine);
			
			//struct timeval start;
			//gettimeofday(&start, NULL);
			//printf("time %ld ms\n", start.tv_sec * 1000 + start.tv_usec / 1000);
			
			lv_tick_inc(5);
		break;
	}
}

/**
 * Print the memory usage periodically
 * @param param
 */
static void memory_monitor(lv_task_t *param)
{
	(void)param; /*Unused*/

	lv_mem_monitor_t mon;
	lv_mem_monitor(&mon);
	printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n",
		(int)mon.total_size - mon.free_size, mon.used_pct, mon.frag_pct,
		(int)mon.free_biggest_size);
}

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics
 * library
 */
static void hal_init(void)
{
	/* Use the 'fbdev' driver */
	fbdev_init();
	
	/*Create a display buffer*/
	static lv_disp_buf_t disp_buf1;
	static lv_color_t buf1_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];
	lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);

	/*Create a display*/
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.buffer = &disp_buf1;
	disp_drv.flush_cb = fbdev_flush;
	lv_disp_drv_register(&disp_drv);

#if 1
	evdev_init();
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = evdev_read;
	lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv);
	
	/*Set a cursor for the mouse*/
	LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
	lv_obj_t * cursor_obj = lv_img_create(lv_scr_act(), NULL); /*Create an image object for the cursor */
	lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
	lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/
#endif

	//lv_indev_t *mouse = lv_indev_next(NULL);
	//lv_obj_t *cursor = lv_label_create(lv_scr_act(), NULL);
	//lv_label_set_recolor(cursor, true);
	//lv_label_set_text(cursor, "#ff0000 .cursor");
	//lv_indev_set_cursor(mouse, cursor);
	
	/* Optional:
	 * Create a memory monitor task which prints the memory usage in
	 * periodically.*/
	//lv_task_create(memory_monitor, 5000, LV_TASK_PRIO_MID, NULL);
	
	struct itimerval value, ovalue;

	//printf("process id is %d\n", getpid());
	signal(SIGALRM, sigroutine);

	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 5000;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 5000;
	setitimer(ITIMER_REAL, &value, &ovalue);
}

int main(void)
{
	/*Initialize LVGL*/
	lv_init();

	/*Initialize the HAL (display, input devices, tick) for LVGL*/
	hal_init();

	lv_demo_widgets();
	//lv_demo_benchmark();
	//lv_demo_keypad_encoder();
	//lv_demo_printer();
	//lv_demo_stress();
	//lv_ex_get_started_1();
	//lv_ex_get_started_2();
	//lv_ex_get_started_3();

	//lv_ex_style_1();
	//lv_ex_style_2();
	//lv_ex_style_3();
	//lv_ex_style_4();
	//lv_ex_style_5();
	//lv_ex_style_6();
	//lv_ex_style_7();
	//lv_ex_style_8();
	//lv_ex_style_9();
	//lv_ex_style_10();
	//lv_ex_style_11();

	while (1)
	{
		/* Periodically call the lv_task handler.
     		 * It could be done in a timer interrupt or an OS task too.*/
		lv_task_handler();
		//printf("lv_task_handler\n");
		
		usleep(5 * 1000);
		//printf("usleep\n");
	}

	return 0;
}

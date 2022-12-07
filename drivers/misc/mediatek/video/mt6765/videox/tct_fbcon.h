
#ifndef __TCT_FBCON_H__
#define __TCT_FBCON_H__

#ifdef __cplusplus
extern "C" {
#endif

enum TCT_STATUS {
	TCT_STATUS_OK = 0,

	TCT_STATUS_NOT_READY = -1,
	TCT_STATUS_INVALID_ARGUMENT = -2,
	TCT_STATUS_LOCK_FAIL = -3,
	TCT_STATUS_LCD_IN_SUSPEND = -4,
	TCT_STATUS_FATAL_ERROR = -10,
};


enum TCT_COLOR {
	TCT_COLOR_BLACK = 0x000000,
	TCT_COLOR_WHITE = 0xFFFFFF,
	TCT_COLOR_RED = 0xFF0000,
	TCT_COLOR_GREEN = 0x00FF00,
	TCT_COLOR_BLUE = 0x0000FF,
	TCT_COLOR_TURQUOISE = (TCT_COLOR_GREEN | TCT_COLOR_BLUE),
	TCT_COLOR_YELLOW = (TCT_COLOR_RED | TCT_COLOR_GREEN),
	TCT_COLOR_PINK = (TCT_COLOR_RED | TCT_COLOR_BLUE),
};


/* Display Assertion Layer API */

unsigned int TCT_GetLayerSize(void);
enum TCT_STATUS TCT_SetScreenColor(enum TCT_COLOR color);
enum TCT_STATUS tct_fb_Init(unsigned long layerVA, unsigned long layerPA);
enum TCT_STATUS TCT_SetColor(unsigned int fgColor, unsigned int bgColor);
enum TCT_STATUS TCT_Clean(void);
enum TCT_STATUS tct_fb_Printf(const char *fmt, ...);
//enum TCT_STATUS TCT_OnDispPowerOn(void);
//enum TCT_STATUS TCT_LowMemoryOn(void);
//enum DAL_STATUS TCT_LowMemoryOff(void);
//int is_TCT_Enabled(void);

#ifdef __cplusplus
}
#endif
#endif				/* __DISP_ASSERT_LAYER_H__ */


#include <linux/types.h>
#include "primary_display.h"
#include "ddp_hal.h"
#include "disp_drv_log.h"
#include "tct_fbcon.h"
#include "disp_assert_layer.h"
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include "ddp_mmp.h"
#include "disp_drv_platform.h"
#include "disp_session.h"
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/semaphore.h>
#include <asm/cacheflush.h>
#include <linux/module.h>

/* /common part */
#define TCT_BPP             (2)
#define TCT_WIDTH           (DISP_GetScreenWidth())
#define TCT_HEIGHT          (DISP_GetScreenHeight())

#ifdef CONFIG_MTK_FB_SUPPORT_ASSERTION_LAYER
/* #if defined(CONFIG_MTK_FB_SUPPORT_ASSERTION_LAYER) */

#include "mtkfb_console.h"

/* --------------------------------------------------------------------------- */


#define RGB888_To_RGB565(x) ((((x) & 0xF80000) >> 8) |                      \
			     (((x) & 0x00FC00) >> 5) |                      \
			     (((x) & 0x0000F8) >> 3))

#define MAKE_TWO_RGB565_COLOR(high, low)  (((low) << 16) | (high))

#if 0
DEFINE_SEMAPHORE(tct_fb_sem);

inline enum TCT_STATUS TCT_LOCK(void)
{
	if (down_interruptible(&tct_fb_sem)) {
		DISP_LOG_PRINT(ANDROID_LOG_WARN, "TCT", "Can't get semaphore in %s()\n", __func__);
		return TCT_STATUS_LOCK_FAIL;
	}
	return TCT_STATUS_OK;
}

#define DAL_UNLOCK() up(&dal_sem)




inline enum DISP_STATUS TCT_CHECK_DISP_RET(enum DISP_STATUS expr)
{
	enum DISP_STATUS ret = (expr);

	if (ret != DISP_STATUS_OK) {
		DISP_LOG_PRINT(ANDROID_LOG_WARN, "TCT",
			       "Warning: call DISP_XXX function failed in %s(), line: %d, ret: %d\n",
			       __func__, __LINE__, ret);
		return ret;
	}
	return DISP_STATUS_OK;
}
#endif

#define tct_LOG(fmt, arg...) DISP_LOG_PRINT(ANDROID_LOG_INFO, "tct_fb", fmt, ##arg)
/* --------------------------------------------------------------------------- */

static MFC_HANDLE tct_mfc_handle;
static void *tct_fb_addr;
static unsigned long tct_fb_pa;

/*static BOOL dal_enable_when_resume = FALSE;*/
//static bool tct_disable_when_resume;
static unsigned int tct_fg_color = RGB888_To_RGB565(TCT_COLOR_WHITE);
static unsigned int tct_bg_color = RGB888_To_RGB565(TCT_COLOR_BLACK);
static char tct_fb_print_buffer[1024];

//bool tct_shown;
//extern unsigned int isAEEEnabled;

#define TCT_FORMAT          (DISP_FORMAT_RGB565)
#define TCT_BG_COLOR        (tct_bg_color)
#define TCT_FG_COLOR        (tct_fg_color)

#if 0
#define DEVICE_ATTR0(_name, _mode, _show, _store) \
	struct device_attribute dev_attr0_##_name = __ATTR(_name, _mode, _show, _store)
#endif

int tct_fb_Printf(const char *fmt, ...);
/* --------------------------------------------------------------------------- */

extern u32 MTK_FB_PAGES;
uint32_t TCT_GetLayerSize(void)
{
	/* avoid lcdc read buffersize+1 issue */
	//return TCT_WIDTH * TCT_HEIGHT * TCT_BPP + 4096;
	return TCT_WIDTH * TCT_HEIGHT * MTK_FB_PAGES;
}


#if 0

#if 0
static enum DAL_STATUS DAL_SetRedScreen(uint32_t *addr)
{
	uint32_t i;
	const uint32_t BG_COLOR = MAKE_TWO_RGB565_COLOR(DAL_BG_COLOR, DAL_BG_COLOR);

	for (i = 0; i < DAL_GetLayerSize() / sizeof(uint32_t); ++i)
		*addr++ = BG_COLOR;
	return DAL_STATUS_OK;
}
#endif


enum DAL_STATUS DAL_Init(unsigned long layerVA, unsigned long layerPA)
{
	pr_debug("%s, layerVA=0x%lx, layerPA=0x%lx\n", __func__, layerVA, layerPA);

	printk(KERN_ERR "minluo DAL_Init 111\n");

	dal_fb_addr = (void *)layerVA;
	dal_fb_pa = layerPA;
	DAL_CHECK_MFC_RET(MFC_Open(&mfc_handle, dal_fb_addr,
				   DAL_WIDTH, DAL_HEIGHT, DAL_BPP, DAL_FG_COLOR, DAL_BG_COLOR));
	/* DAL_Clean(); */
	DAL_SetScreenColor(DAL_COLOR_RED);

	return DAL_STATUS_OK;
}

enum DAL_STATUS DAL_SetColor(unsigned int fgColor, unsigned int bgColor)
{
	if (mfc_handle == NULL)
		return DAL_STATUS_NOT_READY;

	DAL_LOCK();
	dal_fg_color = RGB888_To_RGB565(fgColor);
	dal_bg_color = RGB888_To_RGB565(bgColor);
	DAL_CHECK_MFC_RET(MFC_SetColor(mfc_handle, dal_fg_color, dal_bg_color));
	DAL_UNLOCK();

	return DAL_STATUS_OK;
}
EXPORT_SYMBOL(DAL_SetColor);

enum DAL_STATUS DAL_Dynamic_Change_FB_Layer(unsigned int isAEEEnabled)
{
	return DAL_STATUS_OK;
}

static int show_dal_layer(int enable)
{
	struct disp_session_input_config *session_input;
	struct disp_input_config *input;
	int ret;
	int session_id = MAKE_DISP_SESSION(DISP_SESSION_PRIMARY, 0);
	session_input = kzalloc(sizeof(*session_input), GFP_KERNEL);
	if (!session_input)
		return -ENOMEM;

	session_input->setter = SESSION_USER_AEE;
	session_input->config_layer_num = 1;
	session_input->session_id = session_id;
	input = &session_input->config[0];

	input->src_phy_addr = (void *)dal_fb_pa;
	input->layer_id = primary_display_get_option("ASSERT_LAYER");
	input->layer_enable = enable;
	input->src_offset_x = 0;
	input->src_offset_y = 0;
	input->src_width = DAL_WIDTH;
	input->src_height = DAL_HEIGHT;
	input->tgt_offset_x = 0;
	input->tgt_offset_y = 0;
	input->tgt_width = DAL_WIDTH;
	input->tgt_height = DAL_HEIGHT;
	input->alpha = 0x80;
	input->alpha_enable = 1;
	input->next_buff_idx = -1;
	input->src_pitch = DAL_WIDTH;
	input->src_fmt = DAL_FORMAT;
	input->next_buff_idx = -1;
	input->dirty_roi_num = 0;

	ret = primary_display_config_input_multiple(session_input);
	kfree(session_input);
	return ret;
}

enum DAL_STATUS DAL_Clean(void)
{
	/* const uint32_t BG_COLOR = MAKE_TWO_RGB565_COLOR(DAL_BG_COLOR, DAL_BG_COLOR); */
	enum DAL_STATUS ret = DAL_STATUS_OK;

	static int dal_clean_cnt;
	struct MFC_CONTEXT *ctxt = (struct MFC_CONTEXT *)mfc_handle;

	DISPERR("[MTKFB_DAL] DAL_Clean\n");
	if (mfc_handle == NULL)
		return DAL_STATUS_NOT_READY;


	mmprofile_log_ex(ddp_mmp_get_events()->dal_clean, MMPROFILE_FLAG_START, 0, 0);
	DAL_LOCK();
	if (MFC_ResetCursor(mfc_handle) != MFC_STATUS_OK) {
		DISPERR("mfc_handle = %p\n", mfc_handle);
		goto End;
	}
	ctxt->screen_color = 0;
	DAL_SetScreenColor(DAL_COLOR_RED);


	/* TODO: if dal_shown=false, and 3D enabled, mtkfb may disable UI layer, please modify 3D driver */
	if (isAEEEnabled == 1) {
		show_dal_layer(0);
		/* DAL disable, switch UI layer to default layer 3 */
		DISPERR("[DDP] isAEEEnabled from 1 to 0, %d\n", dal_clean_cnt++);
		isAEEEnabled = 0;
		DAL_Dynamic_Change_FB_Layer(isAEEEnabled); /* restore UI layer to DEFAULT_UI_LAYER */
	}

	dal_shown = false;
	dal_disable_when_resume = false;

	primary_display_trigger(0, NULL, 0);

End:
	DAL_UNLOCK();
	mmprofile_log_ex(ddp_mmp_get_events()->dal_clean, MMPROFILE_FLAG_END, 0, 0);
	return ret;
}
EXPORT_SYMBOL(DAL_Clean);

int is_DAL_Enabled(void)
{
	int ret = 0;

	ret = isAEEEnabled;
	return ret;
}

enum DAL_STATUS DAL_Printf(const char *fmt, ...)
{
	va_list args;
	uint i;
	enum DAL_STATUS ret = DAL_STATUS_OK;


	/* printk("[MTKFB_DAL] DAL_Printf mfc_handle=0x%08X, fmt=0x%08X\n", mfc_handle, fmt); */

	DISPFUNC();

	if (mfc_handle == NULL)
		return DAL_STATUS_NOT_READY;

	if (fmt == NULL)
		return DAL_STATUS_INVALID_ARGUMENT;

	mmprofile_log_ex(ddp_mmp_get_events()->dal_printf, MMPROFILE_FLAG_START, 0, 0);
	DAL_LOCK();
	if (isAEEEnabled == 0) {
		DISPERR("[DDP] isAEEEnabled from 0 to 1, ASSERT_LAYER=%d, dal_fb_pa 0x%lx\n",
		       primary_display_get_option("ASSERT_LAYER"), dal_fb_pa);

		isAEEEnabled = 1;
		DAL_Dynamic_Change_FB_Layer(isAEEEnabled); /* default_ui_layer config to changed_ui_layer */

		show_dal_layer(1);
	}
	va_start(args, fmt);
	i = vsprintf(dal_print_buffer, fmt, args);
	va_end(args);
	if (i >= ARRAY_SIZE(dal_print_buffer)) {
		DISPERR("[AEE]dal print buffer no space, i=%d\n", i);
		return -1;
	}
	DAL_CHECK_MFC_RET(MFC_Print(mfc_handle, dal_print_buffer));

	/* flush_cache_all(); */


	if (!dal_shown)
		dal_shown = true;

	ret = primary_display_trigger(0, NULL, 0);


	DAL_UNLOCK();

	mmprofile_log_ex(ddp_mmp_get_events()->dal_printf, MMPROFILE_FLAG_END, 0, 0);

	return ret;
}
EXPORT_SYMBOL(DAL_Printf);

enum DAL_STATUS DAL_OnDispPowerOn(void)
{
	return DAL_STATUS_OK;
}

#endif

extern  int show_dal_layer(int enable);
extern  enum DAL_STATUS DAL_LOCK(void);
extern void DAL_UNLOCK(void);
extern enum DAL_STATUS DAL_Dynamic_Change_FB_Layer(unsigned int isAEEEnabled);

extern unsigned int mtkfb_fm_auto_test(void);

inline enum MFC_STATUS TCT_CHECK_MFC_RET(enum MFC_STATUS expr)
{
	enum MFC_STATUS ret = expr;

	if (ret != MFC_STATUS_OK) {
		DISP_LOG_PRINT(ANDROID_LOG_WARN, "TCT",
			       "Warning: call MFC_XXX function failed in %s(), line: %d, ret: %d\n",
			       __func__, __LINE__, ret);
		return ret;
	}
	return MFC_STATUS_OK;
}


enum TCT_STATUS TCT_SetColor(unsigned int fgColor, unsigned int bgColor)
{
	if (tct_mfc_handle == NULL)
		return TCT_STATUS_NOT_READY;

	//DAL_LOCK();
	tct_fg_color = RGB888_To_RGB565(fgColor);
	tct_bg_color = RGB888_To_RGB565(bgColor);
	TCT_CHECK_MFC_RET(MFC_SetColor(tct_mfc_handle, tct_fg_color, tct_bg_color));
	//DAL_UNLOCK();

	return TCT_STATUS_OK;
}
EXPORT_SYMBOL(TCT_SetColor);

enum TCT_STATUS TCT_SetScreenColor(enum TCT_COLOR color)
{
	uint32_t i;
	uint32_t size;
	uint32_t BG_COLOR;
	struct MFC_CONTEXT *ctxt = NULL;
	uint32_t offset;
	unsigned int *addr;

	color = RGB888_To_RGB565(color);
	BG_COLOR = MAKE_TWO_RGB565_COLOR(color, color);

	ctxt = (struct MFC_CONTEXT *)tct_mfc_handle;
	if (!ctxt)
		return TCT_STATUS_FATAL_ERROR;
	if (ctxt->screen_color == color)
		return TCT_STATUS_OK;
	offset = MFC_Get_Cursor_Offset(tct_mfc_handle);
	addr = (unsigned int *)(ctxt->fb_addr + offset);

	size = TCT_GetLayerSize() - offset;
	for (i = 0; i < size / sizeof(uint32_t); ++i)
		*addr++ = 0x00000000;//BG_COLOR;
	ctxt->screen_color = color;

	return TCT_STATUS_OK;
}
EXPORT_SYMBOL(TCT_SetScreenColor);



enum TCT_STATUS TCT_Clean(void)
{
	UINT32 fbsize;
	unsigned int i = 0;
	unsigned int *addr;

	/* const uint32_t BG_COLOR = MAKE_TWO_RGB565_COLOR(DAL_BG_COLOR, DAL_BG_COLOR); */
	enum TCT_STATUS ret = TCT_STATUS_OK;

	//static int dal_clean_cnt;
	struct MFC_CONTEXT *ctxt = (struct MFC_CONTEXT *)tct_mfc_handle;

	DISPERR("[MTKFB_TCT] TCT_Clean\n");
	if (tct_mfc_handle == NULL)
		return TCT_STATUS_NOT_READY;

	mmprofile_log_ex(ddp_mmp_get_events()->dal_clean, MMPROFILE_FLAG_START, 0, 0);
	//DAL_LOCK();
	if (MFC_ResetCursor(tct_mfc_handle) != MFC_STATUS_OK) {
		DISPERR("mfc_handle = %p\n", tct_mfc_handle);
		goto End;
	}
	ctxt->screen_color = 0;
	//TCT_SetScreenColor(TCT_COLOR_BLACK);
	addr = (unsigned int *)(ctxt->fb_addr);
	fbsize =
	    ALIGN_TO(DISP_GetScreenWidth(),
		     MTK_FB_ALIGNMENT) * DISP_GetScreenHeight() * MTK_FB_PAGES;
	for (i = 0; i < fbsize; i++)
		*addr++ = 0x00000002;//color

	primary_display_trigger(0, NULL, 0);

End:
	return ret;
}

static struct class * tct_fbcon_class;
static struct device * tct_fbcon_dev;

int status = 1;

static ssize_t tct_fbcon_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int ret;

	ret = snprintf(buf, 50, "%d\n", status);
	return ret;
}

static ssize_t  tct_fbcon_store(struct device *dev,
				struct device_attribute *attr,
				const char  *buf, size_t size)
{
       //char *pData = NULL;

	 printk(KERN_ERR "minluo tct_fbcon_store size=%d,str=%s\n",(int)size,buf);

	 #if 0
        pData = kmalloc(size, GFP_KERNEL);
        if (!pData)
        {
            return -ENOMEM;
        }

       if (copy_from_user(pData, buf, size))
       {
            printk(KERN_ERR "minluo copy_from_user() failed\n");

            kfree(pData);
            return -EFAULT;
        }
	#endif
	//tct_fb_Printf(buf);

	tct_fb_Printf("Secure Boot Failure!\n");
	//kfree(pData);
	return size;
}

//static DEVICE_ATTR0(status, 0666, tct_fbcon_show, tct_fbcon_store);
static DEVICE_ATTR(tctfb, S_IWUSR | S_IRUGO,tct_fbcon_show, tct_fbcon_store);


int tct_fb_Init(unsigned long layerVA, unsigned long layerPA)
{
    int rc = 0;

	pr_debug("%s, layerVA=0x%lx, layerPA=0x%lx\n", __func__, layerVA, layerPA);
	printk(KERN_ERR "minluo DAL_Init 111\n");

	tct_fbcon_class = class_create(THIS_MODULE, "tct_fbcon");
	if (IS_ERR(tct_fbcon_class))
		pr_err("Failed to create class(tct_fbcon_class)!\n");

	tct_fbcon_dev = device_create(tct_fbcon_class, NULL, 0, NULL, "tct_fb");
			if (IS_ERR(tct_fbcon_dev))
				pr_err("Failed to create device!\n");
			rc = device_create_file(tct_fbcon_dev, &dev_attr_tctfb);
			if ( rc < 0)
				pr_err("Failed to create device file(%s)!\n", dev_attr_tctfb.attr.name);


	tct_fb_addr = (void *)layerVA;
	tct_fb_pa = layerPA;
	TCT_CHECK_MFC_RET(MFC_Open(&tct_mfc_handle, tct_fb_addr,
				   TCT_WIDTH, TCT_HEIGHT, TCT_BPP, TCT_FG_COLOR, TCT_BG_COLOR));
	//TCT_Clean();
	//TCT_SetScreenColor(TCT_COLOR_BLACK);

	return TCT_STATUS_OK;
}

#if 1
static int show_tct_layer(int enable)
{
	struct disp_session_input_config *session_input;
	struct disp_input_config *input;
	int ret;
	int session_id = MAKE_DISP_SESSION(DISP_SESSION_PRIMARY, 0);
	session_input = kzalloc(sizeof(*session_input), GFP_KERNEL);
	if (!session_input)
		return -ENOMEM;

	session_input->setter = SESSION_USER_AEE;
	session_input->config_layer_num = 1;
	session_input->session_id = session_id;
	input = &session_input->config[0];

	input->src_phy_addr = (void *)tct_fb_pa;
	input->layer_id = primary_display_get_option("FB_LAYER");//ASSERT_LAYER//FB_LAYER
	input->layer_enable = enable;
	input->src_offset_x = 0;
	input->src_offset_y = 0;
	input->src_width = TCT_WIDTH;
	input->src_height = TCT_HEIGHT;
	input->tgt_offset_x = 0;
	input->tgt_offset_y = 0;
	input->tgt_width = TCT_WIDTH;
	input->tgt_height = TCT_HEIGHT;
	input->alpha = 0x80;
	input->alpha_enable = 1;
	input->next_buff_idx = -1;
	input->src_pitch = TCT_WIDTH;
	input->src_fmt = TCT_FORMAT;
	input->next_buff_idx = -1;
	input->dirty_roi_num = 0;

	ret = primary_display_config_input_multiple(session_input);
	kfree(session_input);
	return ret;
}
#endif

enum TCT_STATUS tct_fb_SetCursor(MFC_HANDLE handle,unsigned int x,unsigned int y)
{
	struct MFC_CONTEXT *ctxt = (struct MFC_CONTEXT *)handle;

	if (!ctxt)
		return TCT_STATUS_INVALID_ARGUMENT;

	if (down_interruptible(&ctxt->sem)) {
		DISPERR("[MFC] ERROR: Can't get semaphore in %s()\n", __func__);
		return TCT_STATUS_LOCK_FAIL;
	}

	ctxt->cursor_row =y;
	ctxt->cursor_col = x;
	up(&ctxt->sem);

	return TCT_STATUS_OK;
}


int tct_fb_Printf(const char *fmt, ...)
{
	va_list args;
	uint i;
	int ret = 0;

	DISPFUNC();

	if (tct_mfc_handle == NULL)
		return -1;

	if (fmt == NULL)
		return -2;

	TCT_Clean();
	tct_fb_SetCursor(tct_mfc_handle,2,16); 

       show_tct_layer(1);

	va_start(args, fmt);
	i = vsprintf(tct_fb_print_buffer, fmt, args);
	va_end(args);
	if (i >= ARRAY_SIZE(tct_fb_print_buffer)) {
		DISPERR("[TCT_FB] print buffer no space, i=%d\n", i);
		return -1;
	}
	TCT_CHECK_MFC_RET(MFC_Print(tct_mfc_handle, tct_fb_print_buffer));

	ret = primary_display_trigger(0, NULL, 0);

	return ret;
}


EXPORT_SYMBOL(tct_fb_Printf);
/* ########################################################################## */
/* !CONFIG_MTK_FB_SUPPORT_ASSERTION_LAYER */
/* ########################################################################## */
#else

#endif /* CONFIG_MTK_FB_SUPPORT_ASSERTION_LAYER */

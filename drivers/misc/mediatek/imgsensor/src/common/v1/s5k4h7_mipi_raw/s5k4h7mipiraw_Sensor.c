

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"

#include "s5k4h7mipiraw_Sensor.h"

/*===FEATURE SWITH===*/
// #define FPTPDAFSUPPORT   //for pdaf switch
// #define FANPENGTAO   //for debug log

//#define NONCONTINUEMODE
/*===FEATURE SWITH===*/

/****************************Modify Following Strings for Debug****************************/
#define PFX "S5K4H7"
#define LOG_INF_NEW(format, args...)                                           \
	pr_err(PFX "[%s] " format, __FUNCTION__, ##args)
#define LOG_INF LOG_INF_NEW
#define LOG_1 LOG_INF("S5K4H7,MIPI 4LANE\n")
#define SENSORDB LOG_INF
/****************************   Modify end    *******************************************/

static DEFINE_SPINLOCK(imgsensor_drv_lock);

static imgsensor_info_struct imgsensor_info = {
    .sensor_id = S5K4H7_SENSOR_ID, //Sensor ID Value: 0x30C8//record sensor id defined in Kd_imgsensor.h

    .checksum_value = 0x52500dc0, //0x49c09f86,		//checksum value for Camera Auto Test

    .pre = {
        .pclk = 280000000,
        .linelength = 3688,
        .framelength = 2530,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 1632,
        .grabwindow_height = 1224,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 280000000,
        .max_framerate = 300,
    },
    .cap = {
        .pclk = 280000000,
        .linelength = 3688,
        .framelength = 2530,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 3264,
        .grabwindow_height = 2448,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 280000000,
        .max_framerate = 300,
    },
    .normal_video = {
        .pclk = 280000000,
        .linelength = 3688,
        .framelength = 2530,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 3264,
        .grabwindow_height = 2448,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 280000000,
        .max_framerate = 300,
    },
    .hs_video = {
        .pclk = 280000000,
        .linelength = 3688,
        .framelength = 632,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 640,
        .grabwindow_height = 480,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 280000000,
        .max_framerate = 1200,
    },
    .slim_video = {
        .pclk = 280000000,
        .linelength = 3688,
        .framelength = 2530,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 1280,
        .grabwindow_height = 720,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 280000000,
        .max_framerate = 300,
    },
    .custom1 = {
        .pclk = 560000000,
        .linelength = 5088,
        .framelength = 3668,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 2328,
        .grabwindow_height = 1752,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 264000000,
        .max_framerate = 300,
    },
    .custom2 = {
        .pclk = 560000000,
        .linelength = 5088,
        .framelength = 3668,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 2328,
        .grabwindow_height = 1752,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 264000000,
        .max_framerate = 300,
    },
    .custom3 = {
        .pclk = 560000000,
        .linelength = 5088,
        .framelength = 3668,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 2328,
        .grabwindow_height = 1752,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 264000000,
        .max_framerate = 300,
    },
    .custom4 = {
        .pclk = 560000000,
        .linelength = 5088,
        .framelength = 3668,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 2328,
        .grabwindow_height = 1752,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 264000000,
        .max_framerate = 300,
    },
    .custom5 = {
        .pclk = 560000000,
        .linelength = 5088,
        .framelength = 3668,
        .startx = 0,
        .starty = 0,
        .grabwindow_width = 2328,
        .grabwindow_height = 1752,
        .mipi_data_lp2hs_settle_dc = 85,
        .mipi_pixel_rate = 264000000,
        .max_framerate = 300,
    },

    .margin = 8,                     //sensor framelength & shutter margin
    .min_shutter = 5,                //min shutter
    .max_frame_length = 0xFFFF,      //REG0x0202 <=REG0x0340-5//max framelength by sensor register's limitation
    .ae_shut_delay_frame = 0,        //shutter delay frame for AE cycle, 2 frame with ispGain_delay-shut_delay=2-0=2
    .ae_sensor_gain_delay_frame = 0, //sensor gain delay frame for AE cycle,2 frame with ispGain_delay-sensor_gain_delay=2-0=2
    .ae_ispGain_delay_frame = 2,     //isp gain delay frame for AE cycle
    .ihdr_support = 0,               //1, support; 0,not support
    .ihdr_le_firstline = 0,          //1,le first ; 0, se first
    .sensor_mode_num = 5,            //support sensor mode num ,don't support Slow motion

    .cap_delay_frame = 1,        //enter capture delay frame num
    .pre_delay_frame = 1,        //enter preview delay frame num
    .video_delay_frame = 1,      //enter video delay frame num
    .hs_video_delay_frame = 1,   //enter high speed video  delay frame num
    .slim_video_delay_frame = 1, //enter slim video delay frame num
    .custom1_delay_frame = 1,
    .custom2_delay_frame = 1,
    .custom3_delay_frame = 1,
    .custom4_delay_frame = 1,
    .custom5_delay_frame = 1,

    .isp_driving_current = ISP_DRIVING_6MA,
    .sensor_interface_type = SENSOR_INTERFACE_TYPE_MIPI,
    .mipi_sensor_type = MIPI_OPHY_NCSI2,                     //0,MIPI_OPHY_NCSI2;  1,MIPI_OPHY_CSI2
    .mipi_settle_delay_mode = 1,                             //0,MIPI_SETTLEDELAY_AUTO; 1,MIPI_SETTLEDELAY_MANNUAL
    .sensor_output_dataformat = SENSOR_OUTPUT_FORMAT_RAW_Gb, //sensor output first pixel color
    .mclk = 24,
    .mipi_lane_num = SENSOR_MIPI_4_LANE,
    .i2c_addr_table = {0x5A, 0xff}, //record sensor support all write id addr, only supprt 4must end with 0xff
    .i2c_speed = 300,                     // i2c read/write speed
};

static imgsensor_struct imgsensor = {
	.mirror      = IMAGE_NORMAL, //mirrorflip information
	.sensor_mode =IMGSENSOR_MODE_INIT, //IMGSENSOR_MODE enum value,record current sensor mode,such as: INIT, Preview, Capture, Video,High Speed Video, Slim Video
	.shutter     = 0x200, //current shutter
	.gain        = 0x200, //current gain
	.dummy_pixel = 0, //current dummypixel
	.dummy_line  = 0, //current dummyline
	.current_fps = 0, //full size current fps : 24fps for PIP, 30fps for Normal or ZSD
	.autoflicker_en = KAL_FALSE, //auto flicker enable: KAL_FALSE for disable auto flicker, KAL_TRUE for enable auto flicker
	.test_pattern   = KAL_FALSE, //test pattern mode or not. KAL_FALSE for in test pattern mode, KAL_TRUE for normal output
	.current_scenario_id = MSDK_SCENARIO_ID_CAMERA_PREVIEW, //current scenario id
	.ihdr_en = KAL_FALSE, //sensor need support LE, SE with HDR feature
	.i2c_write_id = 0x5a, //record current sensor's i2c write id
};

/* Sensor output window information*/
static struct SENSOR_WINSIZE_INFO_STRUCT imgsensor_winsize_info[5] = {
	{ 3280, 2464,   8,   8, 3264, 2448, 1632, 1224, 0, 0, 1632, 1224, 0, 0, 1632, 1224 }, // Preview
	{ 3280, 2464,   8,   8, 3264, 2448, 3264, 2448, 0, 0, 3264, 2448, 0, 0, 3264, 2448 }, // capture
	{ 3280, 2464,   8,   8, 3264, 2448, 3264, 2448, 0, 0, 3264, 2448, 0, 0, 3264, 2448 }, // video
	{ 3280, 2464, 360, 272, 2560, 1920,  640,  480, 0, 0,  640,  480, 0, 0,  640,  480 }, //hight speed video
	{ 3280, 2464, 360, 512, 2560, 1440, 1280,  720, 0, 0, 1280,  720, 0, 0, 1280,  720 }, // slim video
};

static kal_uint16 read_cmos_sensor_byte(kal_uint16 addr)
{
	kal_uint16 get_byte = 0;
	char pu_send_cmd[2] = { (char)(addr >> 8), (char)(addr & 0xFF) };
	iReadRegI2C(pu_send_cmd, 2, (u8 *)&get_byte, 1, imgsensor.i2c_write_id);
	return get_byte;
}

static kal_uint16 read_cmos_sensor(kal_uint32 addr)
{
	kal_uint16 get_byte = 0;
	char pu_send_cmd[2] = { (char)(addr >> 8), (char)(addr & 0xFF) };
	iReadRegI2C(pu_send_cmd, 2, (u8 *)&get_byte, 1, imgsensor.i2c_write_id);
	return get_byte;
}

static void write_cmos_sensor_8(kal_uint32 addr, kal_uint32 para)
{
	char pu_send_cmd[3] = { (char)(addr >> 8), (char)(addr & 0xFF),
				(char)(para & 0xFF) };
	iWriteRegI2C(pu_send_cmd, 3, imgsensor.i2c_write_id);
}

static void write_cmos_sensor(kal_uint16 addr, kal_uint16 para)
{
	char pusendcmd[4] = { (char)(addr >> 8), (char)(addr & 0xFF),
			      (char)(para >> 8), (char)(para & 0xFF) };
	iWriteRegI2C(pusendcmd, 4, imgsensor.i2c_write_id);
}

static void set_dummy(void)
{
	LOG_INF("dummyline = %d, dummypixels = %d \n", imgsensor.dummy_line,
		imgsensor.dummy_pixel);
	/* you can set dummy by imgsensor.dummy_line and imgsensor.dummy_pixel, or you can set dummy by imgsensor.frame_length and imgsensor.line_length */
	write_cmos_sensor(0x0340, imgsensor.frame_length & 0xFFFF);
	write_cmos_sensor(0x0342, imgsensor.line_length & 0xFFFF);
} /*	set_dummy  */

static void set_max_framerate(UINT16 framerate, kal_bool min_framelength_en)
{
	kal_uint32 frame_length = imgsensor.frame_length;
	//unsigned long flags;

	LOG_INF("framerate = %d, min framelength should enable(%d) \n",
		framerate, min_framelength_en);

	frame_length = imgsensor.pclk / framerate * 10 / imgsensor.line_length;
	spin_lock(&imgsensor_drv_lock);
	imgsensor.frame_length = (frame_length > imgsensor.min_frame_length) ?
					 frame_length :
					 imgsensor.min_frame_length;
	imgsensor.dummy_line =
		imgsensor.frame_length - imgsensor.min_frame_length;

	if (imgsensor.frame_length > imgsensor_info.max_frame_length) {
		imgsensor.frame_length = imgsensor_info.max_frame_length;
		imgsensor.dummy_line =
			imgsensor.frame_length - imgsensor.min_frame_length;
	}
	if (min_framelength_en)
		imgsensor.min_frame_length = imgsensor.frame_length;
	spin_unlock(&imgsensor_drv_lock);
	set_dummy();
} /*	set_max_framerate  */

//begin 20200528 liujunting add for front camera
static void check_streamoff(void)
{
	unsigned int i = 0;
	int timeout = (10000 / imgsensor.current_fps) + 1;

	mdelay(3);
	for (i = 0; i < timeout; i++) {
		if (read_cmos_sensor_byte(0x0005) != 0xFF)
			mdelay(1);
		else
			break;
	}
	pr_debug("%s exit!\n", __func__);
}

static kal_uint32 streaming_control(kal_bool enable)
{
	pr_debug("streaming_enable(0=Sw Standby,1=streaming): %d\n", enable);

	if (enable) {
		//write_cmos_sensor(0x6214, 0x7970);
		write_cmos_sensor_8(0x0100, 0x01);
	} else {
		//write_cmos_sensor(0x6028, 0x4000);
		write_cmos_sensor_8(0x0100, 0x00);
		check_streamoff();
	}
	return ERROR_NONE;
}
//end 20200528 liujunting add for front camera

static void set_shutter(kal_uint16 shutter)
{
	unsigned long flags;
	kal_uint16 realtime_fps = 0;
	spin_lock_irqsave(&imgsensor_drv_lock, flags);
	imgsensor.shutter = shutter;
	spin_unlock_irqrestore(&imgsensor_drv_lock, flags);

	spin_lock(&imgsensor_drv_lock);
	if (shutter > imgsensor.min_frame_length - imgsensor_info.margin)
		imgsensor.frame_length = shutter + imgsensor_info.margin;
	else
		imgsensor.frame_length = imgsensor.min_frame_length;
	if (imgsensor.frame_length > imgsensor_info.max_frame_length)
		imgsensor.frame_length = imgsensor_info.max_frame_length;
	spin_unlock(&imgsensor_drv_lock);
	shutter = (shutter < imgsensor_info.min_shutter) ?
			  imgsensor_info.min_shutter :
			  shutter;
	shutter = (shutter >
		   (imgsensor_info.max_frame_length - imgsensor_info.margin)) ?
			  (imgsensor_info.max_frame_length -
			   imgsensor_info.margin) :
			  shutter;

	if (imgsensor.autoflicker_en) {
		realtime_fps = imgsensor.pclk / imgsensor.line_length * 10 /
			       imgsensor.frame_length;
		if (realtime_fps >= 297 && realtime_fps <= 305)
			set_max_framerate(296, 0);
		else if (realtime_fps >= 147 && realtime_fps <= 150)
			set_max_framerate(146, 0);
		else {
			// Extend frame length
			write_cmos_sensor(0x0340,
					  imgsensor.frame_length & 0xFFFF);
		}
	} else {
		// Extend frame length
		write_cmos_sensor(0x0340, imgsensor.frame_length & 0xFFFF);
	}

	// Update Shutter
	write_cmos_sensor(0x0202, shutter & 0xFFFF);
	LOG_INF("Exit! shutter =%d, framelength =%d\n", shutter,
		imgsensor.frame_length);
}

static kal_uint16 gain2reg(const kal_uint16 gain)
{
	kal_uint16 reg_gain = 0x0000;
	reg_gain = gain / 2;
	return (kal_uint16)reg_gain;
}

static kal_uint16 set_gain(kal_uint16 gain)
{
	kal_uint16 reg_gain;

	LOG_INF("set_gain %d \n", gain);
	//gain = 64 = 1x real gain.

	if (gain < BASEGAIN || gain > 16 * BASEGAIN) {
		LOG_INF("Error gain setting");
		if (gain < BASEGAIN)
			gain = BASEGAIN;
		else if (gain > 16 * BASEGAIN)
			gain = 16 * BASEGAIN;
	}

	reg_gain = gain2reg(gain);
	spin_lock(&imgsensor_drv_lock);
	imgsensor.gain = reg_gain;
	spin_unlock(&imgsensor_drv_lock);
	LOG_INF("gain = %d , reg_gain = 0x%x\n ", gain, reg_gain);

	write_cmos_sensor(0x0204, (reg_gain & 0xFFFF));
	return gain;
} /*	set_gain  */

static void set_mirror_flip(kal_uint8 image_mirror)
{
	LOG_INF("image_mirror = %d\n", image_mirror);

	spin_lock(&imgsensor_drv_lock);
	imgsensor.mirror = image_mirror;
	spin_unlock(&imgsensor_drv_lock);
	switch (image_mirror) {
	case IMAGE_NORMAL:
		write_cmos_sensor_8(0x0101, 0x00); //GR
		break;
	case IMAGE_H_MIRROR:
		write_cmos_sensor_8(0x0101, 0x01); //R
		break;
	case IMAGE_V_MIRROR:
		write_cmos_sensor_8(0x0101, 0x02); //B
		break;
	case IMAGE_HV_MIRROR:
		write_cmos_sensor_8(0x0101, 0x03); //GB
		break;
	default:
		LOG_INF("Error image_mirror setting\n");
	}
}

static void night_mode(kal_bool enable)
{
	/*No Need to implement this function*/
} /*	night_mode	*/
static void sensor_init(void)
{
	LOG_INF("E\n");
	write_cmos_sensor_8(0x0100, 0x00);
	write_cmos_sensor_8(0x0B05, 0x01);
	write_cmos_sensor_8(0x3074, 0x06);
	write_cmos_sensor_8(0x3075, 0x2F);
	write_cmos_sensor_8(0x308A, 0x20);
	write_cmos_sensor_8(0x308B, 0x08);
	write_cmos_sensor_8(0x308C, 0x0B);
	write_cmos_sensor_8(0x3081, 0x07);
	write_cmos_sensor_8(0x307B, 0x85);
	write_cmos_sensor_8(0x307A, 0x0A);
	write_cmos_sensor_8(0x3079, 0x0A);
	write_cmos_sensor_8(0x306E, 0x71);
	write_cmos_sensor_8(0x306F, 0x28);
	write_cmos_sensor_8(0x301F, 0x20);
	write_cmos_sensor_8(0x306B, 0x9A);
	write_cmos_sensor_8(0x3091, 0x1F);
	write_cmos_sensor_8(0x30C4, 0x06);
	write_cmos_sensor_8(0x3200, 0x09);
	write_cmos_sensor_8(0x306A, 0x79);
	write_cmos_sensor_8(0x30B0, 0xFF);
	write_cmos_sensor_8(0x306D, 0x08);
	write_cmos_sensor_8(0x3080, 0x00);
	write_cmos_sensor_8(0x3929, 0x3F);
	write_cmos_sensor_8(0x3084, 0x16);
	write_cmos_sensor_8(0x3070, 0x0F);
	write_cmos_sensor_8(0x3B45, 0x01);
	write_cmos_sensor_8(0x30C2, 0x05);
	write_cmos_sensor_8(0x3069, 0x87);
	write_cmos_sensor_8(0x3924, 0x7F);
	write_cmos_sensor_8(0x3925, 0xFD);
	write_cmos_sensor_8(0x3C08, 0xFF);
	write_cmos_sensor_8(0x3C09, 0xFF);
	write_cmos_sensor_8(0x3C31, 0xFF);
	write_cmos_sensor_8(0x3C32, 0xFF);
	write_cmos_sensor_8(0x0A02, 0x14);
} /*	sensor_init  */

static void preview_setting(void)
{
	LOG_INF("E\n");
	write_cmos_sensor_8(0x0100, 0x00);
	mdelay(33);
	write_cmos_sensor_8(0x0136, 0x18);
	write_cmos_sensor_8(0x0137, 0x00);
	write_cmos_sensor_8(0x0305, 0x06);
	write_cmos_sensor_8(0x0306, 0x00);
	write_cmos_sensor_8(0x0307, 0x8C);
	write_cmos_sensor_8(0x030D, 0x06);
	write_cmos_sensor_8(0x030E, 0x00);
	write_cmos_sensor_8(0x030F, 0xAF);
	write_cmos_sensor_8(0x3C1F, 0x00);
	write_cmos_sensor_8(0x3C17, 0x00);
	write_cmos_sensor_8(0x3C1C, 0x05);
	write_cmos_sensor_8(0x3C1D, 0x15);
	write_cmos_sensor_8(0x0301, 0x04);
	write_cmos_sensor_8(0x0820, 0x02);
	write_cmos_sensor_8(0x0821, 0xBC);
	write_cmos_sensor_8(0x0822, 0x00);
	write_cmos_sensor_8(0x0823, 0x00);
	write_cmos_sensor_8(0x0112, 0x0A);
	write_cmos_sensor_8(0x0113, 0x0A);
	write_cmos_sensor_8(0x0114, 0x03);
	write_cmos_sensor_8(0x3906, 0x00);
	write_cmos_sensor_8(0x0344, 0x00);
	write_cmos_sensor_8(0x0345, 0x08);
	write_cmos_sensor_8(0x0346, 0x00);
	write_cmos_sensor_8(0x0347, 0x08);
	write_cmos_sensor_8(0x0348, 0x0C);
	write_cmos_sensor_8(0x0349, 0xC7);
	write_cmos_sensor_8(0x034A, 0x09);
	write_cmos_sensor_8(0x034B, 0x97);
	write_cmos_sensor_8(0x034C, 0x06);
	write_cmos_sensor_8(0x034D, 0x60);
	write_cmos_sensor_8(0x034E, 0x04);
	write_cmos_sensor_8(0x034F, 0xC8);
	write_cmos_sensor_8(0x0900, 0x01);
	write_cmos_sensor_8(0x0901, 0x22);
	write_cmos_sensor_8(0x0381, 0x01);
	write_cmos_sensor_8(0x0383, 0x01);
	write_cmos_sensor_8(0x0385, 0x01);
	write_cmos_sensor_8(0x0387, 0x03);
	write_cmos_sensor_8(0x0101, 0x00);
	write_cmos_sensor_8(0x0340, 0x09);
	write_cmos_sensor_8(0x0341, 0xE2);
	write_cmos_sensor_8(0x0342, 0x0E);
	write_cmos_sensor_8(0x0343, 0x68);
	write_cmos_sensor_8(0x0200, 0x0D);
	write_cmos_sensor_8(0x0201, 0xD8);
	write_cmos_sensor_8(0x0202, 0x02);
	write_cmos_sensor_8(0x0203, 0x08);
	write_cmos_sensor_8(0x3400, 0x00);
	write_cmos_sensor_8(0x0100, 0x01);
} /*	preview_setting  */

static void capture_setting(void)
{
	LOG_INF("E! currefps:%d\n");
	write_cmos_sensor_8(0x0100, 0x00);
	mdelay(33);
	write_cmos_sensor_8(0x0136, 0x18);
	write_cmos_sensor_8(0x0137, 0x00);
	write_cmos_sensor_8(0x0305, 0x06);
	write_cmos_sensor_8(0x0306, 0x00);
	write_cmos_sensor_8(0x0307, 0x8C);
	write_cmos_sensor_8(0x030D, 0x06);
	write_cmos_sensor_8(0x030E, 0x00);
	write_cmos_sensor_8(0x030F, 0xAF);
	write_cmos_sensor_8(0x3C1F, 0x00);
	write_cmos_sensor_8(0x3C17, 0x00);
	write_cmos_sensor_8(0x3C1C, 0x05);
	write_cmos_sensor_8(0x3C1D, 0x15);
	write_cmos_sensor_8(0x0301, 0x04);
	write_cmos_sensor_8(0x0820, 0x02);
	write_cmos_sensor_8(0x0821, 0xBC);
	write_cmos_sensor_8(0x0822, 0x00);
	write_cmos_sensor_8(0x0823, 0x00);
	write_cmos_sensor_8(0x0112, 0x0A);
	write_cmos_sensor_8(0x0113, 0x0A);
	write_cmos_sensor_8(0x0114, 0x03);
	write_cmos_sensor_8(0x3906, 0x04);
	write_cmos_sensor_8(0x0344, 0x00);
	write_cmos_sensor_8(0x0345, 0x08);
	write_cmos_sensor_8(0x0346, 0x00);
	write_cmos_sensor_8(0x0347, 0x08);
	write_cmos_sensor_8(0x0348, 0x0C);
	write_cmos_sensor_8(0x0349, 0xC7);
	write_cmos_sensor_8(0x034A, 0x09);
	write_cmos_sensor_8(0x034B, 0x97);
	write_cmos_sensor_8(0x034C, 0x0C);
	write_cmos_sensor_8(0x034D, 0xC0);
	write_cmos_sensor_8(0x034E, 0x09);
	write_cmos_sensor_8(0x034F, 0x90);
	write_cmos_sensor_8(0x0900, 0x00);
	write_cmos_sensor_8(0x0901, 0x00);
	write_cmos_sensor_8(0x0381, 0x01);
	write_cmos_sensor_8(0x0383, 0x01);
	write_cmos_sensor_8(0x0385, 0x01);
	write_cmos_sensor_8(0x0387, 0x01);
	write_cmos_sensor_8(0x0101, 0x00);
	write_cmos_sensor_8(0x0340, 0x09);
	write_cmos_sensor_8(0x0341, 0xE2);
	write_cmos_sensor_8(0x0342, 0x0E);
	write_cmos_sensor_8(0x0343, 0x68);
	write_cmos_sensor_8(0x0200, 0x0D);
	write_cmos_sensor_8(0x0201, 0xD8);
	write_cmos_sensor_8(0x0202, 0x02);
	write_cmos_sensor_8(0x0203, 0x08);
	write_cmos_sensor_8(0x3400, 0x00);
	write_cmos_sensor_8(0x0100, 0x01);
}

static void normal_video_setting(void)
{
	LOG_INF("E! currefps:%d\n");
	write_cmos_sensor_8(0x0100, 0x00);
	mdelay(33);
	write_cmos_sensor_8(0x0136, 0x18);
	write_cmos_sensor_8(0x0137, 0x00);
	write_cmos_sensor_8(0x0305, 0x06);
	write_cmos_sensor_8(0x0306, 0x00);
	write_cmos_sensor_8(0x0307, 0x8C);
	write_cmos_sensor_8(0x030D, 0x06);
	write_cmos_sensor_8(0x030E, 0x00);
	write_cmos_sensor_8(0x030F, 0xAF);
	write_cmos_sensor_8(0x3C1F, 0x00);
	write_cmos_sensor_8(0x3C17, 0x00);
	write_cmos_sensor_8(0x3C1C, 0x05);
	write_cmos_sensor_8(0x3C1D, 0x15);
	write_cmos_sensor_8(0x0301, 0x04);
	write_cmos_sensor_8(0x0820, 0x02);
	write_cmos_sensor_8(0x0821, 0xBC);
	write_cmos_sensor_8(0x0822, 0x00);
	write_cmos_sensor_8(0x0823, 0x00);
	write_cmos_sensor_8(0x0112, 0x0A);
	write_cmos_sensor_8(0x0113, 0x0A);
	write_cmos_sensor_8(0x0114, 0x03);
	write_cmos_sensor_8(0x3906, 0x04);
	write_cmos_sensor_8(0x0344, 0x00);
	write_cmos_sensor_8(0x0345, 0x08);
	write_cmos_sensor_8(0x0346, 0x00);
	write_cmos_sensor_8(0x0347, 0x08);
	write_cmos_sensor_8(0x0348, 0x0C);
	write_cmos_sensor_8(0x0349, 0xC7);
	write_cmos_sensor_8(0x034A, 0x09);
	write_cmos_sensor_8(0x034B, 0x97);
	write_cmos_sensor_8(0x034C, 0x0C);
	write_cmos_sensor_8(0x034D, 0xC0);
	write_cmos_sensor_8(0x034E, 0x09);
	write_cmos_sensor_8(0x034F, 0x90);
	write_cmos_sensor_8(0x0900, 0x00);
	write_cmos_sensor_8(0x0901, 0x00);
	write_cmos_sensor_8(0x0381, 0x01);
	write_cmos_sensor_8(0x0383, 0x01);
	write_cmos_sensor_8(0x0385, 0x01);
	write_cmos_sensor_8(0x0387, 0x01);
	write_cmos_sensor_8(0x0101, 0x00);
	write_cmos_sensor_8(0x0340, 0x09);
	write_cmos_sensor_8(0x0341, 0xE2);
	write_cmos_sensor_8(0x0342, 0x0E);
	write_cmos_sensor_8(0x0343, 0x68);
	write_cmos_sensor_8(0x0200, 0x0D);
	write_cmos_sensor_8(0x0201, 0xD8);
	write_cmos_sensor_8(0x0202, 0x02);
	write_cmos_sensor_8(0x0203, 0x08);
	write_cmos_sensor_8(0x3400, 0x00);
	write_cmos_sensor_8(0x0100, 0x01);
}

static void hs_video_setting(void)
{
	LOG_INF("E\n");
	write_cmos_sensor_8(0x0100, 0x00);
	mdelay(33);
	write_cmos_sensor_8(0x0136, 0x18);
	write_cmos_sensor_8(0x0137, 0x00);
	write_cmos_sensor_8(0x0305, 0x06);
	write_cmos_sensor_8(0x0306, 0x00);
	write_cmos_sensor_8(0x0307, 0x8C);
	write_cmos_sensor_8(0x030D, 0x06);
	write_cmos_sensor_8(0x030E, 0x00);
	write_cmos_sensor_8(0x030F, 0xAF);
	write_cmos_sensor_8(0x3C1F, 0x00);
	write_cmos_sensor_8(0x3C17, 0x00);
	write_cmos_sensor_8(0x3C1C, 0x05);
	write_cmos_sensor_8(0x3C1D, 0x15);
	write_cmos_sensor_8(0x0301, 0x04);
	write_cmos_sensor_8(0x0820, 0x02);
	write_cmos_sensor_8(0x0821, 0xBC);
	write_cmos_sensor_8(0x0822, 0x00);
	write_cmos_sensor_8(0x0823, 0x00);
	write_cmos_sensor_8(0x0112, 0x0A);
	write_cmos_sensor_8(0x0113, 0x0A);
	write_cmos_sensor_8(0x0114, 0x03);
	write_cmos_sensor_8(0x3906, 0x00);
	write_cmos_sensor_8(0x0344, 0x01);
	write_cmos_sensor_8(0x0345, 0x68);
	write_cmos_sensor_8(0x0346, 0x01);
	write_cmos_sensor_8(0x0347, 0x10);
	write_cmos_sensor_8(0x0348, 0x0B);
	write_cmos_sensor_8(0x0349, 0x67);
	write_cmos_sensor_8(0x034A, 0x08);
	write_cmos_sensor_8(0x034B, 0x8F);
	write_cmos_sensor_8(0x034C, 0x02);
	write_cmos_sensor_8(0x034D, 0x80);
	write_cmos_sensor_8(0x034E, 0x01);
	write_cmos_sensor_8(0x034F, 0xE0);
	write_cmos_sensor_8(0x0900, 0x01);
	write_cmos_sensor_8(0x0901, 0x44);
	write_cmos_sensor_8(0x0381, 0x01);
	write_cmos_sensor_8(0x0383, 0x01);
	write_cmos_sensor_8(0x0385, 0x01);
	write_cmos_sensor_8(0x0387, 0x07);
	write_cmos_sensor_8(0x0101, 0x00);
	write_cmos_sensor_8(0x0340, 0x02);
	write_cmos_sensor_8(0x0341, 0x78);
	write_cmos_sensor_8(0x0342, 0x0E);
	write_cmos_sensor_8(0x0343, 0x68);
	write_cmos_sensor_8(0x0200, 0x0D);
	write_cmos_sensor_8(0x0201, 0xD8);
	write_cmos_sensor_8(0x0202, 0x02);
	write_cmos_sensor_8(0x0203, 0x08);
	write_cmos_sensor_8(0x3400, 0x00);
	write_cmos_sensor_8(0x0100, 0x01);
}

static void slim_video_setting(void)
{
	LOG_INF("E\n");
	write_cmos_sensor_8(0x0100, 0x00);
	mdelay(33);
	write_cmos_sensor_8(0x0136, 0x18);
	write_cmos_sensor_8(0x0137, 0x00);
	write_cmos_sensor_8(0x0305, 0x06);
	write_cmos_sensor_8(0x0306, 0x00);
	write_cmos_sensor_8(0x0307, 0x8C);
	write_cmos_sensor_8(0x030D, 0x06);
	write_cmos_sensor_8(0x030E, 0x00);
	write_cmos_sensor_8(0x030F, 0xAF);
	write_cmos_sensor_8(0x3C1F, 0x00);
	write_cmos_sensor_8(0x3C17, 0x00);
	write_cmos_sensor_8(0x3C1C, 0x05);
	write_cmos_sensor_8(0x3C1D, 0x15);
	write_cmos_sensor_8(0x0301, 0x04);
	write_cmos_sensor_8(0x0820, 0x02);
	write_cmos_sensor_8(0x0821, 0xBC);
	write_cmos_sensor_8(0x0822, 0x00);
	write_cmos_sensor_8(0x0823, 0x00);
	write_cmos_sensor_8(0x0112, 0x0A);
	write_cmos_sensor_8(0x0113, 0x0A);
	write_cmos_sensor_8(0x0114, 0x03);
	write_cmos_sensor_8(0x3906, 0x00);
	write_cmos_sensor_8(0x0344, 0x01);
	write_cmos_sensor_8(0x0345, 0x68);
	write_cmos_sensor_8(0x0346, 0x02);
	write_cmos_sensor_8(0x0347, 0x00);
	write_cmos_sensor_8(0x0348, 0x0B);
	write_cmos_sensor_8(0x0349, 0x67);
	write_cmos_sensor_8(0x034A, 0x07);
	write_cmos_sensor_8(0x034B, 0x9F);
	write_cmos_sensor_8(0x034C, 0x05);
	write_cmos_sensor_8(0x034D, 0x00);
	write_cmos_sensor_8(0x034E, 0x02);
	write_cmos_sensor_8(0x034F, 0xD0);
	write_cmos_sensor_8(0x0900, 0x01);
	write_cmos_sensor_8(0x0901, 0x22);
	write_cmos_sensor_8(0x0381, 0x01);
	write_cmos_sensor_8(0x0383, 0x01);
	write_cmos_sensor_8(0x0385, 0x01);
	write_cmos_sensor_8(0x0387, 0x03);
	write_cmos_sensor_8(0x0101, 0x00);
	write_cmos_sensor_8(0x0340, 0x09);
	write_cmos_sensor_8(0x0341, 0xE2);
	write_cmos_sensor_8(0x0342, 0x0E);
	write_cmos_sensor_8(0x0343, 0x68);
	write_cmos_sensor_8(0x0200, 0x0D);
	write_cmos_sensor_8(0x0201, 0xD8);
	write_cmos_sensor_8(0x0202, 0x02);
	write_cmos_sensor_8(0x0203, 0x08);
	write_cmos_sensor_8(0x3400, 0x00);
	write_cmos_sensor_8(0x0100, 0x01);
}

static kal_uint32 return_sensor_id(void)
{
	return ((read_cmos_sensor_byte(0x0000) << 8) |
		read_cmos_sensor_byte(0x0001));
}

unsigned int s5k4h7_otp_status = 0;

#define MODULE_OFFSET_FLAG 0
#define AWB_OFFSET_FLAG 10
#define AWB_REG_SIZE 9
#define AWB_GROUP_OFFSET 24
#define LSC_REG_SIZE 2

typedef struct module_info_struct {
	kal_uint8 mid;
	kal_uint8 afflag;
	kal_uint8 year;
	kal_uint8 month;
	kal_uint8 day;
	kal_uint8 lensid;
	kal_uint8 vcmid;
	kal_uint8 drivericid;
	kal_uint8 operationflag;
	kal_uint8 reserved[6];
} module_info __attribute__((aligned(1)));

typedef struct awb_info_struct {
	kal_uint8 r;
	kal_uint8 gr;
	kal_uint8 gb;
	kal_uint8 b;
	kal_uint8 golden_r;
	kal_uint8 golden_gr;
	kal_uint8 golden_gb;
	kal_uint8 golden_b;
} awb_info __attribute__((aligned(1)));

typedef struct af_info_struct {
	kal_uint8 macro_h;
	kal_uint8 macro_l;
	kal_uint8 inf_h;
	kal_uint8 inf_l;
	kal_uint8 reserved[2];
} af_info __attribute__((aligned(1)));

typedef struct otp_struct {
	kal_uint8 module_info_flag;
	module_info module_info_group1;
	kal_uint8 module_info_group1_chksum;
	module_info module_info_group2;
	kal_uint8 module_info_group2_chksum;

	kal_uint8 awb_flag;
	awb_info awb_group1;
	kal_uint8 awb_group1_chksum;
	awb_info awb_group2;
	kal_uint8 awb_group2_chksum;

	kal_uint8 af_flag;
	af_info af_group1;
	kal_uint8 af_group1_chksum;
	af_info af_group2;
	kal_uint8 af_group2_chksum;
	kal_uint8 lsc_flag;
	kal_uint8 lsc1_chksum;
	kal_uint8 lsc2_chksum;

	kal_uint8 lsc_buffer1[360];
	kal_uint8 lsc_buffer2[360];

	kal_uint16 rg;
	kal_uint16 bg;
	kal_uint16 rg_ratio_typical;
	kal_uint16 bg_ratio_typical;
} otp_data __attribute__((aligned(1)));

kal_uint8 otp_buffer[0x800];

static void read_otp(void)
{
	int i = 0, j = 0;
	LOG_INF("read_otp %d\n", sizeof(otp_data));

	write_cmos_sensor_8(0x0100, 0x01);
	mdelay(1);

#define READ_PAGE(ADDR, PAGE, SIZE)                                            \
	write_cmos_sensor_8(0x0a02, PAGE);                                     \
	mdelay(1);                                                             \
	write_cmos_sensor_8(0x0a00, 0x01);                                     \
	mdelay(1);                                                             \
	for (i = 0; i < SIZE; i++) {                                           \
		otp_buffer[j++] = read_cmos_sensor_byte(ADDR + i);                 \
		LOG_INF("lsc[%d] = 0x%x\n", j - 1, otp_buffer[j - 1]);             \
	}                                                                      \
	write_cmos_sensor_8(0x0a00, 0x00);                                     \
	mdelay(1);

	READ_PAGE(0x0A04, 21, (0x0A43 - 0x0A04 + 1));
	READ_PAGE(0x0A04, 22, (0x0A09 - 0x0A04 + 1));
	READ_PAGE(0x0A04, 1, 64);
	READ_PAGE(0x0A04, 2, 64);
	READ_PAGE(0x0A04, 3, 64);
	READ_PAGE(0x0A04, 4, 64);
	READ_PAGE(0x0A04, 5, 64);
	READ_PAGE(0x0A04, 6, 64);
	READ_PAGE(0x0A04, 7, 64);
	READ_PAGE(0x0A04, 8, 64);
	READ_PAGE(0x0A04, 9, 64);
	READ_PAGE(0x0A04, 10, 64);
	READ_PAGE(0x0A04, 11, 64);
	READ_PAGE(0x0A04, 12, (0x0A13 - 0x0A04 + 1));

	write_cmos_sensor_8(0x0a00, 0x00);
	mdelay(1);
}

static int read_module_info(void)
{
	kal_uint8 moduleChkSum;
	module_info *p_moduleInfo;
	otp_data *p_otp = (otp_data *)&otp_buffer;

	LOG_INF("module_info_flag 0x%x\n", p_otp->module_info_flag);

	if ((p_otp->module_info_flag & 0x0c) == 0x04) {
		moduleChkSum = p_otp->module_info_group1_chksum;
		p_moduleInfo = &p_otp->module_info_group1;
		LOG_INF("group0 is valid\n");
	} else if ((p_otp->module_info_flag & 0x03) == 0x01) {
		moduleChkSum = p_otp->module_info_group2_chksum;
		p_moduleInfo = &p_otp->module_info_group2;
		LOG_INF("group1 is valid\n");
	} else {
		LOG_INF("%s:invalid or empty opt data\n", __func__);
		return -1;
	}

	LOG_INF("module_id     = 0x%x\n", p_moduleInfo->mid);
	LOG_INF("afflag        = 0x%x\n", p_moduleInfo->afflag);
	LOG_INF("lensid        = 0x%x\n", p_moduleInfo->lensid);
	LOG_INF("vcmid         = 0x%x\n", p_moduleInfo->vcmid);
	LOG_INF("drivericid    = 0x%x\n", p_moduleInfo->drivericid);
	LOG_INF("operationflag = 0x%x\n", p_moduleInfo->operationflag);
	LOG_INF("date is %d-%d_%d\n", p_moduleInfo->year, p_moduleInfo->month,
		p_moduleInfo->day);

	s5k4h7_otp_status |= EEPROM_MODULEINFO_FLAG;

	if (p_moduleInfo->mid == 0x44 && p_moduleInfo->lensid == 0x51 &&
	    p_moduleInfo->vcmid == 0xd2 && p_moduleInfo->drivericid == 0x16 &&
	    p_moduleInfo->afflag == 0xaf)
		s5k4h7_otp_status |= EEPROM_MODULEINFO_VALUE;

#define SUM_MOD                                                            \
	p_moduleInfo->mid + p_moduleInfo->afflag + p_moduleInfo->year +        \
	p_moduleInfo->month + p_moduleInfo->day +                              \
	p_moduleInfo->lensid + p_moduleInfo->vcmid +                           \
	p_moduleInfo->drivericid + p_moduleInfo->operationflag

	if (((SUM_MOD) % 255 + 1) == moduleChkSum)
		s5k4h7_otp_status |= EEPROM_MODULEINFO_CHKSUM;

	LOG_INF("read_module_info cal chksum=%d, chksum = %d\n",
		((SUM_MOD) % 255 + 1), moduleChkSum);

	return 0;
}

static int read_wb_data(void)
{
	kal_uint8 awb_chksum;
	awb_info *p_awb;
	otp_data *p_otp = (otp_data *)&otp_buffer;

	LOG_INF("awb_flag=0x%x\n", p_otp->awb_flag);

	if ((p_otp->awb_flag & 0x0c) == 0x04) {
		awb_chksum = p_otp->awb_group1_chksum;
		p_awb = &p_otp->awb_group1;
		LOG_INF("AWB group0 is valid\n");
	} else if ((p_otp->awb_flag & 0x03) == 0x01) {
		awb_chksum = p_otp->awb_group2_chksum;
		p_awb = &p_otp->awb_group2;
		LOG_INF("AWB group1 is valid\n");
	} else {
		LOG_INF("%s:invalid or empty awb data", __func__);
		return -1;
	}

	s5k4h7_otp_status |= EEPROM_AWB_FLAG;

	LOG_INF("r=0x%x gr=0x%x\n", p_awb->r, p_awb->gr);
	LOG_INF("b=0x%x gb=0x%x\n", p_awb->b, p_awb->gb);
	LOG_INF("golden_r=0x%x golden_gr=0x%x\n", p_awb->golden_r,p_awb->golden_gr);
	LOG_INF("golden_b=0x%x golden_gb=0x%x\n", p_awb->golden_b,p_awb->golden_gb);

	p_otp->rg = 1024 * p_awb->r / p_awb->gr;
	p_otp->bg = 1024 * p_awb->b / p_awb->gb;
	p_otp->rg_ratio_typical = 1024 * p_awb->golden_r / p_awb->golden_gr;
	p_otp->rg_ratio_typical = 1024 * p_awb->golden_b / p_awb->golden_gb;

#define SUM_AWB                                                            \
	p_awb->r + p_awb->gr + p_awb->b + p_awb->gb + p_awb->golden_r +        \
		p_awb->golden_gr + p_awb->golden_b + p_awb->golden_gb

	if (((SUM_AWB) % 255 + 1) == awb_chksum)
		s5k4h7_otp_status |= EEPROM_AWB_CHKSUM;

	LOG_INF("awb_chksum=0x%x\n", awb_chksum);
	LOG_INF("calculate awb chksum: 0x%x\n", ((SUM_AWB) % 255 + 1));
	return 0;
}

static int read_af_data(void)
{
	kal_uint8 af_chksum;
	kal_uint16 af_inf,af_macro;
	af_info *p_af;
	otp_data *p_otp = (otp_data *)&otp_buffer;

	LOG_INF("af_flag=0x%x\n", p_otp->af_flag);

	if ((p_otp->af_flag & 0x0c) == 0x04) {
		af_chksum = p_otp->af_group1_chksum;
		p_af = &p_otp->af_group1;
		LOG_INF("af group0 is valid\n");
	} else if ((p_otp->af_flag & 0x03) == 0x01) {
		af_chksum = p_otp->af_group2_chksum;
		p_af = &p_otp->af_group2;
		LOG_INF("af group1 is valid\n");
	} else {
		LOG_INF("%s:invalid or empty af data", __func__);
		return -1;
	}

	af_inf   = (p_af->inf_h << 8) | p_af->inf_l;
	af_macro = (p_af->macro_h << 8) | p_af->macro_l;

	LOG_INF("macro_h=0x%x macro_l=0x%x af_macro=%d\n", p_af->macro_h, p_af->macro_l,af_macro);
	LOG_INF("inf_h=0x%x inf_l=0x%x af_inf=%d\n", p_af->inf_h, p_af->inf_l,af_inf);
	LOG_INF("af limit:macro(349,649) inf(133,433)\n");

	if((af_inf   >= 133 && af_inf   <= 433) &&
	   (af_macro >= 349 && af_macro <= 649)){
	s5k4h7_otp_status |= EEPROM_AF_FLAG;
	}

#define SUM_AF p_af->macro_h + p_af->macro_l + p_af->inf_h + p_af->inf_l

	if (((SUM_AF) % 255 + 1) == af_chksum)
		s5k4h7_otp_status |= EEPROM_AF_CHKSUM;

	LOG_INF("af_chksum=0x%x\n", af_chksum);
	LOG_INF("calculate af chksum: 0x%x\n", ((SUM_AF) % 255 + 1));
	return 0;
}

static int read_lsc_data(void)
{
	int i = 0, sum = 0;
	kal_uint8 lsc_chksum;
	kal_uint8 *lsc_buffer;

	otp_data *p_otp = (otp_data *)&otp_buffer;

	LOG_INF("lsc_flag=0x%x\n", p_otp->lsc_flag);

	if ((p_otp->lsc_flag & 0x0c) == 0x04) {
		lsc_chksum = p_otp->lsc1_chksum;
		lsc_buffer = p_otp->lsc_buffer1;
		LOG_INF("lsc group0 is valid\n");
	} else if ((p_otp->lsc_flag & 0x03) == 0x01) {
		lsc_chksum = p_otp->lsc2_chksum;
		lsc_buffer = p_otp->lsc_buffer2;
		LOG_INF("lsc group1 is valid\n");
	} else {
		LOG_INF("lsc is invalid\n");
		return 0;
	}

	s5k4h7_otp_status |= EEPROM_LSC_FLAG;

	for (i = 0; i < 360; i++)
		sum += lsc_buffer[i];

	LOG_INF("lsc_chksum=0x%x\n", lsc_chksum);
	LOG_INF("calculate lsc_chksum=0x%x\n", (sum % 255) + 1);

	if (((sum % 255) + 1) == lsc_chksum)
		s5k4h7_otp_status |= EEPROM_LSC_CHKSUM;

	return 0;
}

static int check_s5k4h7_otp(void)
{
	read_otp();
	read_module_info();
	read_wb_data();
	read_af_data();
	read_lsc_data();
	LOG_INF("s5k4h7_otp_status = 0x%x\n", s5k4h7_otp_status);

	if (s5k4h7_otp_status ==
	    (EEPROM_MODULEINFO_FLAG | EEPROM_MODULEINFO_VALUE |EEPROM_MODULEINFO_CHKSUM | 
		 EEPROM_AWB_FLAG | EEPROM_AWB_CHKSUM |
	     EEPROM_AF_FLAG | EEPROM_AF_CHKSUM | 
		 EEPROM_LSC_FLAG|EEPROM_LSC_CHKSUM))
		return 1;

	return 0;
}

static void update_awb(void)
{
	kal_uint16 R_gain = 0, B_gain = 0, G_gain = 0, Base_gain = 0;
	kal_uint16 rg, bg, rg_ratio_typical, bg_ratio_typical;

	otp_data *p_otp = (otp_data *)&otp_buffer;

	rg = p_otp->rg;
	bg = p_otp->bg;

	rg_ratio_typical = p_otp->rg_ratio_typical;
	bg_ratio_typical = p_otp->bg_ratio_typical;

	LOG_INF("rg_ratio=0x%x,bg_ratio=0x%x,golden_rg_ratio=0x%x,golden_bg_ratio=0x%x",
		rg, bg, rg_ratio_typical, bg_ratio_typical);

	if (rg != 0 && bg != 0) {
		R_gain = (rg_ratio_typical * 1000) / rg;
		B_gain = (bg_ratio_typical * 1000) / bg;
		G_gain = 1000;

		LOG_INF(" R_gain=0x%x, G_gain=0x%x, B_gain=0x%x\n", R_gain,
			G_gain, B_gain);
	} else {
		LOG_INF("rg, bg = 0\n");
		return;
	}

	if (R_gain < 1000 || B_gain < 1000) {
		if (R_gain < B_gain)
			Base_gain = R_gain;
		else
			Base_gain = B_gain;
	} else {
		Base_gain = G_gain;
	}

	R_gain = 0x100 * R_gain / (Base_gain);
	B_gain = 0x100 * B_gain / (Base_gain);
	G_gain = 0x100 * G_gain / (Base_gain);

	LOG_INF("R_gain=0x%x, G_gain=0x%x, B_gain=0x%x", R_gain, G_gain,B_gain);

	write_cmos_sensor_8(0x3C0F, 0x00);

	if (R_gain > 0x100) {
		write_cmos_sensor_8(0x0210, R_gain >> 8);
		write_cmos_sensor_8(0x0211, R_gain & 0xff);
	}

	if (G_gain > 0x100) {
		write_cmos_sensor_8(0x020e, G_gain >> 8);
		write_cmos_sensor_8(0x020f, G_gain & 0xff);
		write_cmos_sensor_8(0x0214, G_gain >> 8);
		write_cmos_sensor_8(0x0215, G_gain & 0xff);
	}

	if (B_gain > 0x100) {
		write_cmos_sensor_8(0x0212, B_gain >> 8);
		write_cmos_sensor_8(0x0213, B_gain & 0xff);
	}
}

void update_lensshading(void)
{
	write_cmos_sensor_8(0x3400, 0x00);
	mdelay(1);
	write_cmos_sensor_8(0x0B00, 0x01);
	mdelay(1);
}

static kal_uint32 get_imgsensor_id(UINT32 *sensor_id)
{
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
	//sensor have two i2c address 0x6c 0x6d & 0x21 0x20, we should detect the module used i2c address
	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		do {
			*sensor_id = return_sensor_id();
			if (*sensor_id == imgsensor_info.sensor_id) {
				LOG_INF("i2c write id: 0x%x, ReadOut sensor id: 0x%x, imgsensor_info.sensor_id:0x%x.\n",
					imgsensor.i2c_write_id, *sensor_id,
					imgsensor_info.sensor_id);

				if (check_s5k4h7_otp())
					*sensor_id |= 0x01000000;
				return ERROR_NONE;
			}
			LOG_INF("Read sensor id fail, i2c write id: 0x%x, ReadOut sensor id: 0x%x, imgsensor_info.sensor_id:0x%x.\n",
				imgsensor.i2c_write_id, *sensor_id,
				imgsensor_info.sensor_id);
			retry--;
		} while (retry > 0);
		i++;
		retry = 1;
	}
	if (*sensor_id != imgsensor_info.sensor_id) {
		// if Sensor ID is not correct, Must set *sensor_id to 0xFFFFFFFF
		*sensor_id = 0xFFFFFFFF;
		return ERROR_SENSOR_CONNECT_FAIL;
	}
	return ERROR_NONE;
}

static kal_uint32 open(void)
{
	//const kal_uint8 i2c_addr[] = {IMGSENSOR_WRITE_ID_1, IMGSENSOR_WRITE_ID_2};
	kal_uint8 i = 0;
	kal_uint8 retry = 2;
	kal_uint32 sensor_id = 0;
	LOG_1;
	//sensor have two i2c address 0x6c 0x6d & 0x21 0x20, we should detect the module used i2c address
	while (imgsensor_info.i2c_addr_table[i] != 0xff) {
		spin_lock(&imgsensor_drv_lock);
		imgsensor.i2c_write_id = imgsensor_info.i2c_addr_table[i];
		spin_unlock(&imgsensor_drv_lock);
		do {
			sensor_id = return_sensor_id();
			if (sensor_id == imgsensor_info.sensor_id) {
				LOG_INF("i2c write id: 0x%x, sensor id: 0x%x\n",
					imgsensor.i2c_write_id, sensor_id);
				break;
			}
			LOG_INF("Read sensor id fail, id: 0x%x, sensor id: 0x%x\n",
				imgsensor.i2c_write_id, sensor_id);
			retry--;
		} while (retry > 0);
		i++;
		if (sensor_id == imgsensor_info.sensor_id)
			break;
		retry = 2;
	}
	if (imgsensor_info.sensor_id != sensor_id)
		return ERROR_SENSOR_CONNECT_FAIL;

	/* initail sequence write in  */
	sensor_init();
	update_awb();
	update_lensshading();
	spin_lock(&imgsensor_drv_lock);

	imgsensor.autoflicker_en = KAL_FALSE;
	imgsensor.sensor_mode = IMGSENSOR_MODE_INIT;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.dummy_pixel = 0;
	imgsensor.dummy_line = 0;
	imgsensor.ihdr_en = KAL_FALSE;
	imgsensor.test_pattern = KAL_FALSE;
	imgsensor.current_fps = imgsensor_info.pre.max_framerate;
	spin_unlock(&imgsensor_drv_lock);

	return ERROR_NONE;
} /*  open  */

static kal_uint32 close(void)
{
	LOG_INF("E\n");

	/*No Need to implement this function*/

	return ERROR_NONE;
} /*	close  */

static kal_uint32 preview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");
	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_PREVIEW;
	imgsensor.pclk = imgsensor_info.pre.pclk;
	imgsensor.line_length = imgsensor_info.pre.linelength;
	imgsensor.frame_length = imgsensor_info.pre.framelength;
	imgsensor.min_frame_length = imgsensor_info.pre.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	set_mirror_flip(IMAGE_HV_MIRROR);
	return ERROR_NONE;
} /*	preview   */

static kal_uint32 capture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");
	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CAPTURE;
	imgsensor.pclk = imgsensor_info.cap.pclk;
	imgsensor.line_length = imgsensor_info.cap.linelength;
	imgsensor.frame_length = imgsensor_info.cap.framelength;
	imgsensor.min_frame_length = imgsensor_info.cap.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	capture_setting();
	set_mirror_flip(IMAGE_HV_MIRROR);
	return ERROR_NONE;
} /* capture() */

static kal_uint32 normal_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			       MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_VIDEO;
	imgsensor.pclk = imgsensor_info.normal_video.pclk;
	imgsensor.line_length = imgsensor_info.normal_video.linelength;
	imgsensor.frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.normal_video.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	normal_video_setting();
	set_mirror_flip(IMAGE_HV_MIRROR);
	return ERROR_NONE;
} /*	normal_video   */

static kal_uint32 hs_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			   MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");
	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_HIGH_SPEED_VIDEO;
	imgsensor.pclk = imgsensor_info.hs_video.pclk;
	imgsensor.line_length = imgsensor_info.hs_video.linelength;
	imgsensor.frame_length = imgsensor_info.hs_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.hs_video.framelength;
	imgsensor.dummy_line = 0;
	imgsensor.dummy_pixel = 0;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	hs_video_setting();
	set_mirror_flip(IMAGE_HV_MIRROR);
	return ERROR_NONE;
} /*	hs_video   */

static kal_uint32 slim_video(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			     MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");
	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_SLIM_VIDEO;
	imgsensor.pclk = imgsensor_info.slim_video.pclk;
	imgsensor.line_length = imgsensor_info.slim_video.linelength;
	imgsensor.frame_length = imgsensor_info.slim_video.framelength;
	imgsensor.min_frame_length = imgsensor_info.slim_video.framelength;
	imgsensor.dummy_line = 0;
	imgsensor.dummy_pixel = 0;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	slim_video_setting();
	set_mirror_flip(IMAGE_HV_MIRROR);
	return ERROR_NONE;
}

static kal_uint32 Custom1(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM1;
	imgsensor.pclk = imgsensor_info.custom1.pclk;
	//imgsensor.video_mode = KAL_FALSE;
	imgsensor.line_length = imgsensor_info.custom1.linelength;
	imgsensor.frame_length = imgsensor_info.custom1.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom1.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	return ERROR_NONE;
} /*  Custom1   */

static kal_uint32 Custom2(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM2;
	imgsensor.pclk = imgsensor_info.custom2.pclk;
	//imgsensor.video_mode = KAL_FALSE;
	imgsensor.line_length = imgsensor_info.custom2.linelength;
	imgsensor.frame_length = imgsensor_info.custom2.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom2.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	return ERROR_NONE;
} /*  Custom2   */

static kal_uint32 Custom3(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM3;
	imgsensor.pclk = imgsensor_info.custom3.pclk;
	//imgsensor.video_mode = KAL_FALSE;
	imgsensor.line_length = imgsensor_info.custom3.linelength;
	imgsensor.frame_length = imgsensor_info.custom3.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom3.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	return ERROR_NONE;
} /*  Custom3   */

static kal_uint32 Custom4(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM4;
	imgsensor.pclk = imgsensor_info.custom4.pclk;
	//imgsensor.video_mode = KAL_FALSE;
	imgsensor.line_length = imgsensor_info.custom4.linelength;
	imgsensor.frame_length = imgsensor_info.custom4.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom4.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	return ERROR_NONE;
} /*  Custom4   */
static kal_uint32 Custom5(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("E\n");

	spin_lock(&imgsensor_drv_lock);
	imgsensor.sensor_mode = IMGSENSOR_MODE_CUSTOM5;
	imgsensor.pclk = imgsensor_info.custom5.pclk;
	//imgsensor.video_mode = KAL_FALSE;
	imgsensor.line_length = imgsensor_info.custom5.linelength;
	imgsensor.frame_length = imgsensor_info.custom5.framelength;
	imgsensor.min_frame_length = imgsensor_info.custom5.framelength;
	imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	preview_setting();
	return ERROR_NONE;
} /*  Custom5   */
static kal_uint32
get_resolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *sensor_resolution)
{
	LOG_INF("E\n");
	sensor_resolution->SensorFullWidth =
		imgsensor_info.cap.grabwindow_width;
	sensor_resolution->SensorFullHeight =
		imgsensor_info.cap.grabwindow_height;

	sensor_resolution->SensorPreviewWidth =
		imgsensor_info.pre.grabwindow_width;
	sensor_resolution->SensorPreviewHeight =
		imgsensor_info.pre.grabwindow_height;

	sensor_resolution->SensorVideoWidth =
		imgsensor_info.normal_video.grabwindow_width;
	sensor_resolution->SensorVideoHeight =
		imgsensor_info.normal_video.grabwindow_height;

	sensor_resolution->SensorHighSpeedVideoWidth =
		imgsensor_info.hs_video.grabwindow_width;
	sensor_resolution->SensorHighSpeedVideoHeight =
		imgsensor_info.hs_video.grabwindow_height;

	sensor_resolution->SensorSlimVideoWidth =
		imgsensor_info.slim_video.grabwindow_width;
	sensor_resolution->SensorSlimVideoHeight =
		imgsensor_info.slim_video.grabwindow_height;
	sensor_resolution->SensorCustom1Width =
		imgsensor_info.custom1.grabwindow_width;
	sensor_resolution->SensorCustom1Height =
		imgsensor_info.custom1.grabwindow_height;

	sensor_resolution->SensorCustom2Width =
		imgsensor_info.custom2.grabwindow_width;
	sensor_resolution->SensorCustom2Height =
		imgsensor_info.custom2.grabwindow_height;

	sensor_resolution->SensorCustom3Width =
		imgsensor_info.custom3.grabwindow_width;
	sensor_resolution->SensorCustom3Height =
		imgsensor_info.custom3.grabwindow_height;

	sensor_resolution->SensorCustom4Width =
		imgsensor_info.custom4.grabwindow_width;
	sensor_resolution->SensorCustom4Height =
		imgsensor_info.custom4.grabwindow_height;

	sensor_resolution->SensorCustom5Width =
		imgsensor_info.custom5.grabwindow_width;
	sensor_resolution->SensorCustom5Height =
		imgsensor_info.custom5.grabwindow_height;
	return ERROR_NONE;
} /*	get_resolution	*/

static kal_uint32 get_info(enum MSDK_SCENARIO_ID_ENUM scenario_id,
			   MSDK_SENSOR_INFO_STRUCT *sensor_info,
			   MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

	//sensor_info->SensorVideoFrameRate = imgsensor_info.normal_video.max_framerate/10; /* not use */
	//sensor_info->SensorStillCaptureFrameRate= imgsensor_info.cap.max_framerate/10; /* not use */
	//imgsensor_info->SensorWebCamCaptureFrameRate= imgsensor_info.v.max_framerate; /* not use */

	sensor_info->SensorClockPolarity = SENSOR_CLOCK_POLARITY_LOW;
	sensor_info->SensorClockFallingPolarity =
		SENSOR_CLOCK_POLARITY_LOW; /* not use */
	sensor_info->SensorHsyncPolarity =
		SENSOR_CLOCK_POLARITY_LOW; // inverse with datasheet
	sensor_info->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	sensor_info->SensorInterruptDelayLines = 4; /* not use */
	sensor_info->SensorResetActiveHigh = FALSE; /* not use */
	sensor_info->SensorResetDelayCount = 5; /* not use */

	sensor_info->SensroInterfaceType = imgsensor_info.sensor_interface_type;
	sensor_info->MIPIsensorType = imgsensor_info.mipi_sensor_type;
	sensor_info->SettleDelayMode = imgsensor_info.mipi_settle_delay_mode;
	sensor_info->SensorOutputDataFormat =
		imgsensor_info.sensor_output_dataformat;

	sensor_info->CaptureDelayFrame = imgsensor_info.cap_delay_frame;
	sensor_info->PreviewDelayFrame = imgsensor_info.pre_delay_frame;
	sensor_info->VideoDelayFrame = imgsensor_info.video_delay_frame;
	sensor_info->HighSpeedVideoDelayFrame =
		imgsensor_info.hs_video_delay_frame;
	sensor_info->SlimVideoDelayFrame =
		imgsensor_info.slim_video_delay_frame;
	sensor_info->SlimVideoDelayFrame =
		imgsensor_info.slim_video_delay_frame;
	sensor_info->Custom1DelayFrame = imgsensor_info.custom1_delay_frame;
	sensor_info->Custom2DelayFrame = imgsensor_info.custom2_delay_frame;
	sensor_info->Custom3DelayFrame = imgsensor_info.custom3_delay_frame;
	sensor_info->Custom4DelayFrame = imgsensor_info.custom4_delay_frame;
	sensor_info->Custom5DelayFrame = imgsensor_info.custom5_delay_frame;

	sensor_info->SensorMasterClockSwitch = 0; /* not use */
	sensor_info->SensorDrivingCurrent = imgsensor_info.isp_driving_current;

	sensor_info->AEShutDelayFrame =
		imgsensor_info
			.ae_shut_delay_frame; /* The frame of setting shutter default 0 for TG int */
	sensor_info->AESensorGainDelayFrame =
		imgsensor_info
			.ae_sensor_gain_delay_frame; /* The frame of setting sensor gain */
	sensor_info->AEISPGainDelayFrame =
		imgsensor_info.ae_ispGain_delay_frame;
	sensor_info->IHDR_Support = imgsensor_info.ihdr_support;
	sensor_info->IHDR_LE_FirstLine = imgsensor_info.ihdr_le_firstline;
	sensor_info->SensorModeNum = imgsensor_info.sensor_mode_num;

	sensor_info->SensorMIPILaneNumber = imgsensor_info.mipi_lane_num;
	sensor_info->SensorClockFreq = imgsensor_info.mclk;
	sensor_info->SensorClockDividCount = 3; /* not use */
	sensor_info->SensorClockRisingCount = 0;
	sensor_info->SensorClockFallingCount = 2; /* not use */
	sensor_info->SensorPixelClockCount = 3; /* not use */
	sensor_info->SensorDataLatchCount = 2; /* not use */

	sensor_info->MIPIDataLowPwr2HighSpeedTermDelayCount = 0;
	sensor_info->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
	sensor_info->SensorWidthSampling = 0; // 0 is default 1x
	sensor_info->SensorHightSampling = 0; // 0 is default 1x
	sensor_info->SensorPacketECCOrder = 1;
#ifdef FPTPDAFSUPPORT
	sensor_info->PDAF_Support = 1;
#else
	sensor_info->PDAF_Support = 0;
#endif

	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.pre.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		sensor_info->SensorGrabStartX = imgsensor_info.cap.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.cap.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.cap.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:

		sensor_info->SensorGrabStartX =
			imgsensor_info.normal_video.startx;
		sensor_info->SensorGrabStartY =
			imgsensor_info.normal_video.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.normal_video.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		sensor_info->SensorGrabStartX = imgsensor_info.hs_video.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.hs_video.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.hs_video.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		sensor_info->SensorGrabStartX =
			imgsensor_info.slim_video.startx;
		sensor_info->SensorGrabStartY =
			imgsensor_info.slim_video.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.slim_video.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CUSTOM1:
		sensor_info->SensorGrabStartX = imgsensor_info.custom1.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom1.starty;
		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom1.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		sensor_info->SensorGrabStartX = imgsensor_info.custom2.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom2.starty;
		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom1.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CUSTOM3:
		sensor_info->SensorGrabStartX = imgsensor_info.custom3.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom3.starty;
		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom1.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CUSTOM4:
		sensor_info->SensorGrabStartX = imgsensor_info.custom4.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom4.starty;
		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom1.mipi_data_lp2hs_settle_dc;

		break;
	case MSDK_SCENARIO_ID_CUSTOM5:
		sensor_info->SensorGrabStartX = imgsensor_info.custom5.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.custom5.starty;
		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.custom1.mipi_data_lp2hs_settle_dc;

		break;
	default:
		sensor_info->SensorGrabStartX = imgsensor_info.pre.startx;
		sensor_info->SensorGrabStartY = imgsensor_info.pre.starty;

		sensor_info->MIPIDataLowPwr2HighSpeedSettleDelayCount =
			imgsensor_info.pre.mipi_data_lp2hs_settle_dc;
		break;
	}

	return ERROR_NONE;
} /*	get_info  */

static kal_uint32 control(enum MSDK_SCENARIO_ID_ENUM scenario_id,
			  MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
			  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	LOG_INF("scenario_id = %d\n", scenario_id);
	spin_lock(&imgsensor_drv_lock);
	imgsensor.current_scenario_id = scenario_id;
	spin_unlock(&imgsensor_drv_lock);
	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		preview(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		capture(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		normal_video(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		hs_video(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		slim_video(image_window, sensor_config_data);
		break;
	case MSDK_SCENARIO_ID_CUSTOM1:
		Custom1(image_window, sensor_config_data); // Custom1
		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		Custom2(image_window, sensor_config_data); // Custom1
		break;
	case MSDK_SCENARIO_ID_CUSTOM3:
		Custom3(image_window, sensor_config_data); // Custom1
		break;
	case MSDK_SCENARIO_ID_CUSTOM4:
		Custom4(image_window, sensor_config_data); // Custom1
		break;
	case MSDK_SCENARIO_ID_CUSTOM5:
		Custom5(image_window, sensor_config_data); // Custom1
		break;
	default:
		LOG_INF("Error ScenarioId setting");
		preview(image_window, sensor_config_data);
		return ERROR_INVALID_SCENARIO_ID;
	}
	return ERROR_NONE;
} /* control() */

static kal_uint32 set_video_mode(UINT16 framerate)
{
	LOG_INF("framerate = %d\n ", framerate);
	// SetVideoMode Function should fix framerate
	if (framerate == 0)
		// Dynamic frame rate
		return ERROR_NONE;
	spin_lock(&imgsensor_drv_lock);
	if ((framerate == 300) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 296;
	else if ((framerate == 150) && (imgsensor.autoflicker_en == KAL_TRUE))
		imgsensor.current_fps = 146;
	else
		imgsensor.current_fps = framerate;
	spin_unlock(&imgsensor_drv_lock);
	set_max_framerate(imgsensor.current_fps, 1);

	return ERROR_NONE;
}

static kal_uint32 set_auto_flicker_mode(kal_bool enable, UINT16 framerate)
{
	LOG_INF("enable = %d, framerate = %d \n", enable, framerate);
	spin_lock(&imgsensor_drv_lock);
	if (enable) //enable auto flicker
		imgsensor.autoflicker_en = KAL_TRUE;
	else //Cancel Auto flick
		imgsensor.autoflicker_en = KAL_FALSE;
	spin_unlock(&imgsensor_drv_lock);
	return ERROR_NONE;
}

static kal_uint32
set_max_framerate_by_scenario(enum MSDK_SCENARIO_ID_ENUM scenario_id,
			      MUINT32 framerate)
{
	kal_uint32 frame_length;

	LOG_INF("scenario_id = %d, framerate = %d\n", scenario_id, framerate);

	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		frame_length = imgsensor_info.pre.pclk / framerate * 10 /
			       imgsensor_info.pre.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.pre.framelength) ?
				(frame_length -
				 imgsensor_info.pre.framelength) :
				0;
		imgsensor.frame_length =
			imgsensor_info.pre.framelength + imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		set_dummy();
		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		if (framerate == 0)
			return ERROR_NONE;
		frame_length = imgsensor_info.normal_video.pclk / framerate *
			       10 / imgsensor_info.normal_video.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length >
			 imgsensor_info.normal_video.framelength) ?
				(frame_length -
				 imgsensor_info.normal_video.framelength) :
				0;
		imgsensor.frame_length =
			imgsensor_info.normal_video.framelength +
			imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		set_dummy();
		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		/*Begin ersen.shang for T7698392 porting camera driver for fix s5k4h7 fps set issue*/
		//if (framerate == 300)
		{
			frame_length = imgsensor_info.cap.pclk / framerate *
				       10 / imgsensor_info.cap.linelength;
			spin_lock(&imgsensor_drv_lock);
			imgsensor.dummy_line =
				(frame_length >
				 imgsensor_info.cap.framelength) ?
					(frame_length -
					 imgsensor_info.cap.framelength) :
					0;
			imgsensor.frame_length =
				imgsensor_info.cap.framelength +
				imgsensor.dummy_line;
			imgsensor.min_frame_length = imgsensor.frame_length;
			spin_unlock(&imgsensor_drv_lock);
		} /*
        else
        {
            frame_length = imgsensor_info.cap1.pclk / framerate * 10 / imgsensor_info.cap1.linelength;
            spin_lock(&imgsensor_drv_lock);
            imgsensor.dummy_line = (frame_length > imgsensor_info.cap1.framelength) ? (frame_length - imgsensor_info.cap1.framelength) : 0;
            imgsensor.frame_length = imgsensor_info.cap1.framelength + imgsensor.dummy_line;
            imgsensor.min_frame_length = imgsensor.frame_length;
            spin_unlock(&imgsensor_drv_lock);
        }*/
		/*End   ersen.shang for T7698392 porting camera driver for fix s5k4h7 fps set issue*/
		set_dummy();
		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		frame_length = imgsensor_info.hs_video.pclk / framerate * 10 /
			       imgsensor_info.hs_video.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.hs_video.framelength) ?
				(frame_length -
				 imgsensor_info.hs_video.framelength) :
				0;
		imgsensor.frame_length = imgsensor_info.hs_video.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		set_dummy();
		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		frame_length = imgsensor_info.slim_video.pclk / framerate * 10 /
			       imgsensor_info.slim_video.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.slim_video.framelength) ?
				(frame_length -
				 imgsensor_info.slim_video.framelength) :
				0;
		imgsensor.frame_length = imgsensor_info.slim_video.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		set_dummy();
	case MSDK_SCENARIO_ID_CUSTOM1:
		frame_length = imgsensor_info.custom1.pclk / framerate * 10 /
			       imgsensor_info.custom1.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom1.framelength) ?
				(frame_length -
				 imgsensor_info.custom1.framelength) :
				0;
		if (imgsensor.dummy_line < 0)
			imgsensor.dummy_line = 0;
		imgsensor.frame_length = imgsensor_info.custom1.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		//set_dummy();
		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		frame_length = imgsensor_info.custom2.pclk / framerate * 10 /
			       imgsensor_info.custom2.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom2.framelength) ?
				(frame_length -
				 imgsensor_info.custom2.framelength) :
				0;
		if (imgsensor.dummy_line < 0)
			imgsensor.dummy_line = 0;
		imgsensor.frame_length = imgsensor_info.custom2.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		// set_dummy();
		break;
	case MSDK_SCENARIO_ID_CUSTOM3:
		frame_length = imgsensor_info.custom3.pclk / framerate * 10 /
			       imgsensor_info.custom3.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom3.framelength) ?
				(frame_length -
				 imgsensor_info.custom3.framelength) :
				0;
		if (imgsensor.dummy_line < 0)
			imgsensor.dummy_line = 0;
		imgsensor.frame_length = imgsensor_info.custom3.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		//set_dummy();
		break;
	case MSDK_SCENARIO_ID_CUSTOM4:
		frame_length = imgsensor_info.custom4.pclk / framerate * 10 /
			       imgsensor_info.custom4.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom4.framelength) ?
				(frame_length -
				 imgsensor_info.custom4.framelength) :
				0;
		if (imgsensor.dummy_line < 0)
			imgsensor.dummy_line = 0;
		imgsensor.frame_length = imgsensor_info.custom4.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		//set_dummy();
		break;
	case MSDK_SCENARIO_ID_CUSTOM5:
		frame_length = imgsensor_info.custom5.pclk / framerate * 10 /
			       imgsensor_info.custom5.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.custom5.framelength) ?
				(frame_length -
				 imgsensor_info.custom5.framelength) :
				0;
		if (imgsensor.dummy_line < 0)
			imgsensor.dummy_line = 0;
		imgsensor.frame_length = imgsensor_info.custom1.framelength +
					 imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		//set_dummy();
		break;
	default: //coding with  preview scenario by default
		frame_length = imgsensor_info.pre.pclk / framerate * 10 /
			       imgsensor_info.pre.linelength;
		spin_lock(&imgsensor_drv_lock);
		imgsensor.dummy_line =
			(frame_length > imgsensor_info.pre.framelength) ?
				(frame_length -
				 imgsensor_info.pre.framelength) :
				0;
		imgsensor.frame_length =
			imgsensor_info.pre.framelength + imgsensor.dummy_line;
		imgsensor.min_frame_length = imgsensor.frame_length;
		spin_unlock(&imgsensor_drv_lock);
		//set_dummy();
		LOG_INF("error scenario_id = %d, we use preview scenario \n",
			scenario_id);
		break;
	}
	return ERROR_NONE;
}

static kal_uint32
get_default_framerate_by_scenario(enum MSDK_SCENARIO_ID_ENUM scenario_id,
				  MUINT32 *framerate)
{
	LOG_INF("scenario_id = %d\n", scenario_id);

	switch (scenario_id) {
	case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		*framerate = imgsensor_info.pre.max_framerate;
		break;
	case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
		*framerate = imgsensor_info.normal_video.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
		*framerate = imgsensor_info.cap.max_framerate;
		break;
	case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
		*framerate = imgsensor_info.hs_video.max_framerate;
		break;
	case MSDK_SCENARIO_ID_SLIM_VIDEO:
		*framerate = imgsensor_info.slim_video.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM1:
		*framerate = imgsensor_info.custom1.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM2:
		*framerate = imgsensor_info.custom2.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM3:
		*framerate = imgsensor_info.custom3.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM4:
		*framerate = imgsensor_info.custom4.max_framerate;
		break;
	case MSDK_SCENARIO_ID_CUSTOM5:
		*framerate = imgsensor_info.custom5.max_framerate;
		break;
	default:
		break;
	}

	return ERROR_NONE;
}

static kal_uint32 set_test_pattern_mode(kal_bool enable)
{
	LOG_INF("enable: %d\n", enable);

	if (enable) {
		write_cmos_sensor(0x0600, 0x0002);
	} else {
		write_cmos_sensor(0x0600, 0x0000);
	}
	spin_lock(&imgsensor_drv_lock);
	imgsensor.test_pattern = enable;
	spin_unlock(&imgsensor_drv_lock);
	return ERROR_NONE;
}

static kal_uint32 feature_control(MSDK_SENSOR_FEATURE_ENUM feature_id,
				  UINT8 *feature_para, UINT32 *feature_para_len)
{
	UINT16 *feature_return_para_16 = (UINT16 *)feature_para;
	UINT16 *feature_data_16 = (UINT16 *)feature_para;
	UINT32 *feature_return_para_32 = (UINT32 *)feature_para;
	UINT32 *feature_data_32 = (UINT32 *)feature_para;
	unsigned long long *feature_data = (unsigned long long *)feature_para;

	struct SENSOR_WINSIZE_INFO_STRUCT *wininfo;
	MSDK_SENSOR_REG_INFO_STRUCT *sensor_reg_data =
		(MSDK_SENSOR_REG_INFO_STRUCT *)feature_para;
	//SET_PD_BLOCK_INFO_T *PDAFinfo;

	LOG_INF("feature_id = %d\n", feature_id);
	switch (feature_id) {
	case SENSOR_FEATURE_GET_PERIOD:
		*feature_return_para_16++ = imgsensor.line_length;
		*feature_return_para_16 = imgsensor.frame_length;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
		*feature_return_para_32 = imgsensor.pclk;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_SET_ESHUTTER:
		set_shutter(*feature_data);
		break;
	case SENSOR_FEATURE_SET_NIGHTMODE:
		night_mode((BOOL)*feature_data);
		break;
	case SENSOR_FEATURE_SET_GAIN:
		set_gain((UINT16)*feature_data);
		break;
	case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
	case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
		break;
	case SENSOR_FEATURE_SET_REGISTER:
		write_cmos_sensor(sensor_reg_data->RegAddr,
				  sensor_reg_data->RegData);
		break;
	case SENSOR_FEATURE_GET_REGISTER:
		sensor_reg_data->RegData =
			read_cmos_sensor(sensor_reg_data->RegAddr);
		break;
	case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
		// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
		// if EEPROM does not exist in camera module.
		*feature_return_para_32 = LENS_DRIVER_ID_DO_NOT_CARE;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_SET_VIDEO_MODE:
		set_video_mode(*feature_data);
		break;
	case SENSOR_FEATURE_CHECK_SENSOR_ID:
		get_imgsensor_id(feature_return_para_32);
		break;
	case SENSOR_FEATURE_SET_AUTO_FLICKER_MODE:
		set_auto_flicker_mode((BOOL)*feature_data_16,
				      *(feature_data_16 + 1));
		break;
	case SENSOR_FEATURE_SET_MAX_FRAME_RATE_BY_SCENARIO:
		set_max_framerate_by_scenario((enum MSDK_SCENARIO_ID_ENUM) *
						      feature_data,
					      *(feature_data + 1));
		break;
	case SENSOR_FEATURE_GET_DEFAULT_FRAME_RATE_BY_SCENARIO:
		get_default_framerate_by_scenario(
			(enum MSDK_SCENARIO_ID_ENUM) * (feature_data),
			(MUINT32 *)(uintptr_t)(*(feature_data + 1)));
		break;
	case SENSOR_FEATURE_SET_TEST_PATTERN:
		set_test_pattern_mode((BOOL)*feature_data);
		break;
	case SENSOR_FEATURE_GET_TEST_PATTERN_CHECKSUM_VALUE:
		*feature_return_para_32 = imgsensor_info.checksum_value;
		*feature_para_len = 4;
		break;
	case SENSOR_FEATURE_SET_FRAMERATE:
		LOG_INF("current fps :%d\n", (UINT32)*feature_data);
		spin_lock(&imgsensor_drv_lock);
		imgsensor.current_fps = *feature_data;
		spin_unlock(&imgsensor_drv_lock);
		break;
	case SENSOR_FEATURE_SET_HDR:
		LOG_INF("ihdr enable :%d\n", (BOOL)*feature_data);
		spin_lock(&imgsensor_drv_lock);
		imgsensor.ihdr_en = (BOOL)*feature_data;
		spin_unlock(&imgsensor_drv_lock);
		break;
	case SENSOR_FEATURE_GET_CROP_INFO:
		LOG_INF("SENSOR_FEATURE_GET_CROP_INFO scenarioId:%d\n",
			(UINT32)*feature_data);

		wininfo = (struct SENSOR_WINSIZE_INFO_STRUCT *)(uintptr_t)(
			*(feature_data + 1));

		switch (*feature_data_32) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			memcpy((void *)wininfo,
			       (void *)&imgsensor_winsize_info[1],
			       sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			memcpy((void *)wininfo,
			       (void *)&imgsensor_winsize_info[2],
			       sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			memcpy((void *)wininfo,
			       (void *)&imgsensor_winsize_info[3],
			       sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			memcpy((void *)wininfo,
			       (void *)&imgsensor_winsize_info[4],
			       sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			memcpy((void *)wininfo,
			       (void *)&imgsensor_winsize_info[0],
			       sizeof(struct SENSOR_WINSIZE_INFO_STRUCT));
			break;
		}
		break;
	case SENSOR_FEATURE_SET_IHDR_SHUTTER_GAIN:
		LOG_INF("SENSOR_SET_SENSOR_IHDR LE=%d, SE=%d, Gain=%d\n",
			(UINT16)*feature_data, (UINT16) * (feature_data + 1),
			(UINT16) * (feature_data + 2));
		//ihdr_write_shutter_gain((UINT16)*feature_data,(UINT16)*(feature_data+1),(UINT16)*(feature_data+2));
		break;
		//begin 20200528 liujunting add feature for front camera
	case SENSOR_FEATURE_SET_STREAMING_SUSPEND:
		pr_debug("SENSOR_FEATURE_SET_STREAMING_SUSPEND\n");
		streaming_control(KAL_FALSE);
		break;
	case SENSOR_FEATURE_SET_STREAMING_RESUME:
		pr_debug("SENSOR_FEATURE_SET_STREAMING_RESUME, shutter:%llu\n",
			 *feature_data);
		if (*feature_data != 0)
			set_shutter(*feature_data);
		streaming_control(KAL_TRUE);
		break;
	case SENSOR_FEATURE_GET_MIPI_PIXEL_RATE:
		switch (*feature_data) {
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1)) =
				imgsensor_info.cap.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1)) =
				imgsensor_info.normal_video.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1)) =
				imgsensor_info.hs_video.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_SLIM_VIDEO:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1)) =
				imgsensor_info.slim_video.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_CUSTOM1:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1)) =
				imgsensor_info.custom1.mipi_pixel_rate;
			break;
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		default:
			*(MUINT32 *)(uintptr_t)(*(feature_data + 1)) =
				imgsensor_info.pre.mipi_pixel_rate;
			break;
		}
		break;
//end 20200528 liujunting add feature for front camera
#if 0
    /******************** PDAF START >>> *********/
    case SENSOR_FEATURE_GET_SENSOR_PDAF_CAPACITY:
        LOG_INF("SENSOR_FEATURE_GET_SENSOR_PDAF_CAPACITY scenarioId:%llu\n", *feature_data);
        //PDAF capacity enable or not, 2p8 only full size support PDAF
        switch (*feature_data)
        {
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
            *(MUINT32 *)(uintptr_t)(*(feature_data + 1)) = 1;
            break;
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
            *(MUINT32 *)(uintptr_t)(*(feature_data + 1)) = 1;
            break;
        case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
            *(MUINT32 *)(uintptr_t)(*(feature_data + 1)) = 0;
            break;
        case MSDK_SCENARIO_ID_SLIM_VIDEO:
            *(MUINT32 *)(uintptr_t)(*(feature_data + 1)) = 0;
            break;
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
            *(MUINT32 *)(uintptr_t)(*(feature_data + 1)) = 0;
            break;
        default:
            *(MUINT32 *)(uintptr_t)(*(feature_data + 1)) = 0;
            break;
        }
        break;
    case SENSOR_FEATURE_GET_PDAF_INFO:
        LOG_INF("SENSOR_FEATURE_GET_PDAF_INFO scenarioId:%llu\n", *feature_data);
        PDAFinfo = (SET_PD_BLOCK_INFO_T *)(uintptr_t)(*(feature_data + 1));

        switch (*feature_data)
        {
        case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
        case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
            memcpy((void *)PDAFinfo, (void *)&imgsensor_pd_info, sizeof(SET_PD_BLOCK_INFO_T));
            break;
        case MSDK_SCENARIO_ID_HIGH_SPEED_VIDEO:
        case MSDK_SCENARIO_ID_SLIM_VIDEO:
        case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
        default:
            break;
        }
        break;
    case SENSOR_FEATURE_GET_PDAF_DATA:
        LOG_INF("SENSOR_FEATURE_GET_PDAF_DATA\n");
        //S5K4H7_read_eeprom((kal_uint16 )(*feature_data),(char*)(uintptr_t)(*(feature_data+1)),(kal_uint32)(*(feature_data+2)));
        break;
    /******************** PDAF END   <<< *********/
#endif
	default:
		break;
	}

	return ERROR_NONE;
} /*    feature_control()  */

static struct SENSOR_FUNCTION_STRUCT sensor_func = {
	open, get_info, get_resolution, feature_control, control, close
};

UINT32 S5K4H7_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc != NULL)
		*pfFunc = &sensor_func;
	return ERROR_NONE;
} /*	S5K4H7_MIPI_RAW_SensorInit	*/

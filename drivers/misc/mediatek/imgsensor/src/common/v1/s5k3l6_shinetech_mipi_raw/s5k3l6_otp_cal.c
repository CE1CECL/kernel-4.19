/************************************************* 
History: 1.0 Added by zhanyong.yin for XR 5581518 on 2017/11/20
*************************************************/
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
#include <linux/proc_fs.h> 
#include <linux/dma-mapping.h>
#include <linux/types.h>

#include "kd_camera_typedef.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"
#include "s5k3l6mipiraw_Sensor.h"

#define PFX "S5K3L6_OTP"
#define LOG_INF(format, args...)    pr_err(PFX "[%s] " format, __FUNCTION__, ##args)
#define Delay(ms)  mdelay(ms)

struct s5k3l6_otp_struct {
	int flag;
	int module_integrator_id;
	int lens_id;
	int production_year;
	int production_month;
	int production_day;
	int R_Gr_ratio;
	int B_Gr_ratio;
	int Gb_Gr_ratio;
	int g_R_Gr_ratio;
	int g_B_Gr_ratio;
	int g_Gb_Gr_ratio;
	int checksumAWB;
	int vcm_id;
	int vcm_ic_id;
	int checksumAF;
	int checksumLSC;
	int checksumOTP;
	int checksumTotal;
	int VCM_macro;
	int VCM_inf;
	int VCM_dir;
	int VCM_start;
};

struct s5k3l6_pdaf_struct {
	int checksumpdaf1;
	int checksumpdaf2;
};

static unsigned char S5K3L6MIPI_WRITE_ID = 0XA0;
int S5K3L6_OTP_Flag=0;
int S5K3L6_pdaf_Flag=0;
int S5K3L6_DUAL_CALI_FLAG=0;
static unsigned char lenc[1868];
static unsigned char pdaf1[496];
static unsigned char pdaf2[918];

kal_uint16 S5K3L6_R2A_read_i2c(kal_uint32 addr)
{
	kal_uint16 get_byte=0;
	
	char pu_send_cmd[2] = {(char)(addr >> 8), (char)(addr & 0xFF) };
	iReadRegI2C(pu_send_cmd, 2, (u8*)&get_byte, 1,S5K3L6MIPI_WRITE_ID);

	return get_byte;
}

void S5K3L6_R2A_write_i2c(u16 addr, u32 para)
{
	char pu_send_cmd[3] = {(char)(addr >> 8), (char)(addr & 0xFF), (char)(para & 0xFF)};
	iWriteRegI2C(pu_send_cmd, 3, S5K3L6MIPI_WRITE_ID);
}


int S5K3L6_read_pdaf(struct s5k3l6_pdaf_struct *pdaf_ptr)
{
	int pdaf_flag=0;
	int checksumpdaf1=0;
	int checksumpdaf2=0;
	int pdaf_offset1 = 0x079a;
	int pdaf_offset2 = 0x098b;
	int i=0;

	//pdaf
	pdaf_flag=S5K3L6_R2A_read_i2c(0x0799);
	LOG_INF("pdaf_flag : 0x%x \n", pdaf_flag);
	if(pdaf_flag==0x01){
		for(i = 0; i < 496; i++) {
			pdaf1[i]= S5K3L6_R2A_read_i2c(pdaf_offset1 + i);
			checksumpdaf1 += pdaf1[i];
		}
		checksumpdaf1 = (checksumpdaf1)%256;
		(* pdaf_ptr).checksumpdaf1=S5K3L6_R2A_read_i2c(0x098a);
		LOG_INF("checksumpdaf1:%d, checksumpdaf1 :%d\n", checksumpdaf1, (* pdaf_ptr).checksumpdaf1);

		for(i = 0; i < 918; i++)
		{
			pdaf2[i]= S5K3L6_R2A_read_i2c(pdaf_offset2 + i);
			checksumpdaf2 += pdaf2[i];
		}
		checksumpdaf2 = (checksumpdaf2)%256;
		(* pdaf_ptr).checksumpdaf2=S5K3L6_R2A_read_i2c(0x0d21);
		LOG_INF("checksumpdaf2:%d, checksumpdaf2 :%d\n", checksumpdaf2, (* pdaf_ptr).checksumpdaf2);

	}else{
		LOG_INF("no pdaf data\n");
	}
	
	if((pdaf_flag==0x01)&&(checksumpdaf1==(* pdaf_ptr).checksumpdaf1) && (checksumpdaf2==(* pdaf_ptr).checksumpdaf2)){
		S5K3L6_pdaf_Flag=1;
	}else{
		S5K3L6_pdaf_Flag=0;
	}
	return 0;
}

void s5k3l6_read_dualcamera_cali(void)
{
	unsigned int dual_cali_flag = 0x0;
	
	dual_cali_flag = S5K3L6_R2A_read_i2c(0x0D22);
	printk("bo_liu, s5k3l6 dual_cali_flag = 0x%x\n", dual_cali_flag);
	if (dual_cali_flag == 1)
	{
		S5K3L6_DUAL_CALI_FLAG = 1;
	}
	else
	{
		S5K3L6_DUAL_CALI_FLAG = 0;
	}
	

}

int S5K3L6_read_otp(struct s5k3l6_otp_struct *otp_ptr)
{
	int otp_flag=0;
	int awb_flag=0;
	int vcm_flag=0;
	int lsc_flag=0;
	int addr=0;
	int lsc_addr=0;
	int af_addr=0;
	int i=0;
	int checksum=0;
	int checksumAWB=0;
	int checksumAF=0;
	int checksumLSC = 0;

	otp_flag = S5K3L6_R2A_read_i2c(0x0000);
	LOG_INF(" module information : 0x%x\n", otp_flag);
	addr = 0x0000;

	if(otp_flag == 0x01) {
		(*otp_ptr).module_integrator_id = S5K3L6_R2A_read_i2c(addr+1);
		(*otp_ptr).lens_id = S5K3L6_R2A_read_i2c( addr + 5);
		(*otp_ptr).production_year = S5K3L6_R2A_read_i2c( addr + 2);
		(*otp_ptr).production_month = S5K3L6_R2A_read_i2c( addr + 3);
		(*otp_ptr).production_day = S5K3L6_R2A_read_i2c(addr + 4);
		(*otp_ptr).vcm_id = S5K3L6_R2A_read_i2c(addr + 6);
		(*otp_ptr).vcm_ic_id = S5K3L6_R2A_read_i2c(addr + 7);
		(*otp_ptr).checksumOTP = S5K3L6_R2A_read_i2c(addr + 0x15);
	}else {
		(*otp_ptr).module_integrator_id = 0;
		(*otp_ptr).lens_id = 0;
		(*otp_ptr).production_year = 0;
		(*otp_ptr).production_month = 0;
		(*otp_ptr).production_day = 0;
		(*otp_ptr).vcm_id=0;
		(*otp_ptr).vcm_ic_id=0;
		(*otp_ptr).checksumOTP = 0;
	}
	
	for(i=1;i<=0x14;i++){
		checksum+=S5K3L6_R2A_read_i2c(addr+i);
	}
	
	checksum = (checksum)%256;
	LOG_INF("otp_flag=%d,module_integrator_id=0x%x,lens_id=0x%x,data=%d-%d-%d\n",otp_flag,(*otp_ptr).module_integrator_id,(*otp_ptr).lens_id,(*otp_ptr).production_year,(*otp_ptr).production_month,(*otp_ptr).production_day);
	LOG_INF("vcm_id=0x%x,vcm_ic_id=0x%x",(*otp_ptr).vcm_id,(*otp_ptr).vcm_ic_id);
	LOG_INF("checksum =%d",checksum);
	LOG_INF("checksumOTP =%d",(*otp_ptr).checksumOTP);

	//OTP AWB information
	awb_flag=S5K3L6_R2A_read_i2c(0x0020);
	if(awb_flag==0x01){
		(*otp_ptr).R_Gr_ratio = (S5K3L6_R2A_read_i2c(0x0022)<<8) | (S5K3L6_R2A_read_i2c(0x0021));
		(*otp_ptr).B_Gr_ratio = (S5K3L6_R2A_read_i2c(0x0024)<<8) | (S5K3L6_R2A_read_i2c(0x0023));
		(*otp_ptr).Gb_Gr_ratio = (S5K3L6_R2A_read_i2c(0x0026)<<8) | (S5K3L6_R2A_read_i2c(0x0025));
		(*otp_ptr).g_R_Gr_ratio = (S5K3L6_R2A_read_i2c(0x0028)<<8) | (S5K3L6_R2A_read_i2c(0x0027));
		(*otp_ptr).g_B_Gr_ratio = (S5K3L6_R2A_read_i2c(0x002A)<<8) | (S5K3L6_R2A_read_i2c(0x0029));
		(*otp_ptr).g_Gb_Gr_ratio = (S5K3L6_R2A_read_i2c(0x002C)<<8) | (S5K3L6_R2A_read_i2c(0x002B));
		(*otp_ptr).checksumAWB=S5K3L6_R2A_read_i2c(0x002D);
	}else{
		(*otp_ptr).R_Gr_ratio = 0;
		(*otp_ptr).B_Gr_ratio = 0;
		(*otp_ptr).Gb_Gr_ratio	= 0;
		(*otp_ptr).g_R_Gr_ratio = 0;
		(*otp_ptr).g_B_Gr_ratio = 0;
		(*otp_ptr).g_Gb_Gr_ratio = 0;
		(*otp_ptr).checksumAWB=0;
	}

	for(i=0;i<12;i++){
		checksumAWB+=S5K3L6_R2A_read_i2c(0x0021+i);
	}
	checksumAWB = (checksumAWB)%256;

	LOG_INF("AWB_flag=%d,R_Gr_ratio=0x%x,B_Gr_ratio=0x%x, Gb_Gr_ratio=0x%x\n",awb_flag,(*otp_ptr).R_Gr_ratio,(*otp_ptr).B_Gr_ratio, (*otp_ptr).Gb_Gr_ratio);
	LOG_INF("g_R_Gr_ratio=0x%x,g_B_Gr_ratio=0x%x, g_Gb_Gr_ratio=0x%x\n",(*otp_ptr).g_R_Gr_ratio,(*otp_ptr).g_B_Gr_ratio, (*otp_ptr).g_Gb_Gr_ratio);
	LOG_INF("checksumAWB=%d\n",(*otp_ptr).checksumAWB);
	LOG_INF("checksumAWB=%d\n",checksumAWB);

	// OTP VCM Calibration
	vcm_flag = S5K3L6_R2A_read_i2c(0x078e);
	LOG_INF("VCM calibration flag : 0x%x \n", vcm_flag);

	if(vcm_flag == 0x01) {
		(* otp_ptr).VCM_dir = S5K3L6_R2A_read_i2c(0x078f);
		(* otp_ptr).VCM_inf = (S5K3L6_R2A_read_i2c(0x0791)<<8) | (S5K3L6_R2A_read_i2c(0x0790));
		(* otp_ptr).VCM_macro = (S5K3L6_R2A_read_i2c(0x0793)<<8) | (S5K3L6_R2A_read_i2c(0x0792));
		(* otp_ptr).checksumAF =S5K3L6_R2A_read_i2c(0x0798);
	}else {
		(* otp_ptr).VCM_macro = 0;
		(* otp_ptr).VCM_inf = 0;
		(* otp_ptr).VCM_dir = 0;
		(* otp_ptr).checksumAF = 0;
	}
	
	af_addr=0x078f;
	for(i=0;i<9;i++){
		checksumAF+=S5K3L6_R2A_read_i2c(af_addr+i);
	}

	checksumAF = (checksumAF)%256;

	LOG_INF("VCM_macro: 0x%x ,VCM_inf: 0x%x,VCM_dir: 0x%x,(* otp_ptr).checksumAF: %d\n", (* otp_ptr).VCM_macro,(* otp_ptr).VCM_inf,(* otp_ptr).VCM_dir,(* otp_ptr).checksumAF);
	LOG_INF("checksumAF:%d\n",checksumAF);

	// OTP Lenc Calibration
	lsc_flag = S5K3L6_R2A_read_i2c(0x0040);
	LOG_INF(" Lenc calibration flag : %x \n", lsc_flag);
	lsc_addr=0x0041;
	if(lsc_flag == 0x01) {
		for(i=0;i<1868;i++) {
			lenc[i]= S5K3L6_R2A_read_i2c(lsc_addr + i);
			checksumLSC += lenc[i];
		}

		checksumLSC = (checksumLSC)%256;
		(* otp_ptr).checksumLSC=S5K3L6_R2A_read_i2c(0x078d);
		LOG_INF(" checksumLSC:%d, checksumLSC :%d\n", checksumLSC, (* otp_ptr).checksumLSC);
	}else{
		for(i=0;i<1868;i++) {
			lenc[i]= 0;
			LOG_INF("NO LSC OTP  \n");
		}
	}


	printk("bo_liu, otp_flag = %d\n", otp_flag);
	printk("bo_liu, awb_flag = %d\n", awb_flag);
	printk("bo_liu, lsc_flag = %d\n", lsc_flag);
	printk("bo_liu, (*otp_ptr).module_integrator_id = 0x%x\n", (*otp_ptr).module_integrator_id);
	printk("bo_liu, (*otp_ptr).lens_id = 0x%x\n", (*otp_ptr).lens_id);
	printk("bo_liu, (*otp_ptr).vcm_id = 0x%x\n", (*otp_ptr).vcm_id);
	printk("bo_liu, (*otp_ptr).vcm_ic_id = 0x%x\n", (*otp_ptr).vcm_ic_id);

	if((otp_flag==0x01)&&(awb_flag==0x01)&&(lsc_flag==0x01)&&((*otp_ptr).module_integrator_id==0x43)&&((*otp_ptr).lens_id==0xc)&&((*otp_ptr).vcm_id==0xd4)&&((*otp_ptr).vcm_ic_id==0x13)&&(checksum==(*otp_ptr).checksumOTP)&&(checksumAWB==(*otp_ptr).checksumAWB)
		&&(checksumAF==(* otp_ptr).checksumAF)&&(checksumLSC==(* otp_ptr).checksumLSC)){
		S5K3L6_OTP_Flag=1;
	}else{
		S5K3L6_OTP_Flag=0;
	}

	return 0;
}

void S5K3L6_pdaf_cali(void)
{
	struct s5k3l6_pdaf_struct current_pdaf;
	
	memset(&current_pdaf, 0, sizeof(struct s5k3l6_pdaf_struct));
	S5K3L6_read_pdaf(&current_pdaf);
}

void S5K3L6_otp_cali(void)
{
	struct s5k3l6_otp_struct current_otp;

	memset(&current_otp, 0, sizeof(struct s5k3l6_otp_struct));
	S5K3L6_read_otp(&current_otp);
}



#ifndef _BETTERLIFE_FACTORY_H_
#define _BETTERLIFE_FACTORY_H_


#ifdef  BTL_FACTORY_SCAN_MODE

#define     DRIVER_FACTORY_TEST    0
#define     APK_FACTORY_TEST       1
#define     FACTORY_TEST_TYPE      DRIVER_FACTORY_TEST

#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
#include "bl_type_def.h"
#include "bl_chip.h"
#endif

//Max CMD number
#define BL_MAX_CMD_NUM  6


//scan TP data
struct bl_scan_cmd {
    unsigned char register0x0E;
    unsigned char register0x0F;
    unsigned char register0x20;
    unsigned char register0x9C;
};

struct bl_scan_data {
    unsigned char cb[50];
    //unsigned char raw8[100];
    u16 raw16[50];
};


//scan TP data
struct bl_factory_scan_info {
    int channel_number;
    struct bl_scan_cmd cmd[BL_MAX_CMD_NUM];
    struct bl_scan_data data[BL_MAX_CMD_NUM];
};
//scan TP data


//factoryData.txt data
struct bl_factory_txt_data {
    char raw_min[512];
    char raw_max[512];
    char cb_min[512];
    char cb_max[512];
};

struct bl_factory_txt_int_data {
    unsigned char cb_min[50];
    unsigned char cb_max[50];
    u16 raw_min[50];
    u16 raw_max[50];
};

struct bl_factory_compare_data {
    unsigned char test_result;  // 测试结果  1 NG ， 0 OK
    unsigned char has_test;    //  是否已经测试  1  已测试， 0 未测试。
    unsigned char cb[50];
    unsigned char raw[50];
};

struct bl_factory_txt_info {
    int channel_number;
    int frequency_default;
    int frequencymode2;//mode 2 frequency
    int frequencymode4;//mode 4 frequency
    int raw_check_value;
    int cb_check_value;
    struct bl_factory_txt_data        line_data[BL_MAX_CMD_NUM];
    struct bl_factory_txt_int_data    data[BL_MAX_CMD_NUM];
    struct bl_factory_compare_data    result[BL_MAX_CMD_NUM];
};
//factoryData.txt data

int bl_factory_init(void);               // 增加声明 方便apk调用
int bl_factory_init_struct(void);        // 增加声明 方便apk调用
int bl_factory_uinit(void);              // 增加声明 方便apk调用


#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
// apk  调用接口
int bl_apk_get_factory_test_reslut(unsigned char* pResult, unsigned char len);

int bl_apk_get_factory_has_test_flg(unsigned char* pFlag, unsigned char len);

int bl_apk_get_factory_compare_reslut(int mode, int type, unsigned char* pCmpResult,
                                      unsigned char len);

int bl_apk_get_factory_scan_raw_data(int mode,  unsigned short*  pDataBuf, unsigned char len);

int bl_apk_get_factory_scan_cb_data(int mode,  unsigned char*  pDataBuf, unsigned char len);
#endif

#endif

#endif




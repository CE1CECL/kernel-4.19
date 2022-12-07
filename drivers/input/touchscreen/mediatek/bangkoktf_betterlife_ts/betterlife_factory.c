

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/string.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include "tpd.h"
#include "betterlife_factory.h"


#ifdef  BTL_FACTORY_SCAN_MODE


#if(FACTORY_TEST_TYPE  ==  DRIVER_FACTORY_TEST)
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include "tpd_bl_common.h"


#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
extern struct delayed_work ac_usb_dwork;
#endif

#ifdef BTL_ESD_PROTECT
extern void bl_esd_switch(s32 on);
#endif

#define bl_read_reg btl_read_reg
#define bl_write_reg    btl_write_reg


int bl_strcmp(unsigned char* str1, unsigned char* str2, unsigned char len)
{
    unsigned char i;

    for(i = 0; i < len; i++) {
        if(str1[i] != str2[i]) {
            return -1;
        }

    }
    return 0;
}



#else         /////////////////////////////////  增加Apk的接口 兼容 APK测试

#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "bl_chip.h"
#include <errno.h>

#define  printk  LOGE
#define  KERNEL_DS 0
#define  mdelay(x)  MSLEEP(x)
#define  bl_ts_set_wakeup(x)  bl_ts_set_int_gpio(x)
#define  bl_ts_enable_irq(x)  bl_enable_irq(x)
#define  bl_ts_i2c_lock(x)     bl_i2c_lock(x)
#define  simple_strtoul(x, y, z)  strtoul(x, y ,z)
#define  get_fs()         1
#define  set_fs(x)        x
#define  IS_ERR(x)       (x == NULL || x->fd == -1) ? 1 : 0
#define  kmalloc(x,y)      malloc(x)
#define  kfree(x)         free(x)
#define  bl_strcmp(x, y, z)  memcmp (x, y, z)


struct i2c_client {
    u8  addr;
};
struct ts_driver {
    struct i2c_client* client;
};
struct  ts_info {
    struct ts_driver driver;
};

typedef  int   mm_segment_t;

struct file_operations {
    int (*lseek)(struct inode*, struct file*, off_t, int);
    int (*read)(struct file*, char*, int, loff_t lt);
    int (*write)(struct inode*, struct file*, const char*, int);
    int (*open)(struct inode*, struct file*);
};

struct file {
    int fd;
    mode_t f_mode;
    loff_t f_pos;
    unsigned short f_flags;
    unsigned short f_count;
    struct file_operations* f_op;
};

int apk_file_read(struct file* pfile, char* buf, int len, loff_t lt)
{
    return  read(pfile->fd, buf, len);
}

int vfs_write(struct file* pfile, const char* buffer, int len, loff_t* pos)
{
    return  write(pfile->fd, buffer, len);
}

struct file*  filp_open(const char* filename, int flags, int mode)
{
    struct file* pfile = malloc(sizeof(struct file));
    pfile->f_op = malloc(sizeof(struct file_operations));

    printk(" before open name %s, flag %d, mode %d .\n", filename, flags, mode);
    pfile->fd = open(filename, flags /*, mode*/);
    printk(" after open fd:%d \n", pfile->fd);
    pfile->f_op->read = apk_file_read;

    if(pfile->fd == -1) {
        printk("filp_open fail errno :%d\n", errno);
        return NULL;
    }
    printk("  open success \n");

    return pfile;
}

void  filp_close(struct file* pfile, void*  puser)
{
    if(pfile == NULL)
        return;
    if(pfile->f_op == NULL)
        return;
    close(pfile->fd);
    kfree(pfile->f_op);
    kfree(pfile);
}

int bl_i2c_rxdata(struct i2c_client* client, char* rxdata, int length)
{
    int ret = 0;
    if(bl_fw_read(client->addr, rxdata[0],  rxdata, length) < 0) {
        ret = -1;
        printk("btl msg %s i2c read error: %d\n", __func__, ret);
    }
    return ret;
}

int bl_i2c_txdata(struct i2c_client* client, char* txdata, int length)
{
    int ret = 0;
    if(bl_fw_write(client->addr, txdata, length) < 0) {
        ret = -1;
        printk("btl %s i2c write error: %d\n", __func__, ret);
    }
    return ret;
}

int bl_write_reg(struct i2c_client* client, u8 addr, u8 para)
{
    u8 buf[3];
    int ret = -1;
    buf[0] = addr;
    buf[1] = para;
    ret = bl_i2c_txdata(client, buf, 2);
    if(ret < 0) {
        printk("btl write reg failed! %#x ret: %d", buf[0], ret);
        return -1;
    }
    return 0;
}


int bl_read_reg(struct i2c_client* client, u8 reg_addr, u8* pdata)
{
    int ret = 0;
    u8 buf[2] = {0};
    if(bl_fw_read(client->addr, reg_addr, &buf[1], 1) < 0) {
        ret = - 1;
        printk("btl msg %s i2c read error: %d\n", __func__, ret);
    }
    *pdata = buf[1];
    return ret;
}

struct ts_info  g_btl_ts;
struct ts_info* bl_ts = &g_btl_ts;
int  init_apk_struct();
void  destory_apk_struct();

#endif // end apk api




//scan TP data
struct bl_factory_scan_info* p_bl_factory_scan_info = NULL;

//factoryData.txt data
struct bl_factory_txt_info* p_bl_factory_txt_info = NULL;

int bl_factory_compare_chip_txt(int mode);

int bl_enter_factory_mode(void)
{
    int retry;
    int ret = 1;
    unsigned char factory_value = 0x00;

    for(retry = 0; retry < 3; retry++) {
        ret = bl_read_reg(btl_i2c_client, 0x00, &factory_value);
        if(!ret) break;

        mdelay(50);
    }

    if(retry == 3)  return 1;

    if((factory_value & 0x7F) != 0x40) {

        ret = bl_write_reg(btl_i2c_client, 0x00, 0xC0);//0xC0 0x40
        if(ret) {
            printk("btl enter factory mode 0x00 write 0xC0 error \n");
        } else {
            printk("btl enter factory mode 0x00 write 0xC0 ok\n");
        }

        for(retry = 0; retry < 10; retry++) {
            mdelay(50);
            ret = bl_read_reg(btl_i2c_client, 0x00, &factory_value);
            if(ret) {
                printk("btl enter factory mode read 0x00 error retry:%d\n", retry);
                continue;
            } else {
                printk("btl enter factory mode read 0x00 value:0x%x ok retry:%d\n", factory_value , retry);
            }

            if(!(factory_value & 0x80)) {
                printk("btl enter factory mode 0x80 read ==0 %d\n", retry);
                break;
            } else {
                printk("btl enter factory mode 0x80 read !=0 %d\n", retry);
            }
        }

        if(retry == 10) return 1;
    }

    return 0;
}

int bl_exit_factory_mode(void)
{
    tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
    mdelay(50);
    tpd_gpio_output(btl_tpd_rst_gpio_number, 0);
    mdelay(50);
    tpd_gpio_output(btl_tpd_rst_gpio_number, 1);

    return 0;
}

int bl_set_scan_frequency(struct bl_factory_scan_info* bl_scan, struct bl_factory_txt_info* bl_txt)
{
    if(bl_txt == NULL) {
        printk("btl NULL bl_txt data\n");
        return 1;
    }

    if(bl_txt->frequency_default > 0xFF || bl_txt->frequency_default < 0)
        bl_txt->frequency_default = 0;

    if(bl_txt->frequencymode2 > 0xFF || bl_txt->frequencymode2 < 0)
        bl_txt->frequencymode2 = 0;

    if(bl_txt->frequencymode4 > 0xFF || bl_txt->frequencymode4 < 0)
        bl_txt->frequencymode4 = 0;

    bl_scan->cmd[0].register0x0F = bl_txt->frequency_default;
    bl_scan->cmd[1].register0x0F = bl_txt->frequencymode2;
    bl_scan->cmd[2].register0x0F = bl_txt->frequency_default;
    bl_scan->cmd[3].register0x0F = bl_txt->frequencymode4;
    bl_scan->cmd[4].register0x0F = bl_txt->frequency_default;
    bl_scan->cmd[5].register0x0F = bl_txt->frequency_default;

    return 0;
}

int bl_set_scan_cmd(struct bl_factory_scan_info* bl_scan, struct bl_factory_txt_info* bl_txt)
{
    //0x0E  通道防水设置；1-全屏防水; 0-不防水
    //0x20
    //0x0F  充电时间;间接体现为扫描频率
    //0x9C  1-CB值校准 0-校准完成
    int ret = 1;

    bl_scan->cmd[0].register0x0E = 0x01;
    bl_scan->cmd[0].register0x20 = 0x11;
    bl_scan->cmd[0].register0x9C = 0x02;

    bl_scan->cmd[1].register0x0E = 0x01;
    bl_scan->cmd[1].register0x20 = 0x11;
    bl_scan->cmd[1].register0x9C = 0x02;

    bl_scan->cmd[2].register0x0E = 0x01;
    bl_scan->cmd[2].register0x20 = 0x10;
    bl_scan->cmd[2].register0x9C = 0x02;

    bl_scan->cmd[3].register0x0E = 0x01;
    bl_scan->cmd[3].register0x20 = 0x10;
    bl_scan->cmd[3].register0x9C = 0x02;

    bl_scan->cmd[4].register0x0E = 0x01;
    bl_scan->cmd[4].register0x20 = 0x02;
    bl_scan->cmd[4].register0x9C = 0x02;

    bl_scan->cmd[5].register0x0E = 0x00;
    bl_scan->cmd[5].register0x20 = 0x02;
    bl_scan->cmd[5].register0x9C = 0x02;

    ret = bl_set_scan_frequency(bl_scan, bl_txt);

    return ret;
}

unsigned char bl_get_channel_number(struct bl_factory_scan_info* bl_scan)
{
    unsigned char channel_Va = 0x00;
    unsigned char channel_Key = 0x00;
    int retry;
    int ret = 0;

    for(retry = 0; retry < 3; retry++) {
        ret = bl_read_reg(btl_i2c_client, 0x14, &channel_Va);
        if(ret) {
            mdelay(50);
            continue;
        }

        ret = bl_read_reg(btl_i2c_client, 0x15, &channel_Key);
        if(!ret) {
            break;
        }
    }

    if(retry == 3) {
        bl_scan->channel_number = 0;
        return 1;
    }

    bl_scan->channel_number = channel_Va + channel_Key;

    return 0;
}

int bl_get_scan_cmd_data(struct bl_factory_scan_info* bl_scan, int mode)
{
    int ret = 1;
    int i = 0;
    int j = 0;
    int retry = 0;
    int retry_0x9C = 0;
    unsigned char value_0x9C = 0xFF;
    unsigned char scan_flag = 0x00;
    u16* pvalue16 = NULL;
    u8 addr = 0xff;
    unsigned char raw8[100];

    //enter factory mode
    ret = bl_enter_factory_mode();
    if(ret) {
        printk("btl enter factory mode error\n");
        return 1;
    } else {
        printk("btl enter factory mode ok\n");
    }

    for(retry = 0; retry < 5; retry++) {
        ret = bl_write_reg(btl_i2c_client, 0x0E, bl_scan->cmd[mode - 1].register0x0E);
        if(ret) {
            printk("btl write 0x0e error %d\n", retry);
            mdelay(50);
            continue;
        } else {
            printk("btl write 0x0e ok %d\n", retry);
        }

        ret = bl_write_reg(btl_i2c_client, 0x20, bl_scan->cmd[mode - 1].register0x20);
        if(ret) {
            printk("btl write 0x20 error %d\n", retry);
            mdelay(50);
            continue;
        } else {
            printk("btl write 0x20 ok %d\n", retry);
        }

        ret = bl_write_reg(btl_i2c_client, 0x0F, bl_scan->cmd[mode - 1].register0x0F);
        if(ret) {
            printk("btl write 0x0F error %d\n", retry);
            mdelay(50);
            continue;
        } else {
            printk("btl write 0x0F ok %d\n", retry);
        }

        //校准
        ret = bl_write_reg(btl_i2c_client, 0x9C, bl_scan->cmd[mode - 1].register0x9C);
        if(ret) {
            printk("btl write 0x9c error %d\n", retry);
            mdelay(50);
            continue;
        } else {
            printk("btl write 0x9c ok %d\n", retry);

            for(retry_0x9C = 0; retry_0x9C < 10; retry_0x9C++) {
                mdelay(50);
                ret = bl_read_reg(btl_i2c_client, 0x9C, &value_0x9C);
                if(ret || value_0x9C) {
                    printk("btl read 0x9c error %d\n", retry_0x9C);
                    continue;
                } else {
                    printk("btl read 0x9c ok %d\n", retry_0x9C);
                    break;
                }
            }
        }

        if(retry_0x9C == 10) {
            printk("btl 0x9C check error %d\n", retry_0x9C);
            continue;
        } else {
            printk("btl 0x9C check ok %d \n", retry_0x9C);
            break;
        }

    }

    if(retry == 5) {
        printk("btl write cmd error\n");
        return 1;
    } else {
        printk("btl write cmd ok\n");
    }

    ret = bl_write_reg(btl_i2c_client, 0x00, 0xC0);//启动扫描
    if(ret) {
        printk("btl 0x00 write 0xC0 error\n");
    } else {

        printk("btl 0x00 write 0xC0 ok\n");

        for(i = 0; i < 10; i++) {
            mdelay(50);
            ret = bl_read_reg(btl_i2c_client, 0x00, &scan_flag);
            if(ret) {
                printk("btl 0x00 read scan_flag error %d \n", i);
                continue;
            } else {
                if(!(scan_flag & 0x80)) {
                    printk("btl 0x00 read scan_flag & 0x80 ok %d , scan_flag: %d\n", i, scan_flag);
                    break;
                } else {
                    printk("btl 0x00 read scan_flag & 0x80 error  %d\n", i);
                }
            }
        }

        if(i == 10) {
            printk("btl check 0x00 0xC0 error and exit\n");
            return 1;
        }
    }


    //raw
    for(retry = 0; retry < 5; retry++) {

        addr = 0x7b;
        //ret = bl_i2c_rxdata(bl_ts->driver.client, bl_scan->data[mode - 1].raw8, 2*(bl_scan->channel_number));
        ret = btl_i2c_read(btl_i2c_client, &addr, 1, raw8, 2 * (bl_scan->channel_number));
        if(!ret) {
            printk("btl raw 0x7b read ok %d\n", retry);
            for(i = 0 ; i < bl_scan->channel_number; i++) {
                pvalue16 = (u16*)&raw8[j];
                bl_scan->data[mode - 1].raw16[i] = *pvalue16;
                j = j + 2;

                //(scan_data[i] << 8) + scan_data[i+1];
                //bl_scan->data[mode - 1].raw[i] = (scan_data[i] << 8) + scan_data[i+1];
            }

            break;
        } else {
            printk("btl raw 0x7b read error %d\n", retry);
            continue;
        }
    }

    if(retry == 5)
        return 1;


    //cb
    for(retry = 0; retry < 5; retry++) {
        addr = 0x11;
        ret = btl_i2c_read(btl_i2c_client, &addr, 1, bl_scan->data[mode - 1].cb, bl_scan->channel_number);
        if(!ret) {
            printk("btl cb 0x11 read ok %d\n", retry);
            break;
        } else {
            printk("btl cb 0x11 read error %d\n", retry);
        }
    }

    if(retry == 5)  return 1;

    return 0;
}

int bl_get_scan_complex_data(struct bl_factory_scan_info* bl_scan)
{
    int i = 0;
    int ret = 0;

    //rst msleep(300)
    bl_exit_factory_mode();
    mdelay(500);

    for(i = 1; i <= 6; i++) {
        ret = bl_get_scan_cmd_data(bl_scan, i);
        if(ret) {
            printk("btl scan  %d cmd data error\n\n\n", i);
            return 1;
        } else {
            printk("btl scan %d cmd data ok\n\n\n", i);
            ret = bl_factory_compare_chip_txt(i - 1);
            if(ret) {
                printk("btl mode %d compare data error\n\n\n", i);
                ret = 1;
                //return 1;
            } else {
                printk("btl mode %d compare data OK\n\n\n", i);
            }
        }
    }

    return ret;
}

int bl_printk_scan_data(struct bl_factory_scan_info* bl_scan)
{
    int i, j;

    printk("channel number:%d\n", bl_scan->channel_number);

    for(i = 0; i < 6; i++) {
        printk("mode %d\n", i + 1);

        printk("raw16\n");
        for(j = 0; j < bl_scan->channel_number; j++) {
            printk("0x%x ", bl_scan->data[i].raw16[j]);
        }
        printk("\n");

        printk("cb\n");
        for(j = 0; j < bl_scan->channel_number; j++) {
            printk("%d ", bl_scan->data[i].cb[j]);
        }
        printk("\n");

        printk("\n");
    }

    return 0;
}


//factoryData.txt read
#if (FACTORY_TEST_TYPE == DRIVER_FACTORY_TEST)
static char* read_line(char* buf, int buf_len, struct file* fp)
{
    int ret;
    int i = 0;
    mm_segment_t fs;

    fs = get_fs();
    set_fs(KERNEL_DS);
    ret = fp->f_op->read(fp, buf, buf_len, &(fp->f_pos));
    set_fs(fs);

    if(ret <= 0)
        return NULL;

    while(buf[i++] != '\n' && i < ret);

    if(i < ret) {
        fp->f_pos += i - ret;
    }

    if(i < buf_len) {
        buf[i] = 0;
    }

    return buf;
}
#else
static char* read_line(char* buf, int buf_len, struct file* fp)
{
    int rl = -1;
    char c;

    long count = 0;
    if(fp->fd < 0) {
        printk("open file error\n");
        close(fp->fd);
        return  NULL;
    }
    if(buf == NULL) {
        printk("allocate memory error\n");
        close(fp->fd);
        return  NULL;
    }

    while((rl = read(fp->fd, &c, 1)) > 0) {
        if(count == buf_len) {
            break;
        }
        buf[count++] = c;
        if(c == '\n' || c == EOF) {
            break;
        }
    }
    if(count == 0) {
        return NULL;
    }
    buf[count] = '\0';
    return buf;
}
#endif

static int bl_factory_chartoint(struct bl_factory_txt_info* bl_txt)
{
    char* raw_min_next = NULL;
    char* raw_max_next = NULL;
    char* cb_min_next = NULL;
    char* cb_max_next = NULL;

    int i = 0;
    int j = 0;

    for(i = 0; i < BL_MAX_CMD_NUM; i++) {
        bl_txt->data[i].raw_min[0] = simple_strtoul(bl_txt->line_data[i].raw_min, &raw_min_next, 10);
        bl_txt->data[i].raw_max[0] = simple_strtoul(bl_txt->line_data[i].raw_max, &raw_max_next, 10);
        bl_txt->data[i].cb_min[0] = simple_strtoul(bl_txt->line_data[i].cb_min, &cb_min_next, 10);
        bl_txt->data[i].cb_max[0] = simple_strtoul(bl_txt->line_data[i].cb_max, &cb_max_next, 10);

        for(j = 1; j < bl_txt->channel_number; j++) {
            bl_txt->data[i].raw_min[j] = simple_strtoul(raw_min_next + 1, &raw_min_next, 10);
            bl_txt->data[i].raw_max[j] = simple_strtoul(raw_max_next + 1, &raw_max_next, 10);
            bl_txt->data[i].cb_min[j] = simple_strtoul(cb_min_next + 1, &cb_min_next, 10);
            bl_txt->data[i].cb_max[j] = simple_strtoul(cb_max_next + 1, &cb_max_next, 10);
        }
    }

    return 0;
}


//init bl_factory_txt_info
//factoryData.txt
static int bl_factory_read_file(struct bl_factory_txt_info* bl_txt)
{
    int i = 0;

    struct file* pfile = NULL;
    char buf[512] = {0};

    char max_channel_num[30] = "Max Channel Num,";
    char frequency_default[30] = "frequency default,";
    char frequency_mode2[30] = "frequency mode2,";
    char frequency_mode4[30] = "frequency mode4,";
    char raw_check_value[30] = "raw check value,";
    char cb_check_value[30] = "cb check value,";
    char mode[] = "mode,";
    //char type_raw[10] = "raw";
    //char type_cb[10] = "cb";

    printk(" open  bl_factory_read_file .\n");
    if(NULL == pfile)
        pfile = filp_open("/sdcard/factoryData.txt", O_RDWR, 0);

    printk(" after open  bl_factory_read_file .\n");

    if(IS_ERR(pfile)) {

        printk(" btl error occured while opening file .\n");
        return -1;
    }

    printk(" in bl_factory_read_file .\n");

    while(read_line(buf, 512, pfile)) {

        if(!bl_strcmp(buf, max_channel_num, strlen(max_channel_num))) {
            sscanf(buf, "Max Channel Num,%d,", &bl_txt->channel_number);
        } else if(!bl_strcmp(buf, frequency_default, strlen(frequency_default))) {
            sscanf(buf, "frequency default,%d,", &bl_txt->frequency_default);
        } else if(!bl_strcmp(buf, frequency_mode2, strlen(frequency_mode2))) {
            sscanf(buf, "frequency mode2,%d,", &bl_txt->frequencymode2);
        } else if(!bl_strcmp(buf, frequency_mode4, strlen(frequency_mode4))) {
            sscanf(buf, "frequency mode4,%d,", &bl_txt->frequencymode4);
        } else if(!bl_strcmp(buf, raw_check_value, strlen(raw_check_value))) {
            sscanf(buf, "raw check value,%d,", &bl_txt->raw_check_value);
        } else if(!bl_strcmp(buf, cb_check_value, strlen(cb_check_value))) {
            sscanf(buf, "cb check value,%d,", &bl_txt->cb_check_value);
        }

        //mode,x,data
        else if(!bl_strcmp(buf, mode, strlen(mode))) {
            read_line(bl_txt->line_data[i].raw_min, 512, pfile);
            read_line(bl_txt->line_data[i].raw_max, 512, pfile);
            read_line(buf, 512, pfile);
            //printk("btl buf :%s\n",buf);
            read_line(buf, 512, pfile);
            //printk("btl buf :%s\n",buf);
            read_line(bl_txt->line_data[i].cb_min, 512, pfile);
            //printk("btl cb_min:%s\n",bl_txt->line_data[i].cb_min);
            read_line(bl_txt->line_data[i].cb_max, 512, pfile);
            //printk("btl cb_max:%s\n",bl_txt->line_data[i].cb_max);
            i++;
        }
    }

    filp_close(pfile, NULL);

    return 0;
}

//scanData.txt
#if 1
int bl_scan_write_file(struct bl_factory_scan_info* bl_scan)
{
    int i = 0;
    int j = 0;
    struct file* scanf_file = NULL;
    loff_t pos;
    mm_segment_t old_fs;
    unsigned char buf[512] = {0};
    unsigned char inode[20] = {0};
    unsigned char data[512] = {0};

    //O_TRUNC   起始位置写入/读取
    //O_APPEND  末尾追加  pos
    scanf_file = filp_open("/sdcard/scanData.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(IS_ERR(scanf_file)) {
        printk("btl /sdcard/scanData.txt open error\n");
        return -1;
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;

    memset(buf, 0 , 512);
    sprintf(buf, "Max Channel Num,%d,\n\n", bl_scan->channel_number);
    vfs_write(scanf_file, (const char*)buf, strlen(buf), &pos);

    memset(buf, 0 , 512);
    sprintf(buf, "frequency default,%d,\n\n", bl_scan->cmd[0].register0x0F);
    vfs_write(scanf_file, (const char*)buf, strlen(buf), &pos);

    memset(buf, 0 , 512);
    sprintf(buf, "frequency mode2,%d,\n\n", bl_scan->cmd[1].register0x0F);
    vfs_write(scanf_file, (const char*)buf, strlen(buf), &pos);

    memset(buf, 0 , 512);
    sprintf(buf, "frequency mode4,%d,\n\n", bl_scan->cmd[3].register0x0F);
    vfs_write(scanf_file, (const char*)buf, strlen(buf), &pos);

    for(i = 0; i < 6; i++) {
        memset(buf, 0 , 512);
        sprintf(buf, "mode,%d,raw\n", i + 1);
        vfs_write(scanf_file, (const char*)buf, strlen(buf), &pos);

        memset(data, 0, 512);
        for(j = 0; j < bl_scan->channel_number; j++) {
            memset(inode, 0, 20);
            sprintf(inode, "%d,", bl_scan->data[i].raw16[j]);
            strcat(data, inode);
        }
        sprintf(buf, "%s\n", data);
        vfs_write(scanf_file, (const char*)(buf), strlen(buf), &pos);


        memset(buf, 0 , 512);
        sprintf(buf, "mode,%d,cb\n", i + 1);
        vfs_write(scanf_file, (const char*)buf, strlen(buf), &pos);

        memset(data, 0, 512);
        for(j = 0; j < bl_scan->channel_number; j++) {
            memset(inode, 0, 20);
            sprintf(inode, "%d,", bl_scan->data[i].cb[j]);
            strcat(data, inode);
        }
        sprintf(buf, "%s\n\n", data);
        vfs_write(scanf_file, (const char*)(buf), strlen(buf), &pos);
    }

    filp_close(scanf_file, NULL);
    set_fs(old_fs);

    return 0;
}
#endif


//chip & txt compare
//bl_txt factoryData.txt
//bl_factory scanfData.txt
int bl_factory_compare_chip_txt(int mode)
{
    int inode = 0;
    unsigned char cb_check_value_min = 0x00;
    unsigned char cb_check_value_max = 0xFF;

    struct bl_factory_scan_info* bl_scan = p_bl_factory_scan_info;
    struct bl_factory_txt_info* bl_txt = p_bl_factory_txt_info;

    bl_txt->result[mode].has_test = 1;
    for(inode = 0; inode < bl_scan->channel_number; inode++) {
        if(bl_scan->data[mode].raw16[inode] > (bl_txt->data[mode].raw_max[inode] + bl_txt->raw_check_value) ||
           bl_scan->data[mode].raw16[inode] < (bl_txt->data[mode].raw_min[inode] - bl_txt->raw_check_value)) {
            printk("btl mode:%d inode:%d raw:%d min:%d max:%d raw_check_value:%d\n",
                   mode + 1,
                   inode + 1,
                   bl_scan->data[mode].raw16[inode], bl_txt->data[mode].raw_min[inode],
                   bl_txt->data[mode].raw_max[inode], bl_txt->raw_check_value);

            bl_txt->result[mode].test_result = 1;
            bl_txt->result[mode].raw[inode] = 1;
        }

        if((0xFF - bl_txt->cb_check_value) < bl_txt->data[mode].cb_max[inode]) {
            cb_check_value_max = 0xFF;
        } else {
            cb_check_value_max = bl_txt->data[mode].cb_max[inode] + bl_txt->cb_check_value;
        }

        if(bl_txt->data[mode].cb_min[inode] < bl_txt->cb_check_value) {
            cb_check_value_min = bl_txt->data[mode].cb_min[inode];
        } else {
            cb_check_value_min = bl_txt->data[mode].cb_min[inode] - bl_txt->cb_check_value;
        }

        if(bl_scan->data[mode].cb[inode] > cb_check_value_max //bl_txt->data[mode].cb_max[inode]
           || bl_scan->data[mode].cb[inode] < cb_check_value_min) { //bl_txt->data[mode].cb_min[inode]
            printk("btl mode:%d inode:%d cb:%d min:%d max:%d cb_check_value:%d\n", mode + 1,
                   inode + 1,
                   bl_scan->data[mode].cb[inode], bl_txt->data[mode].cb_min[inode],
                   bl_txt->data[mode].cb_max[inode], bl_txt->cb_check_value);

            bl_txt->result[mode].test_result = 1;
            bl_txt->result[mode].cb[inode] = 1;

        }
    }

    if(bl_txt->result[mode].test_result)
        return 1;


    return 0;
}

int bl_factory_scan_init(void)
{
    int ret = 0;

    //init channel number
#if (FACTORY_TEST_TYPE == DRIVER_FACTORY_TEST)
    bl_i2c_lock(1);
#endif
    ret = bl_get_channel_number(p_bl_factory_scan_info);
#if (FACTORY_TEST_TYPE == DRIVER_FACTORY_TEST)
    bl_i2c_lock(0);
#endif
    if(ret) {
        printk("btl bl_factory_scan_init channel number error\n");
        //kfree(p_bl_factory_scan_info);
        //p_bl_factory_scan_info = NULL;
        return 1;
    }

    //init factory cmd
    ret = bl_set_scan_cmd(p_bl_factory_scan_info, p_bl_factory_txt_info);
    if(ret) {
        printk("btl bl_factory_scan_init frequency init error\n");
        //kfree(p_bl_factory_scan_info);
        //p_bl_factory_scan_info = NULL;
        return 1;
    }

#if (FACTORY_TEST_TYPE == DRIVER_FACTORY_TEST)

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
    cancel_delayed_work_sync(&ac_usb_dwork);
#endif

#ifdef BTL_ESD_PROTECT
    bl_esd_switch(SWITCH_OFF);
#endif

    //init data cmd 1 - 6
    disable_irq(bl_touch_irq);
    bl_i2c_lock(1);
#endif

    ret = bl_get_scan_complex_data(p_bl_factory_scan_info);
    if(ret) {
        printk("btl bl_factory_scan_init scan complex data error\n");
        //kfree(p_bl_factory_scan_info);
        //p_bl_factory_scan_info = NULL;
        //return 1;
    }

#if (FACTORY_TEST_TYPE == DRIVER_FACTORY_TEST)

    bl_i2c_lock(0);
    enable_irq(bl_touch_irq);

#ifdef BTL_ESD_PROTECT
    bl_esd_switch(SWITCH_ON);
#endif

#ifdef  BTL_AC_USB_POWER_DETECT_ENABLE
    schedule_delayed_work(&ac_usb_dwork, msecs_to_jiffies(POLLING_TIME));
#endif

#endif

    //create scanData.txt
    bl_scan_write_file(p_bl_factory_scan_info);


    if(ret) {
        printk("btl bl_complex test result NG \n");
    } else {
        printk("btl bl_complex test result OK \n");
    }

    //printk data
    //bl_printk_scan_data(p_bl_factory_scan_info);

    return ret;
}

int bl_factory_scan_uinit(void)
{
    if(p_bl_factory_scan_info != NULL) {
        kfree(p_bl_factory_scan_info);
        p_bl_factory_scan_info = NULL;
    }

    return 0;
}

int bl_factory_txt_init(void)
{

    //read factoryData.txt
    if(bl_factory_read_file(p_bl_factory_txt_info) < 0) {
        printk("bl_factory_read_file error\n");
        return -1;
    }

    if(bl_factory_chartoint(p_bl_factory_txt_info) < 0) {
        printk("bl_factory_chartoint error\n");
        return  -1;
    }

    return 0;
}

int bl_factory_txt_uinit(void)
{
    if(p_bl_factory_txt_info != NULL) {
        kfree(p_bl_factory_txt_info);
        p_bl_factory_txt_info = NULL;
    }

    return 0;
}

int bl_factory_init(void)
{
    int ret = 1;

#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
    bl_ts_i2c_lock(1);
    bl_ts_enable_irq(0);
#endif

    ret = bl_factory_init_struct();

    if(ret) {
        goto exit;
    }

    ret = bl_factory_txt_init();

    if(ret) {
        printk("btl bl_factory_txt_init error\n");
        goto exit;
    }

    ret = bl_factory_scan_init();
    if(ret) {
        printk("btl bl_factory_scan_init error\n");
        goto exit;
    }

#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
    bl_ts_i2c_lock(0);
    bl_ts_enable_irq(1);
#endif

    bl_exit_factory_mode();

    return ret;

exit:
#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
    bl_ts_i2c_lock(0);
    bl_ts_enable_irq(1);
#endif

    bl_exit_factory_mode();
    return 1;
}

int bl_factory_uinit(void)
{
    bl_factory_txt_uinit();
    bl_factory_scan_uinit();
    bl_exit_factory_mode();

#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
    destory_apk_struct();
#endif

    return 0;
}

int bl_factory_init_struct(void)
{

#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
    if(init_apk_struct()) {
        printk("btl init apk struct error\n");
        return  1;
    }
#endif

    if(p_bl_factory_scan_info == NULL) {
        p_bl_factory_scan_info = (struct bl_factory_scan_info*)kmalloc((sizeof(struct bl_factory_scan_info)), GFP_KERNEL);
        if(p_bl_factory_scan_info == NULL) {
            printk("btl bl_factory_scan_init kmalloc error\n");
            return 1;
        }
    }

    if(p_bl_factory_txt_info == NULL) {
        p_bl_factory_txt_info = (struct bl_factory_txt_info*)kmalloc((sizeof(struct bl_factory_txt_info)), GFP_KERNEL);
        if(p_bl_factory_txt_info == NULL) {
            printk("btl bl_factory_txt_init kmalloc error\n");
            return 1;
        }
    }
    memset(p_bl_factory_scan_info, 0, sizeof(struct bl_factory_scan_info));
    memset(p_bl_factory_txt_info, 0, sizeof(struct bl_factory_txt_info));
    return 0;
}


#if (FACTORY_TEST_TYPE == APK_FACTORY_TEST)
int bl_apk_get_factory_test_reslut(unsigned char* pResult, unsigned char len)
{
    int i;
    if(len != BL_MAX_CMD_NUM || p_bl_factory_txt_info == NULL ||  pResult == NULL)
        return  1;

    for(i = 0; i < BL_MAX_CMD_NUM; i++) {
        pResult[i] = p_bl_factory_txt_info->result[i].test_result;
    }

    return 0;
}

int bl_apk_get_factory_has_test_flg(unsigned char* pFlag, unsigned char len)
{
    int i;
    if(len != BL_MAX_CMD_NUM || p_bl_factory_txt_info == NULL ||  pFlag == NULL)
        return  1;
    for(i = 0; i < BL_MAX_CMD_NUM; i++) {
        pFlag[i] = p_bl_factory_txt_info->result[i].has_test;
    }

    return 0;
}

int bl_apk_get_factory_compare_reslut(int mode, int type, unsigned char* pCmpResult, unsigned char len)
{
    if(pCmpResult == NULL ||  p_bl_factory_txt_info == NULL || len > sizeof(p_bl_factory_txt_info->result[0].raw)) {
        return  1;
    }
    if(type == 0) {
        memcpy(pCmpResult, p_bl_factory_txt_info->result[mode].raw, len);
    } else if(type == 1) {
        memcpy(pCmpResult, p_bl_factory_txt_info->result[mode].cb, len);
    } else {
        return  1;
    }
    return  0;
}

int bl_apk_get_factory_scan_raw_data(int mode,  unsigned short*  pDataBuf, unsigned char len)
{
    int dataSize = 0;
    int i = 0;
    if(pDataBuf == NULL ||  p_bl_factory_scan_info == NULL) {
        printk("btl bl_apk_get_factory_scan_raw_data  Error 1\n");
        return  1;
    }
    dataSize = sizeof(p_bl_factory_scan_info->data[0].raw16) / sizeof(u16);
    if(len > dataSize) {
        printk("btl bl_apk_get_factory_scan_raw_data  Error 2\n");
        return  1;
    }

    for(i = 0; i < len; i++) {
        pDataBuf[i] = p_bl_factory_scan_info->data[mode].raw16[i];
    }

    printk("btl bl_apk_get_factory_scan_raw_data success mode :%d, len:%d\n" , mode, len);

    return  0;
}

int bl_apk_get_factory_scan_cb_data(int mode,  unsigned char*  pDataBuf, unsigned char len)
{
    if(pDataBuf == NULL ||  p_bl_factory_scan_info == NULL || len > sizeof(p_bl_factory_scan_info->data[0].cb)) {
        printk("btl bl_apk_get_factory_scan_cb_data error\n");
        return  1;
    }

    memcpy(pDataBuf, p_bl_factory_scan_info->data[mode].cb, len);
    printk("btl bl_apk_get_factory_scan_cb_data success mode :%d, Len:%d\n", mode,  len);

    return  0;
}

int  init_apk_struct()
{
    if(bl_ts == NULL)
        return 1;

    if(bl_ts->driver.client == NULL) {
        bl_ts->driver.client = malloc(sizeof(struct i2c_client));
        if(bl_ts->driver.client == NULL) {
            return 1;
        }
    }

    bl_ts->driver.client->addr =  bl_chip_info.fw_i2c_addr;
    return  0;

}

void  destory_apk_struct()
{

    if(bl_ts == NULL)
        return;

    if(bl_ts->driver.client == NULL)
        return;

    kfree(bl_ts->driver.client);
    bl_ts->driver.client = NULL;
}
#endif



#endif //end BTL_FACTORY_SCAN_MODE

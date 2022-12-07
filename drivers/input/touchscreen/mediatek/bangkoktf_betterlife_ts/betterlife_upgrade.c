
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#include "tpd.h"
#include "tpd_bl_common.h"
#include "tpd_custom_upgrade.h"
#include "bl_fw.h"

//#include <mt_gpio.h>
//#include <mt_gpio_core.h>

#ifdef  BTL_APK_SUPPORT
//cmd|i2c_addr|data_len|ndata
#define FRAME_CMD_LEN       0x04
//static struct proc_dir_entry *proc_entry;
int is_apk_debug;
wait_queue_head_t debug_queue;
int debug_sync_flag;
static unsigned char* apk_data = NULL;
#endif

#ifdef  BL_UPDATE_BY_I2C_MODE
int bl_chip_update_mode(u8 bEnter);
#define SET_WAKEUP_HIGH     bl_chip_update_mode(0)
#define SET_WAKEUP_LOW      bl_chip_update_mode(1)
#else
extern void bl_ts_set_int_gpio(unsigned char level);
#define SET_WAKEUP_HIGH     bl_ts_set_int_gpio(1)
#define SET_WAKEUP_LOW      bl_ts_set_int_gpio(0)
#endif

#define CHIP_ID_REG     0xe7

int bl_soft_reset_switch_int_wakemode()
{
    unsigned char cmd[4];
	int ret = 0x00;

	cmd[0] = RW_REGISTER_CMD;
	cmd[1] = ~cmd[0];
	cmd[2] = CHIP_ID_REG;
    cmd[3] = 0xe8;
	
    btl_i2c_client->addr = BTL_SLAVE_ADDR;
    ret = btl_i2c_write(btl_i2c_client, cmd, 4);
    btl_i2c_client->addr = BTL_SLAVE_ADDR;
	if(ret < 0){
		BF_TP_LOG("bl_soft_reset_switch_int_wakemode failed:i2c write flash error___\n");
	}
	
    mdelay(50);

    return ret;
}

int bl_get_chip_id(u8* buf)
{
    unsigned char cmd[3];
    int ret = 0x00;
    int retry = 3;

    unsigned int backup_i2c_addr = btl_i2c_client->addr;

    SET_WAKEUP_LOW;
    mdelay(50);
	bl_soft_reset_switch_int_wakemode();
    cmd[0] = RW_REGISTER_CMD;
    //cmd[1] = ~cmd[0];
    cmd[1] = 0xf5;
    cmd[2] = CHIP_ID_REG;

    while(retry --) {
        btl_i2c_client->addr = BTL_FLASH_ADDR;
        ret = btl_i2c_write(btl_i2c_client, cmd, 3);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;

        if(ret < 0) {
            BF_TP_LOG("___%s: btl i2c write flash error___\n", __func__);
            mdelay(30);
        } else {
            break;
        }
    }

    mdelay(2);
    btl_i2c_client->addr = BTL_FLASH_ADDR;
    ret = btl_i2c_apk_read(btl_i2c_client, buf, 1);
    btl_i2c_client->addr = BTL_SLAVE_ADDR;
    //ret = bl_i2c_transfer(BL_FLASH_I2C_ADDR, buf,1,I2C_READ);
    if(ret < 0) {
        BF_TP_LOG("___%s: btl i2c read flash error___\n", __func__);
        goto GET_CHIP_ID_ERROR;
    }

GET_CHIP_ID_ERROR:
    SET_WAKEUP_HIGH;
    btl_i2c_client->addr = backup_i2c_addr;
    mdelay(20);

    return ret;
}

static int bl_get_protect_flag(void)
{
    unsigned char ret = 0;
    unsigned char protectFlag = 0x00;
	unsigned char addr = 0xee;
	BF_TP_LOG("bl_get_protect_flag\n");
	btl_i2c_client->addr = BTL_SLAVE_ADDR;
	ret = btl_i2c_read(btl_i2c_client, &addr, 1, &protectFlag, 1);
	btl_i2c_client->addr = BTL_SLAVE_ADDR;
	if(ret < 0)
    {
    	BF_TP_LOG("bl_get_protect_flag failed,ret = %x\n",ret);
		return 0;
    }
	if(protectFlag == 0x55)
    {
        BF_TP_LOG("bl_get_protect_flag:protectFlag = %x\n",protectFlag);
		return 1;
    }
	return 0;
}

#ifdef  BL_UPDATE_BY_I2C_MODE

#define BL7XX0_I2C_IDLE_TIME    20
int bl_check_chip_update_mode(void)
{
    u8 i = 0;
    u8 cmd_buf[3] = {0x0A, 0xF5};
    u8 reg_addr[6] = {0x8F, 0x90, 0x91, 0x92, 0x93};
    u8 reg_value[6] = {0x00};
    int ret = -1;
    int retry = 3;

    //for(i = 0; i < 6; i++) {
    for(i = 0; i < 5; i++) {
        cmd_buf[2] = reg_addr[i];
        retry = 3;

        while(retry --) {
            btl_i2c_client->addr = BTL_FLASH_ADDR;
            ret = btl_i2c_write(btl_i2c_client, cmd_buf, 3);
            btl_i2c_client->addr = BTL_SLAVE_ADDR;
            if(ret < 0) {
                TPD_DEBUG("___%s:i2c write flash error___\n", __func__);
                mdelay(BL7XX0_I2C_IDLE_TIME);
            } else {
                break;
            }
        }

        btl_i2c_client->addr = BTL_FLASH_ADDR;
        ret = btl_i2c_apk_read(btl_i2c_client, &reg_value[i], 1);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;
        TPD_DEBUG("i2c read char :%02x\n", reg_value[i]);
        if(ret < 0) {
            TPD_DEBUG("___%s:i2c read flash error___\n", __func__);
            return -1;
        }
    }

    //TPD_DEBUG("%s,%s\n",__func__,reg_value);
    ret = strcmp(reg_value, "blest");

    return ret;
}

int bl_chip_update_mode(u8 bEnter)
{
    u8 i = 0;
    u8 retry = 5;
    u8 cmd_buf[210] = {0x00};
    int ret = -1;

    TPD_DEBUG("bl_chip_update_mode :%d\n", bEnter);

    if(bEnter) {
        //enter update mode
        for(i = 0; i < sizeof(cmd_buf); i += 2) {
            cmd_buf[i] = 0x5A;
            cmd_buf[i + 1] = 0xA5;
        }

        for(i = 0; i < retry; i++) {
            btl_i2c_client->addr = BTL_FLASH_ADDR;
            ret = btl_i2c_write(btl_i2c_client, cmd_buf, sizeof(cmd_buf));
            btl_i2c_client->addr = BTL_SLAVE_ADDR;
            if(ret < 0)
                continue;

            ret = bl_check_chip_update_mode();
            if(0 == ret) {
                TPD_DEBUG("enter chip update mode \n");
                break;
            }

        }

        mdelay(BL7XX0_I2C_IDLE_TIME);

    } else {
        //exit update mode
        cmd_buf[0] = 0x5A;
        cmd_buf[1] = 0xA5;

        for(i = 0; i < retry; i++) {
            mdelay(BL7XX0_I2C_IDLE_TIME);

            btl_i2c_client->addr = BTL_FLASH_ADDR;
            ret = btl_i2c_write(btl_i2c_client, cmd_buf, 2);
            btl_i2c_client->addr = BTL_SLAVE_ADDR;
            if(ret > 0)
                break;
        }
    }

    if(i == retry) {
        TPD_DEBUG("enter chip update mode fail \n");
        return -1;
    }

    return 0;
}
#endif


#ifdef  BL_UPDATE_FIRMWARE_ENABLE

static int bl_write_flash(unsigned char cmd, unsigned short flash_start_addr, unsigned char* buf, int len)
{
    unsigned char cmd_buf[6 + I2C_TRANSFER_WSIZE];
    unsigned short flash_end_addr;
    int ret = 0;

    if(!len) {
        TPD_DEBUG("btl ___write flash len is 0x00,return___\n");
        return -1;
    }

    flash_end_addr = flash_start_addr + len - 1;

    if(flash_end_addr >= MAX_FLASH_SIZE) {
        TPD_DEBUG("btl ___write flash end addr is overflow,return___\n");
        return -1;
    }

    cmd_buf[0] = cmd;
    cmd_buf[1] = ~cmd;
    cmd_buf[2] = flash_start_addr >> 0x08;
    cmd_buf[3] = flash_start_addr & 0xff;
    cmd_buf[4] = flash_end_addr >> 0x08;
    cmd_buf[5] = flash_end_addr & 0xff;

    memcpy(&cmd_buf[6], buf, len);

    if(btl_i2c_client) {
        btl_i2c_client->addr = BTL_FLASH_ADDR;
        ret = btl_i2c_write(btl_i2c_client, cmd_buf, len + 6);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;
    }

    if(ret < 0) {
        TPD_DEBUG("btl ___%s:i2c transfer error___\n", __func__);
        return -1;
    }

    return 0;
}

//btl flash read
int bl_read_flash(unsigned char cmd, unsigned short flash_start_addr, unsigned char* buf, int len)
{
    unsigned char cmd_buf[6];
    unsigned short flash_end_addr;
    int ret = 0;

    if(!len) {
        TPD_DEBUG("btl ___write flash len is 0x00,return___\n");
        return -1;
    }

    flash_end_addr = flash_start_addr + len - 1;

    if(flash_end_addr >= MAX_FLASH_SIZE) {
        TPD_DEBUG("btl ___write flash end addr is overflow,return___\n");
        return -1;
    }

    cmd_buf[0] = cmd;
    cmd_buf[1] = ~cmd;
    cmd_buf[2] = flash_start_addr >> 0x08;
    cmd_buf[3] = flash_start_addr & 0xff;
    cmd_buf[4] = flash_end_addr >> 0x08;
    cmd_buf[5] = flash_end_addr & 0xff;

    if(btl_i2c_client) {
        btl_i2c_client->addr = BTL_FLASH_ADDR;
        ret = btl_i2c_write(btl_i2c_client, cmd_buf, 6);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;
    }

    if(ret < 0) {
        TPD_DEBUG("btl ___%s:i2c transfer write error___\n", __func__);
        return -1;
    }

    btl_i2c_client->addr = BTL_FLASH_ADDR;
    ret = btl_i2c_apk_read(btl_i2c_client, buf, len);
    btl_i2c_client->addr = BTL_SLAVE_ADDR;

    if(ret < 0) {
        TPD_DEBUG("btl ___%s:i2c transfer read error___\n", __func__);
        return -1;
    }

    return 0;
}

static int bl_erase_flash(void)
{
    unsigned char cmd[2];
    int ret = -1;

    TPD_DEBUG("btl bl_erase_flash \n");
    cmd[0] = ERASE_ALL_MAIN_CMD;
    cmd[1] = ~cmd[0];

    if(btl_i2c_client) {
        btl_i2c_client->addr = BTL_FLASH_ADDR;
        ret = btl_write_reg(btl_i2c_client, cmd[0], cmd[1]);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;
    }

    if(ret < 0) {
        TPD_DEBUG("btl bl_erase_flash error\n");
        return -1;
    }

    return 0;
}

#ifdef BL_UPDATE_I2C_USE_400K_SPEED
//i2c speed 400k
static int bl_download_fw(u8* pfwbin, int fwsize)
{
    unsigned int i;
    unsigned char cmd_buf[6];
    unsigned short flash_start_addr = 0;
    unsigned short flash_end_addr;
    int ret = 0;
    unsigned char value = 0xFF;

    TPD_DEBUG("btl ___bl_download_fw flash start___\n");

    if(!bl_erase_flash()) {
        TPD_DEBUG("btl ___erase flash ok___\n");
    } else {
        TPD_DEBUG("btl ___erase flash error___\n");
        return -1;
    }


    mdelay(250);
    SET_WAKEUP_HIGH;
    mdelay(50);
    SET_WAKEUP_LOW;
    mdelay(50);
    bl_soft_reset_switch_int_wakemode();
    flash_end_addr = fwsize - 1;

    if(flash_end_addr >= MAX_FLASH_SIZE) {
        TPD_DEBUG("btl ___write flash end addr is overflow,return___\n");
        return -1;
    }

    cmd_buf[0] = WRITE_MAIN_CMD;
    cmd_buf[1] = ~WRITE_MAIN_CMD;
    cmd_buf[2] = flash_start_addr >> 0x08;
    cmd_buf[3] = flash_start_addr & 0xff;
    cmd_buf[4] = flash_end_addr >> 0x08;
    cmd_buf[5] = flash_end_addr & 0xff;

    if(btl_i2c_client) {
        btl_i2c_client->addr = BTL_FLASH_ADDR;
        ret = btl_i2c_write(btl_i2c_client, cmd_buf, 6);
        btl_i2c_client->addr = BTL_SLAVE_ADDR;
    }

    if(ret < 0) {
        TPD_DEBUG("btl ___%s:i2c 400k transfer write cmd error___\n", __func__);
        return -1;
    }

    btl_i2c_client->addr = BTL_FLASH_ADDR;
    for(i = 0; i < fwsize; i++) {
        if(i == VERTIFY_START_OFFSET || i == VERTIFY_END_OFFSET) {
            ret = btl_i2c_write(btl_i2c_client, &value, 1);
        } else {
            ret = btl_i2c_write(btl_i2c_client, (unsigned char*)&fwbin[i], 1);
        }

        if(ret < 0) {
            TPD_DEBUG("btl ___%s:i2c 400k transfer write data error___\n", __func__);
            btl_i2c_client->addr = BTL_SLAVE_ADDR;
            return -1;
        }
        udelay(5);
    }
    btl_i2c_client->addr = BTL_SLAVE_ADDR;

    return 0;
}

#else
//i2c speed 100k 200k 300k
static int bl_download_fw(u8* pfwbin, int fwsize)
{
    unsigned int i;
    unsigned short size, len;
    unsigned short addr;
    u8* pbt_buf = NULL;

    if(!bl_erase_flash()) {
        TPD_DEBUG("btl ___erase flash ok___\n");
    } else {
        TPD_DEBUG("btl ___erase flash error___\n");
        return -1;
    }


    mdelay(250);
    SET_WAKEUP_HIGH;
    mdelay(50);
    SET_WAKEUP_LOW;
    mdelay(50);
    bl_soft_reset_switch_int_wakemode();
    pbt_buf = kmalloc(fwsize + 1, GFP_KERNEL);
    if(pbt_buf == NULL) {
        TPD_DEBUG(" btl ERROR:Get buf failed\n");
        return -1;
    }
    memcpy(pbt_buf, pfwbin, fwsize);

    *(pbt_buf + VERTIFY_START_OFFSET) = 0xFF;
    *(pbt_buf + VERTIFY_END_OFFSET) = 0xFF;

    for(i = 0; i < fwsize;) {

        size = fwsize - i;

        if(size > I2C_TRANSFER_WSIZE) {
            len = I2C_TRANSFER_WSIZE;
        } else {
            len = size;
        }

        addr = i;

        TPD_DEBUG("btl bl_download_fw len=%d fwbin[%d]=%d\n", len, i, fwbin[i]);
        //if(bl_write_flash(WRITE_MAIN_CMD,addr, (unsigned char *)&fwbin[i],len)){
        if(bl_write_flash(WRITE_MAIN_CMD, addr, pbt_buf + i, len)) {
            kfree(pbt_buf);
            TPD_DEBUG("btl bl_download_fw error !!!\n");
            return -1;
        }

        i += len;
        udelay(5);

    }

    kfree(pbt_buf);
    return 0;
}

#endif

//bin文件的checksum
static int bl_get_bin_checksum(unsigned char* fwbin, int fwlen, unsigned short* p_bin_checksum)
{
    int i = 0;

    unsigned char*  p_fw = (unsigned char*)fwbin;
    unsigned short  bin_checksum = 0;

    for(i = 0; i < fwlen; i++) {
        bin_checksum += p_fw[i];
    }
    for(i = fwlen; i < MAX_FLASH_SIZE; i++) {
        bin_checksum += 0xff;
    }

    *p_bin_checksum = bin_checksum;
    return 0;
}

#ifdef BL_UPDATE_ARGUMENT_CHECKSUM
static int bl_get_argument_checksum(unsigned short* argument_checksum)
{
    unsigned char buf[3];
    unsigned char checksum_ready = 0;
    int retry = 3;
    int ret = 0x00;
    unsigned char addr = 0x00;

    buf[0] = CHECKSUM_CAL_REG;
    buf[1] = CHECKSUM_ARG;//0xba

    mdelay(FW_CHECKSUM_DELAY_TIME);

    while(retry--) {
        ret = btl_write_reg(btl_i2c_client, buf[0], buf[1]);
        if(ret < 0) {
            TPD_DEBUG("___%s: btl write checksum cmd error retry:%d__\n", __func__, retry);
            mdelay(30);
        } else {
            break;
        }
    }

    mdelay(FW_CHECKSUM_DELAY_TIME);

    addr = CHECKSUM_REG;
    ret = btl_i2c_read(btl_i2c_client, &addr, 1, buf, 3);
    if(ret < 0) {
        TPD_DEBUG("___%s: btl read checksum error_1__\n", __func__);
        return -1;
    }

    checksum_ready = buf[0];

    retry = 5;
    while((retry--) && (checksum_ready != CHECKSUM_READY)) {
        mdelay(50);
        ret = btl_i2c_read(btl_i2c_client, &addr, 1, buf, 3);
        if(ret < 0) {
            TPD_DEBUG("___%s: btl retry= %d read checksum error_2__\n", __func__, retry);
            return -1;
        }

        checksum_ready = buf[0];
    }

    if(checksum_ready != CHECKSUM_READY) {
        TPD_DEBUG("___%s: btl read checksum fail_3__\n", __func__);
        return -1;
    }
    *argument_checksum = (buf[1] << 8) + buf[2];

    return 0;
}
#endif

static int bl_get_fw_checksum(unsigned short* fw_checksum)
{
    unsigned char buf[3];
    unsigned char checksum_ready = 0;
    int retry = 3;
    int ret = 0x00;
    unsigned char addr = 0x00;

    buf[0] = CHECKSUM_CAL_REG;
    buf[1] = CHECKSUM_CAL;

    mdelay(FW_CHECKSUM_DELAY_TIME);

    while(retry--) {
        ret = btl_write_reg(btl_i2c_client, buf[0], buf[1]);
        if(ret < 0) {
            TPD_DEBUG("___%s: btl write checksum cmd error retry:%d__\n", __func__, retry);
            mdelay(30);
        } else {
            break;
        }
    }
    mdelay(FW_CHECKSUM_DELAY_TIME);

    addr = CHECKSUM_REG;
    ret = btl_i2c_read(btl_i2c_client, &addr, 1, buf, 3);
    if(ret < 0) {
        TPD_DEBUG("___%s: btl read checksum error_1__\n", __func__);
        return -1;
    }

    checksum_ready = buf[0];

    retry = 60;
    while((retry--) && (checksum_ready != CHECKSUM_READY)) {
        mdelay(50);
        ret = btl_i2c_read(btl_i2c_client, &addr, 1, buf, 3);
        if(ret < 0) {
            TPD_DEBUG("___%s: btl retry= %d read checksum error_2__\n", __func__, retry);
            return -1;
        }

        checksum_ready = buf[0];
    }

    if(checksum_ready != CHECKSUM_READY) {
        TPD_DEBUG("___%s: btl read checksum fail_3__:%d\n", __func__, checksum_ready);
        return -1;
    }
    *fw_checksum = (buf[1] << 8) + buf[2];

    return 0;
}


static int bl_write_check_value(unsigned char start_value, unsigned char end_value)
{
    unsigned char i = 0;
    unsigned char vertify_check[2] = {0};//校验值 0xAA 0x55写入0x3FC 0x3FD
    unsigned char vertify_value[2] = {0};//校验值 0xAA 0x55写入0x3FC 0x3FD

    vertify_value[0] = start_value;
    vertify_value[1] = end_value;

    for(i = 0; i < 3; i++) {
        if(bl_write_flash(WRITE_MAIN_CMD, VERTIFY_START_OFFSET, vertify_value, 2)) {
            TPD_DEBUG("btl bl_write_check_value bl_write_flash vertify_value error!\n");
            continue;
        } else {
            mdelay(5);
            TPD_DEBUG(" btl bl_write_check_value bl_write_flash vertify_value ok!\n");
            if(bl_read_flash(READ_MAIN_CMD, VERTIFY_START_OFFSET, vertify_check, 2)) {
                TPD_DEBUG(" btl bl_write_check_value bl_read_flash error!\n");
                continue;
            }

            //if(strcmp(vertify_value, vertify_check))
            if((vertify_value[0] == vertify_check[0]) && (vertify_value[1] == vertify_check[1])) {
                TPD_DEBUG("btl bl_write_check_value bl_memcmp ok!\n");
                break;
            } else {
                TPD_DEBUG("btl bl_write_check_value bl_memcmp error!\n");
                continue;
            }
        }
    }

    if(i == 3) {
        return -1;
    }

    return 0;
}

static int bl_read_flash_vertify(void)
{
	unsigned char cnt = 0;
	int ret = 0;
	unsigned char vertify[2] = {0xAA, 0x55};
	unsigned char vertify1[2] = {0};
	SET_WAKEUP_LOW;
	mdelay(50);
	bl_soft_reset_switch_int_wakemode();
	while(cnt < 3)
	{
        ret = bl_read_flash(READ_MAIN_CMD, VERTIFY_START_OFFSET, vertify1, 2);
		if(ret < 0)
		{
			TPD_DEBUG("bl_write_flash_vertify: read fail\n");
			continue;
		}

		if(memcmp(vertify, vertify1, 2) == 0)
		{
			ret = 0;
			break;
		}
		else
        {
            ret = -1;
        }
		cnt++;
	}
	SET_WAKEUP_HIGH;
	mdelay(20);
	return ret;
}

int bl_update_flash(unsigned char update_type, u8* pfwbin, int fwsize)
{
    unsigned short fw_checksum = !BIN_CHECKSUM;
    unsigned short bin_checksum = 0x0;
    int retry = 0;
    int ret = 0;

    ret = bl_get_fw_checksum(&fw_checksum);
    if(ret < 0) {
        TPD_DEBUG("btl fw update start bl_get_fw_checksum error\n");
        //return -1;
    }

    if(update_type == INIT_UPDATE) { //probe .h update
        TPD_DEBUG("btl fw update start .h init,fw checksum = 0x%x,bin checksum = 0x%x___\n", fw_checksum, BIN_CHECKSUM);
    } else if(update_type == BIN_UPDATE) { //sys/class/i2c-adapter/i2c-x/fwupdate update
        ret = bl_get_bin_checksum(pfwbin, fwsize, &bin_checksum);
        TPD_DEBUG("btl fw update start sys fwupdate ,fw_checksum=0x%x,bin_checksum = 0x%x___\n", fw_checksum, bin_checksum);
    } else {
        TPD_DEBUG("btl fw update start error update type\n");
        return -1;
    }

    tpd_gpio_output(btl_tpd_rst_gpio_number, 1);

    retry = 3;
    while((retry--) && (((update_type == INIT_UPDATE) && (fw_checksum != BIN_CHECKSUM))
                        || ((update_type == BIN_UPDATE) && (bin_checksum != fw_checksum)))) {
        SET_WAKEUP_LOW;

        mdelay(100);//50
		bl_soft_reset_switch_int_wakemode();
        ret = bl_download_fw(pfwbin, fwsize);
        if(ret < 0) {
            TPD_DEBUG("btl fw update start bl_download_fw error retry=%d\n", retry);
            continue;
        }

        mdelay(50);

        SET_WAKEUP_HIGH;

        mdelay(200);

        ret = bl_get_fw_checksum(&fw_checksum);
        if(ret < 0) {
            TPD_DEBUG("btl fw update start bl_download_fw bl_get_fw_checksum error");
            continue;
        } else {
            fw_checksum = fw_checksum - 0xFF - 0xFF + fwbin[VERTIFY_START_OFFSET] + fwbin[VERTIFY_END_OFFSET];
        }

        TPD_DEBUG("btl fw update end,fw checksum = 0x%x,bin checksum =0x%x, BIN_CHECKSUM = 0x%x___\n", fw_checksum, bin_checksum, BIN_CHECKSUM);

        if(((update_type == INIT_UPDATE) && (fw_checksum == BIN_CHECKSUM))
           || ((update_type == BIN_UPDATE) && (fw_checksum == bin_checksum))) {
            SET_WAKEUP_LOW;
            mdelay(100);//50
            bl_soft_reset_switch_int_wakemode();
            ret = bl_write_check_value(pfwbin[VERTIFY_START_OFFSET], pfwbin[VERTIFY_END_OFFSET]);

            SET_WAKEUP_HIGH;
            mdelay(200);

            if(ret < 0) {
                TPD_DEBUG("btl fw update 0xAA 0x55 check error___\n");
                if(update_type == INIT_UPDATE) {
                    fw_checksum = !BIN_CHECKSUM;
                } else if(update_type == BIN_UPDATE) {
                    fw_checksum = !bin_checksum;
                }
            } else {
                TPD_DEBUG("btl fw update 0xAA 0x55 check ok___\n");
                break;
            }
        }
    }

    if(retry == 0) {
        TPD_DEBUG("btl fw update error\n");
        return 1;
    }

    TPD_DEBUG("btl fw update success___\n");

    return 0;
}

int bl_update_fw(struct i2c_client* client)
{
    u8 fwverdata[3];
    //u8 chip_id = 0x00;
    int ret_FW = 0x01;
    int ret = 0x01;
    u8 fwverdata_addr = BL_FWVER_REG;


    ret_FW = btl_i2c_read(client, &fwverdata_addr, 1, fwverdata, 3);

    BF_TP_LOG("ret_FW=%d fwverdata fw=%d arg=%d vendorID=%d fwbin[main]=%d fwbin[argu]=%d fwbin[vendor]=%d\n", ret_FW, fwverdata[0], fwverdata[1], fwverdata[1],fwbin[BL_FWVER_MAIN_OFFSET], fwbin[BL_FWVER_ARGU_OFFSET],fwbin[BL_FACTORY_ID_OFFSET]);

	if(((ret_FW < 0) 		
		|| ((ret_FW == 0) && (fwverdata[0] == 0))		
		|| ((ret_FW == 0) && (fwverdata[0] == 0xff)) 		
		|| ((ret_FW == 0) && (fwverdata[0] == BL_FWVER_REG))		
		|| (bl_read_flash_vertify() < 0)
		|| (fwverdata[0] != fwbin[BL_FWVER_MAIN_OFFSET])
		|| (fwverdata[1] != fwbin[BL_FWVER_ARGU_OFFSET]))
		&& (!bl_get_protect_flag()))	
	{
	
        bl_disable_irq();
        ret = bl_update_flash(INIT_UPDATE, (u8*)fwbin, sizeof(fwbin));
        tpd_gpio_as_int(btl_tpd_int_gpio_number);

        //mt_set_gpio_pull_enable(5, GPIO_PULL_ENABLE);
        //mt_set_gpio_pull_select(5, GPIO_PULL_UP);
        //msleep(50);

        bl_enable_irq();
    }

    return ret;
}


static int fts_GetFirmwareSize(char* firmware_name)
{
    struct file* pfile = NULL;
    struct inode* inode;
    unsigned long magic;
    off_t fsize = 0;
    char filepath[128];
    memset(filepath, 0, sizeof(filepath));

    sprintf(filepath, "/sdcard/%s", firmware_name);

    if(NULL == pfile)
        pfile = filp_open(filepath, O_RDONLY, 0);

    if(IS_ERR(pfile)) {
        pr_err("btl error occured while opening file %s.\n", filepath);
        return -EIO;
    }

    //inode = pfile->f_dentry->d_inode;
    inode = pfile->f_inode;
    magic = inode->i_sb->s_magic;
    fsize = inode->i_size;
    filp_close(pfile, NULL);
    return fsize;
}

static int fts_ReadFirmware(char* firmware_name, unsigned char* firmware_buf)
{
    struct file* pfile = NULL;
    struct inode* inode;
    unsigned long magic;
    off_t fsize;
    char filepath[128];
    loff_t pos;
    mm_segment_t old_fs;
    memset(filepath, 0, sizeof(filepath));
    sprintf(filepath, "/sdcard/%s", firmware_name);
    if(NULL == pfile)
        pfile = filp_open(filepath, O_RDONLY, 0);

    if(IS_ERR(pfile)) {
        pr_err("btl error occured while opening file %s.\n", filepath);
        return -EIO;
    } else {
        TPD_DEBUG("btl open file is ok\n");
    }

    //inode = pfile->f_dentry->d_inode;
    inode = pfile->f_inode;
    magic = inode->i_sb->s_magic;
    fsize = inode->i_size;
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    vfs_read(pfile, firmware_buf, fsize, &pos);
    //TPD_DEBUG("btl vfs_read pos=%d\n",pos);
    filp_close(pfile, NULL);
    set_fs(old_fs);
    return 0;
}

int bl_fw_upgrade_with_app_file(struct i2c_client* client, char* firmware_name)
{
    u8* pbt_buf = NULL;
    int i_ret = 0;

    int fwsize = fts_GetFirmwareSize(firmware_name);
    TPD_DEBUG("btl fwsize = %d\n", fwsize);
    if(fwsize <= 0) {
        dev_err(&client->dev, "btl %s ERROR:Get firmware size failed\n", __func__);
        return -EIO;
    }
    if(fwsize < 8 || fwsize > MAX_FLASH_SIZE) {
        dev_dbg(&client->dev, "btl %s:FW length error\n", __func__);
        return -EIO;
    }

    /*=========FW upgrade========================*/
    pbt_buf = kmalloc(fwsize + 1, GFP_KERNEL);
    if(pbt_buf == NULL) {
        dev_err(&client->dev, "btl %s ERROR:Get buf failed\n", __func__);
    } else {
        TPD_DEBUG("btl kmalloc is ok\n");
    }

    if(fts_ReadFirmware(firmware_name, pbt_buf)) {
        dev_err(&client->dev, "btl %s() - ERROR: request_firmware failed\n", __func__);
        kfree(pbt_buf);
        return -EIO;
    } else {
        TPD_DEBUG("btl copy firmware\n");
    }

    bl_disable_irq();
    bl_i2c_lock(1);
    i_ret = bl_update_flash(BIN_UPDATE, pbt_buf, fwsize);
    bl_i2c_lock(0);
    tpd_gpio_as_int(btl_tpd_int_gpio_number);
    bl_enable_irq();
    kfree(pbt_buf);

    return i_ret;
}
#endif

#ifdef BTL_APK_SUPPORT
// ****************************************************************************
// fs/apk interface function define/declaration
// ****************************************************************************
//cmd|i2c_addr|data_len|ndata
static int bl_apk_open(struct inode* inode, struct file* file)
{

    apk_data = kzalloc(1024, GFP_KERNEL);
    if(apk_data == NULL) {
        TPD_DEBUG("btl can't allocate memory for apk\n");
        return -1;
    }
    return 0;
}

static int bl_apk_close(struct inode* inode, struct file* file)
{

    if(apk_data != NULL) {
        kfree(apk_data);
    }
    return 0;
}

static ssize_t bl_apk_write(struct file* file, const char __user* buff, size_t count, loff_t* offp)
{

    int ret = 0;
    unsigned short len;
    //unsigned char addr;

    if(copy_from_user(apk_data, buff, count)) {
        return -EFAULT;
    }

    len = apk_data[2] + (apk_data[3] << 8);

    switch(apk_data[0]) {

    case WRITE_CMD : {
        if(is_apk_debug == 1) {
            wait_event_interruptible(debug_queue, debug_sync_flag);
            debug_sync_flag = 0;
        } else {
            //cmd|i2c_addr|data_len 低8位|data_len 高8位|ndata|
            //ret = bl_i2c_transfer(apk_data[1], &apk_data[FRAME_CMD_LEN], len,I2C_WRITE);//lgj test
            //addr = i2c_client->addr;
            bl_i2c_lock(1);
            btl_i2c_client->addr = apk_data[1];
            ret = btl_i2c_write(btl_i2c_client, &apk_data[FRAME_CMD_LEN], len);
            btl_i2c_client->addr = BTL_SLAVE_ADDR;
            bl_i2c_lock(0);
        }

        if(ret < 0) {
            TPD_DEBUG(" btl ___%s:i2c transfer error___\n", __func__);
            return -1;
        }
    }
    break;

    case WAKE_CMD : {
        if(apk_data[FRAME_CMD_LEN]) {
            tpd_gpio_output(btl_tpd_int_gpio_number, 1);
            //tpd_gpio_output(btl_tpd_rst_gpio_number, 1);
            tpd_gpio_as_int(btl_tpd_int_gpio_number);
            //mt_set_gpio_pull_enable(5, GPIO_PULL_ENABLE);
            //mt_set_gpio_pull_select(5, GPIO_PULL_UP);

            //msleep(50);
        } else {
            tpd_gpio_output(btl_tpd_int_gpio_number, 0);
            //tpd_gpio_output(btl_tpd_rst_gpio_number, 0);
        }

    }
    break;

    case INTERRUPT_CMD : {
        if(apk_data[FRAME_CMD_LEN]) {
            bl_enable_irq();
        } else {
            bl_disable_irq();
        }

    }
    break;

    case I2C_LOCK_CMD : {
        TPD_DEBUG("btl bl_I2C_LOCK_CMD=%d\n", apk_data[FRAME_CMD_LEN]);
        if(apk_data[FRAME_CMD_LEN]) {
            bl_i2c_lock(1);
        } else {
            bl_i2c_lock(0);
        }
    }
    break;

    default : {
        return -1;
    }
    break;

    }

    return count;
}


static ssize_t bl_apk_read(struct file* file, char __user* buff, size_t count, loff_t* offp)
{
    unsigned short len;
    //unsigned char addr;
    int ret = 0;

    if(copy_from_user(apk_data, buff, FRAME_CMD_LEN)) {
        return -EFAULT;
    }

    len = apk_data[2] + (apk_data[3] << 8);
    //address = apk_data[1];
    switch(apk_data[0]) {

    case READ_CMD : {
        if(is_apk_debug == 1) {
            debug_sync_flag = 0;
            TPD_DEBUG(" btl ___%s %d:i2c transfer \n", __func__, __LINE__);
            wait_event_interruptible(debug_queue, debug_sync_flag);
        } else {
            //int btl_i2c_read(struct i2c_client *client, unsigned char *writebuf,int writelen, unsigned char *readbuf, int readlen)
            //cmd|i2c_addr|data_len|ndata
            //ret = bl_i2c_transfer(apk_data[1], apk_data, len,I2C_READ);//lgj test
            bl_i2c_lock(1);
            btl_i2c_client->addr = apk_data[1];
            ret = btl_i2c_apk_read(btl_i2c_client, apk_data, len);
            btl_i2c_client->addr = BTL_SLAVE_ADDR;
            bl_i2c_lock(0);
        }

        if(ret < 0) {
            TPD_DEBUG("btl ___%s %d:i2c transfer \n", __func__, __LINE__);
            return -1;
        }
    }
    break;

    case DRIVER_INFO : {
        apk_data[0] = 0x00;
    }
    break;

    default : {
        return -1;
    }
    break;

    }

    if(copy_to_user(buff, apk_data, len)) {
        return -EFAULT;
    }

    return count;
}

void apk_i2c_data_transfer(void)
{
    unsigned short len;
    //unsigned char address;

    int ret = 0;

    if(apk_data == NULL)
        return;

    len = apk_data[2] + (apk_data[3] << 8);
    TPD_DEBUG("\napk_i2c_data_transfer data[1]=%x len=%d\n\n", apk_data[1], len);

    //bl_i2c_transfer(apk_data[1], apk_data, len,I2C_READ);//lgj test
    //int btl_i2c_read(struct i2c_client *client, unsigned char *writebuf,int writelen, unsigned char *readbuf, int readlen)
    //cmd|i2c_addr|data_len|ndata
    //ret = bl_i2c_transfer(apk_data[1], apk_data, len,I2C_READ);//lgj test
    bl_i2c_lock(1);
    btl_i2c_client->addr = apk_data[1];
    ret = btl_i2c_apk_read(btl_i2c_client, apk_data, len);
    btl_i2c_client->addr = BTL_SLAVE_ADDR;
    bl_i2c_lock(0);

    if(ret < 0) {
        TPD_DEBUG("btl ___%s %d:i2c transfer error___\n", __func__, __LINE__);
        return;
    }
    TPD_DEBUG("btl ___%s %d:apk_data[0]=%x apk_data[1]=%x \n", __func__, __LINE__, apk_data[0], apk_data[1]);
}

struct file_operations bl_apk_fops = {
    .owner   = THIS_MODULE,
    .open    = bl_apk_open,
    .release = bl_apk_close,

    .read    = bl_apk_read,
    .write   = bl_apk_write,
};
#endif

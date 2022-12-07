
#ifndef ____LINUX_PLATFORM_DATA_SPI_MTK_H
#define ____LINUX_PLATFORM_DATA_SPI_MTK_H

/* Board specific platform_data */
struct mtk_chip_config {
//Add-begin by baiwei.peng for TP porting on 2020/09/03
        u32 tx_mlsb;
        u32 rx_mlsb;
        u32 cs_pol;
//Add-end by baiwei.peng for TP porting on 2020/09/03
	u32 sample_sel;

	//patch the parameter as customized
	u32 cs_setuptime;
	u32 cs_holdtime;
	u32 cs_idletime;
	u32 deassert_mode;
	u32 tick_delay;
};
#endif

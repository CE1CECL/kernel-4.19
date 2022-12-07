obj-$(CONFIG_MTK_MMC) += mtk_sd.o
mtk_sd-objs := sd.o dbg.o
ifeq ($(CONFIG_MTK_AEE_KDUMP),y)
EXTRA_CFLAGS		+= -DMT6575_SD_DEBUG
endif

clean:
	@rm -f *.o modules.order .*.cmd

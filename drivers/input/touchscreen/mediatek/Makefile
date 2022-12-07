subdir-ccflags-y += -Werror
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/video/include/
# In case the platform does NOT support this type of sensors
obj-y   +=  mtk_tpd.o
obj-y   +=  tpd_button.o
obj-y   +=  tpd_calibrate.o
obj-y   +=  tpd_debug.o
obj-y   +=  tpd_default.o
obj-y   +=  tpd_misc.o
obj-y   +=  tpd_setting.o

#Add-begin by baiwei.peng for TP porting on 2020/09/03
obj-$(CONFIG_TOUCHSCREEN_MTK_TOKYOLITETMO_NT36525BH)    +=  tokyolitetmo_nt36525bh_touch/
obj-$(CONFIG_TOUCHSCREEN_MTK_TOKYOLITETMO_FT8006S)      +=  tokyolitetmo_ft8006s_touch/
#Add-end by baiwei.peng for TP porting on 2020/09/03

obj-$(CONFIG_TOUCHSCREEN_MTK_FT5446_BANGKOKTF)      +=  bangkoktf_ft5446/
obj-$(CONFIG_TOUCHSCREEN_MTK_CHSC5XXX_BANGKOKTF)      +=  bangkoktf_CHSC5XXX/
obj-$(CONFIG_TOUCHSCREEN_MTK_BETTERLIFE_BANGKOKTF)      +=  bangkoktf_betterlife_ts/
obj-$(CONFIG_TOUCHSCREEN_MTK_GT1151_BANGKOKTF)      +=  bangkoktf_GT1151/




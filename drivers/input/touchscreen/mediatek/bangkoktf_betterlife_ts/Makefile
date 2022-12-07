
# Linux driver folder

ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include/
ccflags-y += -I$(srctree)/drivers/base
ccflags-y += -I$(srctree)/drivers/misc/mediatek/hwmon/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/alsps/inc/
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/alsps/inc/
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/bangkoktf_betterlife_ts


obj-y	+=  betterlife_driver.o
obj-y	+=  betterlife_upgrade.o
obj-y	+=  betterlife_ps.o
obj-y	+=  betterlife_factory.o

obj-y	+=  bl_test.o
obj-y	+=  bl_test_bl7401.o
obj-y	+=  bl_test_ini.o





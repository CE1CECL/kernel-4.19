# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/include

obj-y   +=  accel.o accel_factory.o

#obj-$(CONFIG_MTK_ACCELHUB) += accelhub/
#obj-$(CONFIG_MTK_BMI160_I2C)   += bmi160-i2c/
#obj-$(CONFIG_MTK_LIS3DH_NEW) += lis3dh/
#obj-$(CONFIG_MTK_MC3410_I2C) += mc3410-i2c/
#obj-$(CONFIG_MTK_MC3433) += mc3433/
#obj-$(CONFIG_MTK_MPU6050G) += mpu6050g/
#begin modified by zhikui.li,bring up sensor
obj-$(CONFIG_TOKYO_2ND_MC3416)   += tokyo_2nd_MC3416/
obj-$(CONFIG_SEOUL_2ND_MC3416)   += seoul_2nd_MC3416/
obj-$(CONFIG_SEOUL_1ST_STK8BA53)   += Seoul_1st_STK8BA53/
obj-$(CONFIG_TOKYO_1ST_STK8BA53)   += TOKYO_1st_STK8BA53/
obj-$(CONFIG_MTK_SIGNIFICANT_SENSOR)   +=  significant_motion/
#end modified by zhikui.li,bring up sensor
obj-$(CONFIG_TCT_1ST_STK8BA53)   += TCT_1ST_STK8BA53/
obj-$(CONFIG_TCT_2ND_MC3416)   += TCT_2ND_MC3416/
obj-$(CONFIG_MTK_SIGNIFICANT_SENSOR)   +=  significant_motion/

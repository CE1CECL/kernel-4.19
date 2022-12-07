# In case the platform does NOT support this type of sensors
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
obj-y   +=  alsps.o
obj-y   +=  alsps_factory.o
obj-y   +=  aal_control.o

#obj-$(CONFIG_MTK_ALSPSHUB) += alspshub/
#obj-$(CONFIG_MTK_CM36558) += cm36558/
#obj-$(CONFIG_MTK_LTR303) += ltr303/
obj-$(CONFIG_TCT_STK3321_39A)  +=  TCT_STK3321_39A/
#[Feature] Add-Begin by zhikui.li@tcl.com for porting pl-sensor driver
obj-$(CONFIG_MTK_STK3X1X_TCT)	+=  stk3x1x_tct/
#[Feature] Add-End by zhikui.li@tcl.com for porting pl-sensor driver
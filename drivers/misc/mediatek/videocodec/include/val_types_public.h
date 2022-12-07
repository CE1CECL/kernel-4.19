/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _VAL_TYPES_PUBLIC_H_
#define _VAL_TYPES_PUBLIC_H_

/* #include <sys/types.h> */
/* #include <linux/ion.h> */

/* /< support max 16 return register values when HW done */
#define IRQ_STATUS_MAX_NUM 16

/* /< support max 16 multiple thread currently */
#define VCODEC_THREAD_MAX_NUM 16


#define VAL_VOID_T void     /* /< void type definition */
#define VAL_BOOL_T char     /* /< char type definition */
#define VAL_CHAR_T char     /* /< char type definition */
#define VAL_INT8_T signed char      /* /< signed char type definition */
#define VAL_INT16_T signed short    /* /< signed short type definition */
#define VAL_INT32_T signed int      /* /< signed int type definition */
#define VAL_UCHAR_T unsigned char   /* /< unsigned char type definition */
#define VAL_UINT8_T unsigned char   /* /< unsigned char type definition */
#define VAL_UINT16_T unsigned short /* /< unsigned short definition */
#define VAL_UINT32_T unsigned int   /* /< unsigned int type definition */
#define VAL_UINT64_T unsigned long long /* unsigned long long type definition */
#define VAL_INT64_T long long   /* long long type definition */
#define VAL_HANDLE_T unsigned long /* unsigned int (handle) type definition */
#define VAL_LONG_T signed long
#define VAL_ULONG_T unsigned long

#define VAL_NULL (0)  /* /< VAL_NULL = 0 */
#define VAL_TRUE (1)  /* /< VAL_TRUE = 1 */
#define VAL_FALSE (0) /* /< VAL_FALSE = 0 */

#define VAL_RESOLUTION_CHANGED (2)

enum VAL_MEM_CODEC_T {
	VAL_MEM_CODEC_FOR_VENC = 0,    /* /< Memory for Video Encoder */
	VAL_MEM_CODEC_FOR_VDEC,	/* /< Memory for Video Decoder */
	VAL_MEM_CODEC_MAX = 0xFFFFFFFF /* /< Max Value */
};

enum VAL_CHIP_NAME_T {
	VAL_CHIP_NAME_MT6516 = 0, /* /< MT6516 */
	VAL_CHIP_NAME_MT6571,     /* /< MT6571 */
	VAL_CHIP_NAME_MT6572,     /* /< MT6572 */
	VAL_CHIP_NAME_MT6573,     /* /< MT6573 */
	VAL_CHIP_NAME_MT6575,     /* /< MT6575 */
	VAL_CHIP_NAME_MT6577,     /* /< MT6577 */
	VAL_CHIP_NAME_MT6589,     /* /< MT6589 */
	VAL_CHIP_NAME_MT6582,     /* /< MT6582 */
	VAL_CHIP_NAME_MT8135,     /* /< MT8135 */
	VAL_CHIP_NAME_ROME,       /* /< ROME */
	VAL_CHIP_NAME_MT6592,     /* /< MT6592 */
	VAL_CHIP_NAME_MT8127,     /* /< MT8127 */
	VAL_CHIP_NAME_MT6752,     /* /< MT6752 */
	VAL_CHIP_NAME_MT6795,     /* /< MT6795 */
	VAL_CHIP_NAME_DENALI_1,   /* /< MT6737-1 */
	VAL_CHIP_NAME_DENALI_2,   /* /< MT6737-2 */
	VAL_CHIP_NAME_DENALI_3,   /* /< MT6737-3 */
	VAL_CHIP_NAME_MT6570,     /* /< MT6570 (2 core) */
	VAL_CHIP_NAME_MT6580,     /* /< MT6580 (4 core) */
	VAL_CHIP_NAME_MT8163,
	VAL_CHIP_NAME_MT8173,	  /* / <8173 */
	VAL_CHIP_NAME_MT6755,	  /* / <MT6755 */
	VAL_CHIP_NAME_MT6757,	  /* / <MT6757 */
	VAL_CHIP_NAME_MT6797,	  /* / <MT6797 */
	VAL_CHIP_NAME_MT7623,	  /* / <MT7623 */
	VAL_CHIP_NAME_MT8167,	  /* / <MT8167 */
	VAL_CHIP_NAME_ELBRUS,	  /* /< ELBRUS */
	VAL_CHIP_NAME_MT6799,	  /* /< MT6799 */
	VAL_CHIP_NAME_MT6759,	  /* /< MT6759 */
	VAL_CHIP_NAME_MT6758,	  /* / <MT6758 */
	VAL_CHIP_NAME_MT6763,	  /* /< MT6763 */
	VAL_CHIP_NAME_MT6739,	  /* /< MT6739 */
	VAL_CHIP_NAME_MT6771,	  /* /< MT6771 */
	VAL_CHIP_NAME_MT6775,	  /* /< MT6775 */
	VAL_CHIP_NAME_MT6765,	  /* /< MT6765 */
	VAL_CHIP_NAME_MT3967,	  /* /< MT3967 */
	VAL_CHIP_NAME_MT6761,	  /* /< MT6761 */
	VAL_CHIP_NAME_MAX = 0xFFFFFFFF /* /< Max Value */
};

enum VAL_CHIP_VARIANT_T {
	VAL_CHIP_VARIANT_MT6571L = 0,     /* /< MT6571L */
	VAL_CHIP_VARIANT_MAX = 0xFFFFFFFF /* /< Max Value */
};

enum VAL_CHIP_VERSION_T {
	/* /< The data will be "6595" for 6595 series; "6795" for 6795 series,
	 *...
	 */
	VAL_CHIP_VERSION_HW_CODE = 0,
	/* /< The data will be "0000" for E1; "0001" for E2, ... */
	VAL_CHIP_VERSION_SW_VER,
	/* /< Max Value */
	VAL_CHIP_VERSION_MAX = 0xFFFFFFFF
};

enum VAL_DRIVER_TYPE_T {
	VAL_DRIVER_TYPE_NONE = 0,	   /* /< None */
	VAL_DRIVER_TYPE_MP4_ENC,	    /* /< MP4 encoder */
	VAL_DRIVER_TYPE_MP4_DEC,	    /* /< MP4 decoder */
	VAL_DRIVER_TYPE_H263_ENC,	   /* /< H.263 encoder */
	VAL_DRIVER_TYPE_H263_DEC,	   /* /< H.263 decoder */
	VAL_DRIVER_TYPE_H264_ENC,	   /* /< H.264 encoder */
	VAL_DRIVER_TYPE_H264_DEC,	   /* /< H.264 decoder */
	VAL_DRIVER_TYPE_SORENSON_SPARK_DEC, /* /< Sorenson Spark decoder */
	VAL_DRIVER_TYPE_VC1_SP_DEC,      /* /< VC-1 simple profile decoder */
	VAL_DRIVER_TYPE_RV9_DEC,	 /* /< RV9 decoder */
	VAL_DRIVER_TYPE_MP1_MP2_DEC,     /* /< MPEG1/2 decoder */
	VAL_DRIVER_TYPE_XVID_DEC,	/* /< Xvid decoder */
	/* /< VC-1 main profile (WMV9) decoder */
	VAL_DRIVER_TYPE_VC1_MP_WMV9_DEC,
	VAL_DRIVER_TYPE_RV8_DEC,       /* /< RV8 decoder */
	VAL_DRIVER_TYPE_WMV7_DEC,      /* /< WMV7 decoder */
	VAL_DRIVER_TYPE_WMV8_DEC,      /* /< WMV8 decoder */
	VAL_DRIVER_TYPE_AVS_DEC,       /* /< AVS decoder */
	/* /< H.264 main profile decoder (due to different packet) == 20 */
	VAL_DRIVER_TYPE_H264_DEC_MAIN,
	/* /< H.264 main profile decoder for CABAC type but packet is the same,
	 * just for reload.
	 */
	VAL_DRIVER_TYPE_H264_DEC_MAIN_CABAC,
	VAL_DRIVER_TYPE_VP8_DEC,     /* /< VP8 decoder */
	VAL_DRIVER_TYPE_MP2_DEC,     /* /< MPEG2 decoder */
	VAL_DRIVER_TYPE_VP9_DEC,     /* /< VP9 decoder */
	VAL_DRIVER_TYPE_VP8_ENC,     /* /< VP8 encoder */
	VAL_DRIVER_TYPE_VC1_ADV_DEC, /* /< VC1 advance decoder */
	VAL_DRIVER_TYPE_VC1_DEC,     /* /< VC1 simple/main/advance decoder */
	VAL_DRIVER_TYPE_JPEG_ENC,    /* /< JPEG encoder */
	VAL_DRIVER_TYPE_HEVC_ENC,    /* /< HEVC encoder */
	VAL_DRIVER_TYPE_HEVC_DEC,    /* /< HEVC decoder */
	VAL_DRIVER_TYPE_H264_ENC_LIVEPHOTO, /* LivePhoto type */
	VAL_DRIVER_TYPE_MMDVFS,		    /* /< MMDVFS */
	VAL_DRIVER_TYPE_VP9_ENC,	    /* /< VP9 encoder */
	VAL_DRIVER_TYPE_MAX = 0xFFFFFFFF    /* /< Max driver type */
};

enum VAL_RESULT_T {
	VAL_RESULT_NO_ERROR = 0,      /* /< The function work successfully */
	VAL_RESULT_INVALID_DRIVER,    /* /< Error due to invalid driver */
	VAL_RESULT_INVALID_PARAMETER, /* /< Error due to invalid parameter */
	VAL_RESULT_INVALID_MEMORY,    /* /< Error due to invalid memory */
	VAL_RESULT_INVALID_ISR,       /* /< Error due to invalid isr request */
	VAL_RESULT_ISR_TIMEOUT,       /* /< Error due to invalid isr request */
	VAL_RESULT_UNKNOWN_ERROR,     /* /< Unknown error */
	VAL_RESULT_RESTARTSYS,	/* /< Restart sys */
	VAL_RESULT_MAX = 0xFFFFFFFF   /* /< Max result */
};

enum VAL_MEM_ALIGN_T {
	VAL_MEM_ALIGN_1 = 1,	   /* /< 1 byte alignment */
	VAL_MEM_ALIGN_2 = (1 << 1),    /* /< 2 byte alignment */
	VAL_MEM_ALIGN_4 = (1 << 2),    /* /< 4 byte alignment */
	VAL_MEM_ALIGN_8 = (1 << 3),    /* /< 8 byte alignment */
	VAL_MEM_ALIGN_16 = (1 << 4),   /* /< 16 byte alignment */
	VAL_MEM_ALIGN_32 = (1 << 5),   /* /< 32 byte alignment */
	VAL_MEM_ALIGN_64 = (1 << 6),   /* /< 64 byte alignment */
	VAL_MEM_ALIGN_128 = (1 << 7),  /* /< 128 byte alignment */
	VAL_MEM_ALIGN_256 = (1 << 8),  /* /< 256 byte alignment */
	VAL_MEM_ALIGN_512 = (1 << 9),  /* /< 512 byte alignment */
	VAL_MEM_ALIGN_1K = (1 << 10),  /* /< 1K byte alignment */
	VAL_MEM_ALIGN_2K = (1 << 11),  /* /< 2K byte alignment */
	VAL_MEM_ALIGN_4K = (1 << 12),  /* /< 4K byte alignment */
	VAL_MEM_ALIGN_8K = (1 << 13),  /* /< 8K byte alignment */
	VAL_MEM_ALIGN_MAX = 0xFFFFFFFF /* /< Max memory byte alignment */
};

enum VAL_MEM_TYPE_T {
	VAL_MEM_TYPE_FOR_SW = 0, /* /< External memory foe SW */
	/* /< External memory for HW Cacheable */
	VAL_MEM_TYPE_FOR_HW_CACHEABLE,
	/* /< External memory for HW Cacheable, with MCI port config */
	VAL_MEM_TYPE_FOR_HW_CACHEABLE_MCI,
	/* /< External memory for HW Non-Cacheable */
	VAL_MEM_TYPE_FOR_HW_NONCACHEABLE,
	VAL_MEM_TYPE_MAX = 0xFFFFFFFF /* /< Max memory type */
};

struct VAL_MEM_ADDR_T { /* union extend 64bits for TEE*/
	union {
		unsigned long u4VA; /* /< [IN/OUT] virtual address */
		unsigned long long u4VA_ext64;
	};
	union {
		unsigned long u4PA; /* /< [IN/OUT] physical address */
		unsigned long long u4PA_ext64;
	};
	union {
		unsigned long u4Size; /* /< [IN/OUT] size */
		unsigned long long u4Size_ext64;
	};
};

struct VAL_VCODEC_THREAD_ID_T {
	unsigned int u4tid1;
	unsigned int u4tid2;
	unsigned int u4VCodecThreadNum;
	unsigned int u4VCodecThreadID[VCODEC_THREAD_MAX_NUM];
};

struct VAL_VCODEC_CPU_LOADING_INFO_T {
	unsigned long long _cpu_idle_time;   /* /< [OUT] cpu idle time */
	unsigned long long _thread_cpu_time; /* /< [OUT] thread cpu time */
	unsigned long long _sched_clock;     /* /< [OUT] sched clock */
	unsigned int _inst_count;	    /* /< [OUT] inst count */
};

struct VAL_VCODEC_CPU_OPP_LIMIT_T {
	int limited_freq; /* /< [IN] limited freq */
	int limited_cpu;  /* /< [IN] limited cpu */
	int enable;       /* /< [IN] enable */
};

struct VAL_VCODEC_M4U_BUFFER_CONFIG_T {
	enum VAL_MEM_CODEC_T eMemCodec;
	unsigned int cache_coherent;
	unsigned int security;
};

struct VAL_MEMORY_T { /* union extend 64bits for TEE*/
	unsigned int u4MemSign;
	enum VAL_MEM_TYPE_T eMemType;
	union {
		unsigned long u4MemSize;
		unsigned long long u4MemSize_ext64;
	};
	union {
		void *pvMemVa;
		unsigned long long pvMemVa_ext64;
	};
	union {
		void *pvMemPa;
		unsigned long long pvMemPa_ext64;
	};
	enum VAL_MEM_ALIGN_T eAlignment;
	union {
		void *pvAlignMemVa;
		unsigned long long pvAlignMemVa_ext64;
	};
	union {
		void *pvAlignMemPa;
		unsigned long long pvAlignMemPa_ext64;
	};
	enum VAL_MEM_CODEC_T eMemCodec;
	unsigned int i4IonShareFd;

	union {
		struct ion_handle *pIonBufhandle;
		unsigned long long pIonBufhandle_ext64;
	};
	union {
		void *pvReserved;
		unsigned long long pvReserved_ext64;
	};
	union {
		unsigned long u4ReservedSize;
		unsigned long long u4ReservedSize_ext64;
	};
#ifdef __EARLY_PORTING__
	union {
		void *pvReservedPmem;
		unsigned long long pvReservedPmem_ext64;
	};
#endif
	unsigned int i4IonDevFd;
};

struct VAL_RECORD_SIZE_T {
	unsigned int u4FrmWidth;
	unsigned int u4FrmHeight;
	unsigned int u4BufWidth;
	unsigned int u4BufHeight;
};

struct VAL_ATOI_T {
	void *pvStr;
	int i4Result;
	void *pvReserved;
	unsigned int u4ReservedSize;
};

struct VAL_STRSTR_T {
	void *pvStr;
	void *pvStrSearch;
	void *pvStrResult;
	void *pvReserved;
	unsigned int u4ReservedSize;
};

struct VAL_ISR_T {
	void *pvHandle;
	unsigned int u4HandleSize;
	enum VAL_DRIVER_TYPE_T eDriverType;
	void *pvIsrFunction;
	void *pvReserved;
	unsigned int u4ReservedSize;
	unsigned int u4TimeoutMs;
	unsigned int u4IrqStatusNum;
	unsigned int u4IrqStatus[IRQ_STATUS_MAX_NUM];
};

struct VAL_HW_LOCK_T {
	void *pvHandle;
	unsigned int u4HandleSize;
	void *pvLock;
	unsigned int u4TimeoutMs;
	void *pvReserved;
	unsigned int u4ReservedSize;
	enum VAL_DRIVER_TYPE_T eDriverType;
	char bSecureInst;
};

struct VAL_TIME_T {
	unsigned int u4Sec;  /* /< [IN/OUT] second */
	unsigned int u4uSec; /* /< [IN/OUT] micro second */
};

enum VAL_SET_TYPE_T {
	VAL_SET_TYPE_CURRENT_SCENARIO,  /* /< Set current scenario */
	VAL_SET_TYPE_MCI_PORT_CONFIG,   /* /< Set MCI port config */
	VAL_SET_TYPE_M4U_PORT_CONFIG,   /* /< Set M4U port config */
	VAL_SET_TYPE_SET_TCM_ON,	/* /< Set TCM on */
	VAL_SET_TYPE_SET_TCM_OFF,       /* /< Set TCM off */
	VAL_SET_TYPE_SET_AV_TASK_GROUP, /* /< Set AV task grouping */
	VAL_SET_FRAME_INFO, /* /< Set current frame info for PM QoS */
};

enum VAL_GET_TYPE_T {
	VAL_GET_TYPE_CURRENT_SCENARIO_CNT,
	VAL_GET_TYPE_LCM_INFO,
};

enum VAL_VCODEC_SCENARIO_T {
	VAL_VCODEC_SCENARIO_VENC_1080P = 0x1, /* /< Camera recording 1080P */
	VAL_VCODEC_SCENARIO_VDEC_1080P = 0x2, /* /< Playback 1080P */
	VAL_VCODEC_SCENARIO_VENC_WFD = 0x4,   /* /< Wifi-display encoding */
	VAL_VCODEC_SCENARIO_VDEC_60FPS = 0x8, /* /< Playback 60fps video */
	VAL_VCODEC_SCENARIO_VDEC_4K = 0x10,   /* /< Playback 4K */
	VAL_VCODEC_SCENARIO_VDEC_2K = 0x20,   /* /< Playback 2K */
	VAL_VCODEC_SCENARIO_VENC_4K = 0x40,   /* /< VR 4K */
};

struct VAL_CURRENT_SCENARIO_T {
	unsigned int u4Scenario;
	unsigned int u4OnOff;
};

struct VAL_CURRENT_SCENARIO_CNT_T {
	unsigned int u4Scenario;
	unsigned int u4ScenarioRefCount;
};

struct _VAL_MCI_PORT_CONFIG_T {
	enum VAL_MEM_CODEC_T eMemCodecType;
	unsigned int u4Config;
};

struct VAL_LCM_INFO_T {
	unsigned int u4Width;  /* /< [OUT] width */
	unsigned int u4Height; /* /< [OUT] height */
};

/* /< VAL_M4UPORT_DEFAULT_ALL = 1, config all M4U port for VENC or VDEC */
#define VAL_M4U_PORT_ALL (-1)

struct VAL_M4U_MPORT_CONFIG_T {
	enum VAL_MEM_CODEC_T eMemCodec;
	unsigned int i4M4UPortID;
	char bSecurity;
	char bVirtuality;
};

/* for DirectLink Meta Mode + */
#define META_HANDLE_LIST_MAX 50

struct VAL_MetaBufInfo {
	void *pNativeHandle;
	unsigned long u4VA;
	unsigned long u4PA;
	unsigned int u4BuffSize;
	char bUseION;
	int fd;
	struct ion_handle *pIonBufhandle;
};

struct VAL_MetaHandleList {
	int mIonDevFd;
	struct VAL_MetaBufInfo rMetaBufInfo[META_HANDLE_LIST_MAX];
	char fgSeqHdrEncoded;
};

struct VAL_BufInfo {
	unsigned char fgIsConfigData;
	unsigned long u4BSVA;
	unsigned char fgBSStatus;
	unsigned char fgIsKeyFrame;
	unsigned int u4BSSize;
};
/* for DirectLink Meta Mode - */

struct VAL_FRAME_INFO_T {
	void *handle; /* driver handle */
	enum VAL_DRIVER_TYPE_T driver_type;
	unsigned int input_size; /* input bitstream bytes */
	unsigned int frame_width;
	unsigned int frame_height; /* field pic has half height */
	/* 0: intra, 1: inter 1 ref, 2: inter 2 ref, 3: copy */
	unsigned int frame_type;
	unsigned int is_compressed; /* is output buffer compressed */
};

#endif /* #ifndef _VAL_TYPES_PUBLIC_H_ */

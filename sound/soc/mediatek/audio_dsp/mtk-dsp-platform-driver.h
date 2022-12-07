/* SPDX-License-Identifier: GPL-2.0 */


#ifndef _MTK_DSP_PLATFORM_DRIVER_H_
#define _MTK_DSP_PLATFORM_DRIVER_H_

struct mtk_base_afe;

#define AFE_DSP_NAME "AUDIO_DSP_PCM"

extern const struct snd_soc_component_driver mtk_dsp_pcm_platform;
extern unsigned int SmartpaSwdspProcessEnable;

#endif

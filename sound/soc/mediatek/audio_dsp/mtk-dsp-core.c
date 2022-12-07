// SPDX-License-Identifier: GPL-2.0

#include <adsp_helper.h>

bool is_adsp_core_ready(void)
{
	return is_adsp_ready(ADSP_A_ID);
}

bool is_adsp_feature_registered(void)
{
	return false;
}


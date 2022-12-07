/************************************************************************
* Copyright (c) 2012-2020, Betterlife Systems (R), All Rights Reserved.
*
* File Name: bl_test_bl7401.c
*
* Author: Betterlife Driver Team
*
* Created: 2015-07-14
*
* Abstract:
*
************************************************************************/

/*****************************************************************************
* Included header files
*****************************************************************************/
#include "bl_test.h"

/*****************************************************************************
* Private constant and macro definitions using #define
*****************************************************************************/

/*****************************************************************************
* Private enumerations, structures and unions using typedef
*****************************************************************************/

/*****************************************************************************
* Static variables
*****************************************************************************/

/*****************************************************************************
* Global variable or extern global variabls/functions
*****************************************************************************/

/*****************************************************************************
* Static function prototypes
*****************************************************************************/
static int short_ch_gnd_bl7401(
    struct btl_test *tdata,
    u8 param,
    int *adc,
    int *resistor,
    bool *result)
{
    int i = 0;
    bool tmp_result = true;
    struct mc_sc_threshold *thr = &tdata->ic.mc_sc.thr;
    int tx = tdata->sc_node.tx_num;

    BTL_TEST_FUNC_ENTER();
    if (!adc || !result) {
        BTL_TEST_SAVE_ERR("adc/result is null\n");
        return -EINVAL;
    }

    BTL_TEST_DBG("short cg adc:\n");
    btl_print_buffer(adc, tdata->sc_node.channel_num, 0);

    btl_show_data_mc_sc(adc);

    for (i = 0; i < tdata->sc_node.channel_num; i++) {
        if (adc[i] < thr->basic.short_cg) {
            tmp_result = false;
            if (i < tdata->sc_node.tx_num) {
                BTL_TEST_SAVE_ERR("TX%d:%d ", i + 1, adc[i]);
            } else {
                BTL_TEST_SAVE_ERR("RX%d:%d ", i + 1 - tx, adc[i]);
            }
        }
    }

    BTL_TEST_DBG("short cg resistor:\n");
    btl_print_buffer(adc, tdata->sc_node.channel_num, 0);

    *result = tmp_result;
    BTL_TEST_FUNC_EXIT();
    return 0;
}

static int bl7401_rawdata_test(struct btl_test *tdata, bool *test_result)
{
    int ret = 0;
    bool result = false;
    int *rawdata = NULL;
    int *rawdata_tmp = NULL;
    int rawdata_cnt = 0;
    struct mc_sc_threshold *thr = &tdata->ic.mc_sc.thr;

    BTL_TEST_FUNC_ENTER();
    BTL_TEST_SAVE_INFO("\n============ Test Item: rawdata test\n");
    memset(tdata->buffer, 0, tdata->buffer_length);
    rawdata = tdata->buffer;

    if (!thr->rawdata_h_min || !thr->rawdata_h_max) {
        BTL_TEST_SAVE_ERR("rawdata_h_min/max is null\n");
        ret = -EINVAL;
        goto test_err;
    }

    ret = btl_enter_factory_mode();
    if (ret < 0) {
        BTL_TEST_SAVE_ERR("failed to enter factory mode,ret=%d\n", ret);
        goto test_err;
    }
	
    BTL_TEST_SAVE_INFO( "bl7401_rawdata_test:\n");
    rawdata_tmp = rawdata + rawdata_cnt;
    ret = btl_get_rawdata_mc(0xff, 0xff, rawdata_tmp);
    if (ret < 0) {
    	BTL_TEST_SAVE_ERR("get rawdata fail,ret=%d\n", ret);
    	goto test_err;
    }
    
    btl_show_data(rawdata_tmp, false);
    
    /* compare */
    result = btl_compare_array(rawdata_tmp,
    					   thr->rawdata_h_min,
    					   thr->rawdata_h_max,
    					   false);
    
    rawdata_cnt += tdata->node.node_num;

test_err:
    if (result) {
        *test_result = true;
        BTL_TEST_SAVE_INFO("------ Rawdata Test PASS\n");
    } else {
        *test_result = false;
        BTL_TEST_SAVE_INFO("------ Rawdata Test NG\n");
    }

    /* save data */
    btl_test_save_data("Rawdata Test", CODE_M_RAWDATA_TEST,
                       rawdata, rawdata_cnt, false, false, *test_result);

    BTL_TEST_FUNC_EXIT();
    return ret;
}

static int bl7401_scap_cb_test(struct btl_test *tdata, bool *test_result)
{
    int ret = 0;
    bool tmp_result = false;
    int byte_num = 0;
    bool tx_check = false;
    bool rx_check = false;
    int *scap_cb = NULL;
    int *scb_tmp = NULL;
    int scb_cnt = 0;
    struct mc_sc_threshold *thr = &tdata->ic.mc_sc.thr;

    BTL_TEST_FUNC_ENTER();
    BTL_TEST_SAVE_INFO("\n============ Test Item: Scap CB Test\n");
    memset(tdata->buffer, 0, tdata->buffer_length);
    scap_cb = tdata->buffer;
    byte_num = tdata->sc_node.node_num;

    if ((tdata->sc_node.node_num * 2) > tdata->buffer_length) {
        BTL_TEST_SAVE_ERR("scap cb num(%d) > buffer length(%d)",
                          tdata->sc_node.node_num * 2,
                          tdata->buffer_length);
        ret = -EINVAL;
        goto test_err;
    }

    if (!thr->scap_cb_off_min || !thr->scap_cb_off_max) {
        BTL_TEST_SAVE_ERR("scap_cb_on/off_min/max is null\n");
        ret = -EINVAL;
        goto test_err;
    }

    ret = btl_enter_factory_mode();
    if (ret < 0) {
        BTL_TEST_SAVE_ERR("enter factory mode fail,ret=%d\n", ret);
        goto test_err;
    }

	ret = btl_sc_cb_calibrate();
    if (ret < 0) {
        BTL_TEST_SAVE_ERR("cb calibrate fail,ret=%d\n", ret);
        goto test_err;
    }

    BTL_TEST_SAVE_INFO( "bl7401_scap_cb_test:\n");
	scb_tmp = scap_cb + scb_cnt;
	/* 1:waterproof 0:non-waterproof */
	ret = btl_get_cb_mc_sc(WATER_PROOF_OFF, byte_num, scb_tmp, DATA_ONE_BYTE);
	if (ret < 0) {
		BTL_TEST_SAVE_ERR("read sc_cb fail,ret=%d\n", ret);
		goto test_err;
	}
	
	/* show & save Scap CB */
	BTL_TEST_SAVE_INFO("scap_cb in waterproof off mode:\n");
	btl_show_data_mc_sc(scb_tmp);

	tx_check = false;
	rx_check = true;	
	tmp_result = btl_compare_sc_cb(tx_check, rx_check, scb_tmp,
								thr->scap_cb_off_min,
								thr->scap_cb_off_max);
	
	scb_cnt += tdata->sc_node.node_num;

test_err:
    if (tmp_result) {
        *test_result = true;
        BTL_TEST_SAVE_INFO("------ SCAP CB Test PASS\n");
    } else {
        *test_result = false;
        BTL_TEST_SAVE_ERR("------ SCAP CB Test NG\n");
    }

    /* save data */
    btl_test_save_data("SCAP CB Test", CODE_M_SCAP_CB_TEST,
                       scap_cb, scb_cnt, true, false, *test_result);

    BTL_TEST_FUNC_EXIT();
    return ret;
}

static int bl7401_scap_rawdata_test(struct btl_test *tdata, bool *test_result)
{
    int ret = 0;
    bool tmp_result = false;
    bool tx_check = false;
    bool rx_check = false;
    int *scap_rawdata = NULL;
    int *srawdata_tmp = NULL;
    int srawdata_cnt = 0;
    struct mc_sc_threshold *thr = &tdata->ic.mc_sc.thr;

    BTL_TEST_FUNC_ENTER();
    BTL_TEST_SAVE_INFO("\n============ Test Item: Scap Rawdata Test\n");
    memset(tdata->buffer, 0, tdata->buffer_length);
    scap_rawdata = tdata->buffer;

    if ((tdata->sc_node.node_num * 2) > tdata->buffer_length) {
        BTL_TEST_SAVE_ERR("scap rawdata num(%d) > buffer length(%d)",
                          tdata->sc_node.node_num * 2,
                          tdata->buffer_length);
        ret = -EINVAL;
        goto test_err;
    }
    BTL_TEST_DBG("scap rawdata num(%d) > buffer length(%d)",
                          tdata->sc_node.node_num * 2,
                          tdata->buffer_length);
    if (!thr->scap_rawdata_on_min || !thr->scap_rawdata_on_max) {
        BTL_TEST_SAVE_ERR("scap_rawdata_on/off_min/max is null\n");
        ret = -EINVAL;
        goto test_err;
    }

	srawdata_tmp = scap_rawdata + srawdata_cnt;
	ret = btl_get_rawdata_mc_sc(WATER_PROOF_ON, srawdata_tmp);
	if (ret < 0) {
		BTL_TEST_SAVE_ERR("get scap(WP_ON) rawdata fail\n");
		goto test_err;
	}
	
	BTL_TEST_SAVE_INFO("scap_rawdata in waterproof on mode:\n");
	btl_show_data_mc_sc(srawdata_tmp);
	
	/* compare */
	tx_check = false;
	rx_check = true;
	tmp_result = btl_compare_mc_sc(tx_check, rx_check, srawdata_tmp,
							   thr->scap_rawdata_on_min,
							   thr->scap_rawdata_on_max);
	
	srawdata_cnt += tdata->sc_node.node_num;
test_err:
    if (tmp_result) {
        *test_result = true;
        BTL_TEST_SAVE_INFO("------ SCAP Rawdata Test PASS\n");
    } else {
        *test_result = false;
        BTL_TEST_SAVE_INFO("------ SCAP Rawdata Test NG\n");
    }

    /* save data */
    btl_test_save_data("SCAP Rawdata Test", CODE_M_SCAP_RAWDATA_TEST,
                       scap_rawdata, srawdata_cnt, true, false, *test_result);
    BTL_TEST_FUNC_EXIT();
    return ret;
}

static int bl7401_short_test(struct btl_test *tdata, bool *test_result)
{
    int ret = 0;
    int i = 0;
    bool tmp_result = false;
    int adc_num = 0;
    int *adc = NULL;

    BTL_TEST_SAVE_INFO("\n============ Test Item: Short Test\n");

    BTL_TEST_FUNC_ENTER();
	
	memset(tdata->buffer, 0, tdata->buffer_length);
    adc = tdata->buffer;
	
    adc_num = tdata->sc_node.channel_num * 2;
    if (adc_num > tdata->buffer_length) {
        BTL_TEST_SAVE_ERR("adc num(%d) > buffer len(%d)", \
                          adc_num, tdata->buffer_length);
        ret = -EIO;
        goto test_err;
    }

    /* get adc data */
    for (i = 0; i < 5; i++) {
        ret = btl_short_get_adc_data_mc(0xff, adc_num, adc, \
                                    0xff);
        if (ret >= 0)
            break;
        else
            BTL_TEST_DBG("retry:%d", i);
    }
    if (i >= 5) {
        BTL_TEST_SAVE_ERR("get adc data timeout\n");
        ret = -EIO;
        goto test_err;
    }

    /* channel to gnd */
    ret = short_ch_gnd_bl7401(tdata, 0xff, adc, NULL, &tmp_result);
    if (ret < 0) {
        BTL_TEST_SAVE_ERR("channel to gnd fail\n");
    }

test_err:
    if (tmp_result) {
        *test_result = true;
        BTL_TEST_SAVE_INFO("------ Short Test PASS\n");
    } else {
        *test_result = false;
        BTL_TEST_SAVE_ERR("code1:%d,offset:%d\n", tdata->code1, tdata->offset);
        BTL_TEST_SAVE_ERR("------ Short Test NG\n");
    }
    BTL_TEST_FUNC_EXIT();
    return ret;
}

static int bl7401_panel_differ_test(struct btl_test *tdata, bool *test_result)
{
    int ret = 0;
    bool tmp_result = false;
    int *panel_differ = NULL;
	
    struct mc_sc_threshold *thr = &tdata->ic.mc_sc.thr;

    BTL_TEST_FUNC_ENTER();
    BTL_TEST_SAVE_INFO("\n============ Test Item: Panel Differ Test\n");
    memset(tdata->buffer, 0, tdata->buffer_length);
    panel_differ = tdata->buffer;

    
    if (!thr->panel_differ_min || !thr->panel_differ_max) {
        BTL_TEST_SAVE_ERR("panel_differ_h_min/max is null\n");
        ret = -EINVAL;
        goto test_err;
    }

    ret = btl_get_panel_diff(panel_differ,thr->panel_differ_high_freq,thr->panel_differ_low_freq);
    if (ret < 0) {
        BTL_TEST_SAVE_ERR("get differ fail,ret=%d\n", ret);
        goto test_err;
    }

    /* show & save test data */
    btl_show_data(panel_differ, false);

    /* compare */
    tmp_result = btl_compare_diff(panel_differ,
                               thr->panel_differ_min,
                               thr->panel_differ_max,
                               false);

test_err:
    /* result */
    if (tmp_result) {
        *test_result = true;
        BTL_TEST_SAVE_INFO("------ Panel Diff Test PASS\n");
    } else {
        * test_result = false;
        BTL_TEST_SAVE_ERR("------ Panel Diff Test NG\n");
    }

    /* save test data */
    btl_test_save_data("Panel Differ Test", CODE_M_PANELDIFFER_TEST,
                       panel_differ, 0, false, false, *test_result);

    BTL_TEST_FUNC_EXIT();
    return ret;
}

static int start_test_bl7401(void)
{
    int ret = 0;
    struct btl_test *tdata = btl_ftest;
    struct mc_sc_testitem *test_item = &tdata->ic.mc_sc.u.item;
    bool temp_result = false;
    bool test_result = true;

    BTL_TEST_FUNC_ENTER();
    BTL_TEST_INFO("test item:0x%x", btl_ftest->ic.mc_sc.u.tmp);

    /* rawdata test */
    if (true == test_item->rawdata_test) {
        ret = bl7401_rawdata_test(tdata, &temp_result);
        if ((ret < 0) || (false == temp_result)) {
            test_result = false;
        }
    }

    /* scap_cb test */
    if (true == test_item->scap_cb_test) {
        ret = bl7401_scap_cb_test(tdata, &temp_result);
        if ((ret < 0) || (false == temp_result)) {
            test_result = false;
        }
    }

    /* scap_rawdata test */
    if (true == test_item->scap_rawdata_test) {
        ret = bl7401_scap_rawdata_test(tdata, &temp_result);
        if ((ret < 0) || (false == temp_result)) {
            test_result = false;
        }
    }

    /* short test */
    if (true == test_item->short_test) {
		g_test_item = 1;
        ret = bl7401_short_test(tdata, &temp_result);
		g_test_item = 0;
        if ((ret < 0) || (false == temp_result)) {
            test_result = false;
        }
    }

    /* panel differ test */
    if (true == test_item->panel_differ_test) {
        ret = bl7401_panel_differ_test(tdata, &temp_result);
        if ((ret < 0) || (false == temp_result)) {
            test_result = false;
        }
    }
	
    return test_result;
}

struct test_funcs test_func_bl7401 = {
    .ctype = {0x41, 0x41},
    .hwtype = IC_HW_MC_SC,
    .key_num_total = 0,
    .rawdata2_support = false,
    .raw_u16 = false,
	.mc_sc_short_v2 = false,
    .startscan_mode = SCAN_NORMAL,
    .start_test = start_test_bl7401,
};

// SPDX-License-Identifier: GPL-2.0-only
//
// aw88261.h  --  AW88261 ALSA SoC Audio driver
//
// Copyright (c) 2023 awinic Technology CO., LTD
//
// Author: Jimmy Zhang <zhangjianming@awinic.com>
// Author: Weidong Wang <wangweidong.a@awinic.com>
//

#ifndef __AW88261_H__
#define __AW88261_H__

#define AW88261_ID_REG			(0x00)
#define AW88261_SYSST_REG		(0x01)
#define AW88261_SYSINT_REG		(0x02)
#define AW88261_SYSINTM_REG		(0x03)
#define AW88261_SYSCTRL_REG		(0x04)
#define AW88261_SYSCTRL2_REG		(0x05)
#define AW88261_I2SCTRL1_REG		(0x06)
#define AW88261_I2SCTRL2_REG		(0x07)
#define AW88261_I2SCTRL3_REG		(0x08)
#define AW88261_DACCFG1_REG		(0x09)
#define AW88261_DACCFG2_REG		(0x0A)
#define AW88261_DACCFG3_REG		(0x0B)
#define AW88261_DACCFG4_REG		(0x0C)
#define AW88261_DACCFG5_REG		(0x0D)
#define AW88261_DACCFG6_REG		(0x0E)
#define AW88261_DACCFG7_REG		(0x0F)
#define AW88261_DACCFG8_REG		(0x10)
#define AW88261_PWMCTRL1_REG		(0x11)
#define AW88261_PWMCTRL2_REG		(0x12)
#define AW88261_I2SCFG1_REG		(0x13)
#define AW88261_DBGCTRL_REG		(0x14)
#define AW88261_DACCFG9_REG		(0x15)
#define AW88261_DACCFG10_REG		(0x16)
#define AW88261_DACST_REG		(0x20)
#define AW88261_VBAT_REG		(0x21)
#define AW88261_TEMP_REG		(0x22)
#define AW88261_PVDD_REG		(0x23)
#define AW88261_ISNDAT_REG		(0x24)
#define AW88261_VSNDAT_REG		(0x25)
#define AW88261_I2SINT_REG		(0x26)
#define AW88261_I2SCAPCNT_REG		(0x27)
#define AW88261_ANASTA1_REG		(0x28)
#define AW88261_ANASTA2_REG		(0x29)
#define AW88261_ANASTA3_REG		(0x2A)
#define AW88261_TESTDET_REG		(0x2B)
#define AW88261_DSMCFG1_REG		(0x30)
#define AW88261_DSMCFG2_REG		(0x31)
#define AW88261_DSMCFG3_REG		(0x32)
#define AW88261_DSMCFG4_REG		(0x33)
#define AW88261_DSMCFG5_REG		(0x34)
#define AW88261_DSMCFG6_REG		(0x35)
#define AW88261_DSMCFG7_REG		(0x36)
#define AW88261_DSMCFG8_REG		(0x37)
#define AW88261_TESTIN_REG		(0x38)
#define AW88261_TESTOUT_REG		(0x39)
#define AW88261_SADCCTRL1_REG		(0x3A)
#define AW88261_SADCCTRL2_REG		(0x3B)
#define AW88261_SADCCTRL3_REG		(0x3C)
#define AW88261_SADCCTRL4_REG		(0x3D)
#define AW88261_SADCCTRL5_REG		(0x3E)
#define AW88261_SADCCTRL6_REG		(0x3F)
#define AW88261_SADCCTRL7_REG		(0x40)
#define AW88261_VSNTM1_REG		(0x50)
#define AW88261_VSNTM2_REG		(0x51)
#define AW88261_ISNCTRL1_REG		(0x52)
#define AW88261_ISNCTRL2_REG		(0x53)
#define AW88261_PLLCTRL1_REG		(0x54)
#define AW88261_PLLCTRL2_REG		(0x55)
#define AW88261_PLLCTRL3_REG		(0x56)
#define AW88261_CDACTRL1_REG		(0x57)
#define AW88261_CDACTRL2_REG		(0x58)
#define AW88261_DITHERCFG1_REG		(0x59)
#define AW88261_DITHERCFG2_REG		(0x5A)
#define AW88261_DITHERCFG3_REG		(0x5B)
#define AW88261_CPCTRL_REG		(0x5C)
#define AW88261_BSTCTRL1_REG		(0x60)
#define AW88261_BSTCTRL2_REG		(0x61)
#define AW88261_BSTCTRL3_REG		(0x62)
#define AW88261_BSTCTRL4_REG		(0x63)
#define AW88261_BSTCTRL5_REG		(0x64)
#define AW88261_BSTCTRL6_REG		(0x65)
#define AW88261_BSTCTRL7_REG		(0x66)
#define AW88261_BSTCTRL8_REG		(0x67)
#define AW88261_BSTCTRL9_REG		(0x68)
#define AW88261_TM_REG			(0x6F)
#define AW88261_TESTCTRL1_REG		(0x70)
#define AW88261_TESTCTRL2_REG		(0x71)
#define AW88261_EFCTRL1_REG		(0x72)
#define AW88261_EFCTRL2_REG		(0x73)
#define AW88261_EFWH_REG		(0x74)
#define AW88261_EFWM2_REG		(0x75)
#define AW88261_EFWM1_REG		(0x76)
#define AW88261_EFWL_REG		(0x77)
#define AW88261_EFRH4_REG		(0x78)
#define AW88261_EFRH3_REG		(0x79)
#define AW88261_EFRH2_REG		(0x7A)
#define AW88261_EFRH1_REG		(0x7B)
#define AW88261_EFRL4_REG		(0x7C)
#define AW88261_EFRL3_REG		(0x7D)
#define AW88261_EFRL2_REG		(0x7E)
#define AW88261_EFRL1_REG		(0x7F)

#define AW88261_REG_MAX		(0x80)
#define AW88261_EF_DBMD_MASK		(0xfff7)
#define AW88261_OR_VALUE		(0x0008)

#define AW88261_TEMH_MASK		(0x83ff)
#define AW88261_TEML_MASK		(0x83ff)
#define AW88261_DEFAULT_CFG		(0x0000)

#define AW88261_ICALK_SHIFT		(0)
#define AW88261_ICALKL_SHIFT		(0)
#define AW88261_VCALK_SHIFT		(0)
#define AW88261_VCALKL_SHIFT		(0)

#define AW88261_AMPPD_START_BIT	(1)
#define AW88261_AMPPD_BITS_LEN		(1)
#define AW88261_AMPPD_MASK		\
	(~(((1<<AW88261_AMPPD_BITS_LEN)-1) << AW88261_AMPPD_START_BIT))

#define AW88261_UVLS_START_BIT		(14)
#define AW88261_UVLS_NORMAL		(0)
#define AW88261_UVLS_NORMAL_VALUE	\
	(AW88261_UVLS_NORMAL << AW88261_UVLS_START_BIT)

#define AW88261_BSTOCS_START_BIT	(11)
#define AW88261_BSTOCS_OVER_CURRENT	(1)
#define AW88261_BSTOCS_OVER_CURRENT_VALUE	\
	(AW88261_BSTOCS_OVER_CURRENT << AW88261_BSTOCS_START_BIT)

#define AW88261_BSTS_START_BIT		(9)
#define AW88261_BSTS_FINISHED		(1)
#define AW88261_BSTS_FINISHED_VALUE	\
	(AW88261_BSTS_FINISHED << AW88261_BSTS_START_BIT)

#define AW88261_SWS_START_BIT		(8)
#define AW88261_SWS_SWITCHING		(1)
#define AW88261_SWS_SWITCHING_VALUE	\
	(AW88261_SWS_SWITCHING << AW88261_SWS_START_BIT)

#define AW88261_NOCLKS_START_BIT	(5)
#define AW88261_NOCLKS_NO_CLOCK	(1)
#define AW88261_NOCLKS_NO_CLOCK_VALUE	\
	(AW88261_NOCLKS_NO_CLOCK << AW88261_NOCLKS_START_BIT)

#define AW88261_CLKS_START_BIT		(4)
#define AW88261_CLKS_STABLE		(1)
#define AW88261_CLKS_STABLE_VALUE	\
	(AW88261_CLKS_STABLE << AW88261_CLKS_START_BIT)

#define AW88261_OCDS_START_BIT		(3)
#define AW88261_OCDS_OC		(1)
#define AW88261_OCDS_OC_VALUE		\
	(AW88261_OCDS_OC << AW88261_OCDS_START_BIT)

#define AW88261_OTHS_START_BIT		(1)
#define AW88261_OTHS_OT		(1)
#define AW88261_OTHS_OT_VALUE		\
	(AW88261_OTHS_OT << AW88261_OTHS_START_BIT)

#define AW88261_PLLS_START_BIT		(0)
#define AW88261_PLLS_LOCKED		(1)
#define AW88261_PLLS_LOCKED_VALUE	\
	(AW88261_PLLS_LOCKED << AW88261_PLLS_START_BIT)

#define AW88261_BIT_PLL_CHECK \
		(AW88261_CLKS_STABLE_VALUE | \
		AW88261_PLLS_LOCKED_VALUE)

#define AW88261_BIT_SYSST_CHECK_MASK \
		(~(AW88261_UVLS_NORMAL_VALUE | \
		AW88261_BSTOCS_OVER_CURRENT_VALUE | \
		AW88261_BSTS_FINISHED_VALUE | \
		AW88261_SWS_SWITCHING_VALUE | \
		AW88261_NOCLKS_NO_CLOCK_VALUE | \
		AW88261_CLKS_STABLE_VALUE | \
		AW88261_OCDS_OC_VALUE | \
		AW88261_OTHS_OT_VALUE | \
		AW88261_PLLS_LOCKED_VALUE))

#define AW88261_BIT_SYSST_CHECK \
		(AW88261_BSTS_FINISHED_VALUE | \
		AW88261_SWS_SWITCHING_VALUE | \
		AW88261_CLKS_STABLE_VALUE | \
		AW88261_PLLS_LOCKED_VALUE)

#define AW88261_ULS_HMUTE_START_BIT	(14)
#define AW88261_ULS_HMUTE_BITS_LEN	(1)
#define AW88261_ULS_HMUTE_MASK		\
	(~(((1<<AW88261_ULS_HMUTE_BITS_LEN)-1) << AW88261_ULS_HMUTE_START_BIT))

#define AW88261_ULS_HMUTE_DISABLE	(0)
#define AW88261_ULS_HMUTE_DISABLE_VALUE	\
	(AW88261_ULS_HMUTE_DISABLE << AW88261_ULS_HMUTE_START_BIT)

#define AW88261_ULS_HMUTE_ENABLE	(1)
#define AW88261_ULS_HMUTE_ENABLE_VALUE	\
	(AW88261_ULS_HMUTE_ENABLE << AW88261_ULS_HMUTE_START_BIT)

#define AW88261_HMUTE_START_BIT	(8)
#define AW88261_HMUTE_BITS_LEN		(1)
#define AW88261_HMUTE_MASK		\
	(~(((1<<AW88261_HMUTE_BITS_LEN)-1) << AW88261_HMUTE_START_BIT))

#define AW88261_HMUTE_DISABLE		(0)
#define AW88261_HMUTE_DISABLE_VALUE	\
	(AW88261_HMUTE_DISABLE << AW88261_HMUTE_START_BIT)

#define AW88261_HMUTE_ENABLE		(1)
#define AW88261_HMUTE_ENABLE_VALUE	\
	(AW88261_HMUTE_ENABLE << AW88261_HMUTE_START_BIT)

#define AW88261_AMPPD_START_BIT	(1)
#define AW88261_AMPPD_BITS_LEN		(1)
#define AW88261_AMPPD_MASK		\
	(~(((1<<AW88261_AMPPD_BITS_LEN)-1) << AW88261_AMPPD_START_BIT))

#define AW88261_AMPPD_WORKING		(0)
#define AW88261_AMPPD_WORKING_VALUE	\
	(AW88261_AMPPD_WORKING << AW88261_AMPPD_START_BIT)

#define AW88261_AMPPD_POWER_DOWN	(1)
#define AW88261_AMPPD_POWER_DOWN_VALUE	\
	(AW88261_AMPPD_POWER_DOWN << AW88261_AMPPD_START_BIT)

#define AW88261_PWDN_START_BIT		(0)
#define AW88261_PWDN_BITS_LEN		(1)
#define AW88261_PWDN_MASK		\
	(~(((1<<AW88261_PWDN_BITS_LEN)-1) << AW88261_PWDN_START_BIT))

#define AW88261_PWDN_WORKING		(0)
#define AW88261_PWDN_WORKING_VALUE	\
	(AW88261_PWDN_WORKING << AW88261_PWDN_START_BIT)

#define AW88261_PWDN_POWER_DOWN	(1)
#define AW88261_PWDN_POWER_DOWN_VALUE	\
	(AW88261_PWDN_POWER_DOWN << AW88261_PWDN_START_BIT)

#define AW88261_MUTE_VOL		(90 * 8)
#define AW88261_VOLUME_STEP_DB		(6 * 8)

#define AW88261_VOL_6DB_START		(6)

#define AW88261_VOL_START_BIT		(0)
#define AW88261_VOL_BITS_LEN		(10)
#define AW88261_VOL_MASK		\
	(~(((1<<AW88261_VOL_BITS_LEN)-1) << AW88261_VOL_START_BIT))

#define AW88261_VOL_DEFAULT_VALUE	(0)

#define AW88261_I2STXEN_START_BIT	(6)
#define AW88261_I2STXEN_BITS_LEN	(1)
#define AW88261_I2STXEN_MASK		\
	(~(((1<<AW88261_I2STXEN_BITS_LEN)-1) << AW88261_I2STXEN_START_BIT))

#define AW88261_I2STXEN_DISABLE	(0)
#define AW88261_I2STXEN_DISABLE_VALUE	\
	(AW88261_I2STXEN_DISABLE << AW88261_I2STXEN_START_BIT)

#define AW88261_I2STXEN_ENABLE		(1)
#define AW88261_I2STXEN_ENABLE_VALUE	\
	(AW88261_I2STXEN_ENABLE << AW88261_I2STXEN_START_BIT)

#define AW88261_CCO_MUX_START_BIT	(14)
#define AW88261_CCO_MUX_BITS_LEN	(1)
#define AW88261_CCO_MUX_MASK		\
	(~(((1<<AW88261_CCO_MUX_BITS_LEN)-1) << AW88261_CCO_MUX_START_BIT))

#define AW88261_CCO_MUX_DIVIDED	(0)
#define AW88261_CCO_MUX_DIVIDED_VALUE	\
	(AW88261_CCO_MUX_DIVIDED << AW88261_CCO_MUX_START_BIT)

#define AW88261_CCO_MUX_BYPASS		(1)
#define AW88261_CCO_MUX_BYPASS_VALUE	\
	(AW88261_CCO_MUX_BYPASS << AW88261_CCO_MUX_START_BIT)

#define AW88261_EF_VSN_GESLP_H_START_BIT	(0)
#define AW88261_EF_VSN_GESLP_H_BITS_LEN	(10)
#define AW88261_EF_VSN_GESLP_H_MASK		\
	(~(((1<<AW88261_EF_VSN_GESLP_H_BITS_LEN)-1) << AW88261_EF_VSN_GESLP_H_START_BIT))

#define AW88261_EF_VSN_GESLP_L_START_BIT	(0)
#define AW88261_EF_VSN_GESLP_L_BITS_LEN	(10)
#define AW88261_EF_VSN_GESLP_L_MASK		\
	(~(((1<<AW88261_EF_VSN_GESLP_L_BITS_LEN)-1) << AW88261_EF_VSN_GESLP_L_START_BIT))

#define AW88261_FORCE_PWM_START_BIT		(12)
#define AW88261_FORCE_PWM_BITS_LEN		(1)
#define AW88261_FORCE_PWM_MASK			\
	(~(((1<<AW88261_FORCE_PWM_BITS_LEN)-1) << AW88261_FORCE_PWM_START_BIT))

#define AW88261_FORCE_PWM_FORCEMINUS_PWM	(1)
#define AW88261_FORCE_PWM_FORCEMINUS_PWM_VALUE	\
	(AW88261_FORCE_PWM_FORCEMINUS_PWM << AW88261_FORCE_PWM_START_BIT)

#define AW88261_BST_OS_WIDTH_START_BIT		(0)
#define AW88261_BST_OS_WIDTH_BITS_LEN		(3)
#define AW88261_BST_OS_WIDTH_MASK		\
	(~(((1<<AW88261_BST_OS_WIDTH_BITS_LEN)-1) << AW88261_BST_OS_WIDTH_START_BIT))

#define AW88261_BST_OS_WIDTH_50NS		(4)
#define AW88261_BST_OS_WIDTH_50NS_VALUE	\
	(AW88261_BST_OS_WIDTH_50NS << AW88261_BST_OS_WIDTH_START_BIT)

/* BST_LOOPR bit 1:0 (BSTCTRL6 0x65) */
#define AW88261_BST_LOOPR_START_BIT	(0)
#define AW88261_BST_LOOPR_BITS_LEN	(2)
#define AW88261_BST_LOOPR_MASK		\
	(~(((1<<AW88261_BST_LOOPR_BITS_LEN)-1) << AW88261_BST_LOOPR_START_BIT))

#define AW88261_BST_LOOPR_340K		(2)
#define AW88261_BST_LOOPR_340K_VALUE	\
	(AW88261_BST_LOOPR_340K << AW88261_BST_LOOPR_START_BIT)

/* RSQN_DLY bit 15:14 (BSTCTRL7 0x66) */
#define AW88261_RSQN_DLY_START_BIT	(14)
#define AW88261_RSQN_DLY_BITS_LEN	(2)
#define AW88261_RSQN_DLY_MASK		\
	(~(((1<<AW88261_RSQN_DLY_BITS_LEN)-1) << AW88261_RSQN_DLY_START_BIT))

#define AW88261_RSQN_DLY_35NS		(2)
#define AW88261_RSQN_DLY_35NS_VALUE	\
	(AW88261_RSQN_DLY_35NS << AW88261_RSQN_DLY_START_BIT)

/* BURST_SSMODE bit 3 (BSTCTRL8 0x67) */
#define AW88261_BURST_SSMODE_START_BIT	(3)
#define AW88261_BURST_SSMODE_BITS_LEN	(1)
#define AW88261_BURST_SSMODE_MASK	\
	(~(((1<<AW88261_BURST_SSMODE_BITS_LEN)-1) << AW88261_BURST_SSMODE_START_BIT))

#define AW88261_BURST_SSMODE_FAST	(0)
#define AW88261_BURST_SSMODE_FAST_VALUE	\
	(AW88261_BURST_SSMODE_FAST << AW88261_BURST_SSMODE_START_BIT)

/* BST_BURST bit 9:7 (BSTCTRL9 0x68) */
#define AW88261_BST_BURST_START_BIT	(7)
#define AW88261_BST_BURST_BITS_LEN	(3)
#define AW88261_BST_BURST_MASK		\
	(~(((1<<AW88261_BST_BURST_BITS_LEN)-1) << AW88261_BST_BURST_START_BIT))

#define AW88261_BST_BURST_30MA		(2)
#define AW88261_BST_BURST_30MA_VALUE	\
	(AW88261_BST_BURST_30MA << AW88261_BST_BURST_START_BIT)

#define AW88261_EF_VSN_GESLP_SIGN_MASK		(~0x0200)
#define AW88261_EF_VSN_GESLP_NEG		(~0xfc00)

#define AW88261_EF_ISN_GESLP_SIGN_MASK		(~0x0200)
#define AW88261_EF_ISN_GESLP_NEG		(~0xfc00)

#define AW88261_EF_ISN_GESLP_H_START_BIT	(0)
#define AW88261_EF_ISN_GESLP_H_BITS_LEN	(10)
#define AW88261_EF_ISN_GESLP_H_MASK		\
	(~(((1<<AW88261_EF_ISN_GESLP_H_BITS_LEN)-1) << AW88261_EF_ISN_GESLP_H_START_BIT))

#define AW88261_EF_ISN_GESLP_L_START_BIT	(0)
#define AW88261_EF_ISN_GESLP_L_BITS_LEN	(10)
#define AW88261_EF_ISN_GESLP_L_MASK		\
	(~(((1<<AW88261_EF_ISN_GESLP_L_BITS_LEN)-1) << AW88261_EF_ISN_GESLP_L_START_BIT))

#define AW88261_CABL_BASE_VALUE		(1000)
#define AW88261_ICABLK_FACTOR		(1)
#define AW88261_VCABLK_FACTOR		(1)

#define AW88261_VCAL_FACTOR		(1<<13)

#define AW88261_START_RETRIES		(5)
#define AW88261_START_WORK_DELAY_MS	(0)

#define AW88261_I2C_NAME		"aw88261"

#define AW88261_RATES (SNDRV_PCM_RATE_8000_48000 | \
			SNDRV_PCM_RATE_96000)
#define AW88261_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | \
			SNDRV_PCM_FMTBIT_S24_LE | \
			SNDRV_PCM_FMTBIT_S32_LE)

#define FADE_TIME_MAX			100000
#define FADE_TIME_MIN			0

#define AW88261_DEV_DEFAULT_CH		(0)
#define AW88261_ACF_FILE		"/*(DEBLOBBED)*/"
#define AW88261_DEV_SYSST_CHECK_MAX	(10)
#define AW88261_SOFT_RESET_VALUE	(0x55aa)
#define AW88261_REG_TO_DB		(0x3f)
#define AW88261_VOL_START_MASK		(0xfc00)
#define AW88261_INIT_PROFILE		(0)

#define REG_VAL_TO_DB(value)		((((value) >> AW88261_VOL_6DB_START) * \
					AW88261_VOLUME_STEP_DB) + \
					((value) & AW88261_REG_TO_DB))
#define DB_TO_REG_VAL(value)		((((value) / AW88261_VOLUME_STEP_DB) << \
					AW88261_VOL_6DB_START) + \
					((value) % AW88261_VOLUME_STEP_DB))

#define AW88261_PROFILE_EXT(xname, profile_info, profile_get, profile_set) \
{ \
	.iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
	.name = xname, \
	.info = profile_info, \
	.get = profile_get, \
	.put = profile_set, \
}

enum {
	AW88261_SYNC_START = 0,
	AW88261_ASYNC_START,
};

enum aw88261_id {
	AW88261_CHIP_ID = 0x2113,
};

enum {
	AW88261_500_US = 500,
	AW88261_1000_US = 1000,
	AW88261_2000_US = 2000,
};

enum {
	AW88261_DEV_PW_OFF = 0,
	AW88261_DEV_PW_ON,
};

enum {
	AW88261_DEV_FW_FAILED = 0,
	AW88261_DEV_FW_OK,
};

enum {
	AW88261_EF_AND_CHECK = 0,
	AW88261_EF_OR_CHECK,
};

enum {
	AW88261_FRCSET_DISABLE = 0,
	AW88261_FRCSET_ENABLE,
};

struct aw88261 {
	struct aw_device *aw_pa;
	struct mutex lock;
	struct gpio_desc *reset_gpio;
	struct delayed_work start_work;
	struct regmap *regmap;
	struct aw_container *aw_cfg;

	int efuse_check;
	int frcset_en;
	unsigned int mute_st;
	unsigned int amppd_st;

	bool phase_sync;
};

#endif

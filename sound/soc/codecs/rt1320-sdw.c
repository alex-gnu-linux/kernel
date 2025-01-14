// SPDX-License-Identifier: GPL-2.0-only
//
// rt1320-sdw.c -- rt1320 SDCA ALSA SoC amplifier audio driver
//
// Copyright(c) 2024 Realtek Semiconductor Corp.
//
//
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/pm_runtime.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/dmi.h>
#include <linux/firmware.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/tlv.h>
#include <sound/sdw.h>
#include "rt1320-sdw.h"

/*
 * The 'blind writes' is an SDCA term to deal with platform-specific initialization.
 * It might include vendor-specific or SDCA control registers.
 */
static const struct reg_sequence rt1320_blind_write[] = {
	{ 0xc003, 0xe0 },
	{ 0xc01b, 0xfc },
	{ 0xc5c3, 0xf2 },
	{ 0xc5c2, 0x00 },
	{ 0xc5c6, 0x10 },
	{ 0xc5c4, 0x12 },
	{ 0xc5c8, 0x03 },
	{ 0xc5d8, 0x0a },
	{ 0xc5f7, 0x22 },
	{ 0xc5f6, 0x22 },
	{ 0xc5d0, 0x0f },
	{ 0xc5d1, 0x89 },
	{ 0xc057, 0x51 },
	{ 0xc054, 0x35 },
	{ 0xc053, 0x55 },
	{ 0xc052, 0x55 },
	{ 0xc051, 0x13 },
	{ 0xc050, 0x15 },
	{ 0xc060, 0x77 },
	{ 0xc061, 0x55 },
	{ 0xc063, 0x55 },
	{ 0xc065, 0xa5 },
	{ 0xc06b, 0x0a },
	{ 0xca05, 0xd6 },
	{ 0xca25, 0xd6 },
	{ 0xcd00, 0x05 },
	{ 0xc604, 0x40 },
	{ 0xc609, 0x40 },
	{ 0xc046, 0xff },
	{ 0xc045, 0xff },
	{ 0xc044, 0xff },
	{ 0xc043, 0xff },
	{ 0xc042, 0xff },
	{ 0xc041, 0xff },
	{ 0xc040, 0xff },
	{ 0xcc10, 0x01 },
	{ 0xc700, 0xf0 },
	{ 0xc701, 0x13 },
	{ 0xc901, 0x04 },
	{ 0xc900, 0x73 },
	{ 0xde03, 0x05 },
	{ 0xdd0b, 0x0d },
	{ 0xdd0a, 0xff },
	{ 0xdd09, 0x0d },
	{ 0xdd08, 0xff },
	{ 0xc570, 0x08 },
	{ 0xe803, 0xbe },
	{ 0xc003, 0xc0 },
	{ 0xc081, 0xfe },
	{ 0xce31, 0x0d },
	{ 0xce30, 0xae },
	{ 0xce37, 0x0b },
	{ 0xce36, 0xd2 },
	{ 0xce39, 0x04 },
	{ 0xce38, 0x80 },
	{ 0xce3f, 0x00 },
	{ 0xce3e, 0x00 },
	{ 0xd470, 0x8b },
	{ 0xd471, 0x18 },
	{ 0xc019, 0x10 },
	{ 0xd487, 0x3f },
	{ 0xd486, 0xc3 },
};

static const struct reg_sequence rt1320_vc_blind_write[] = {
	{ 0xc003, 0xe0 },
	{ 0xe80a, 0x01 },
	{ 0xc5c3, 0xf3 },
	{ 0xc057, 0x51 },
	{ 0xc054, 0x35 },
	{ 0xca05, 0xd6 },
	{ 0xca07, 0x07 },
	{ 0xca25, 0xd6 },
	{ 0xca27, 0x07 },
	{ 0xc604, 0x40 },
	{ 0xc609, 0x40 },
	{ 0xc046, 0xff },
	{ 0xc045, 0xff },
	{ 0xda81, 0x14 },
	{ 0xda8d, 0x14 },
	{ 0xc044, 0xff },
	{ 0xc043, 0xff },
	{ 0xc042, 0xff },
	{ 0xc041, 0x7f },
	{ 0xc040, 0xff },
	{ 0xcc10, 0x01 },
	{ 0xc700, 0xf0 },
	{ 0xc701, 0x13 },
	{ 0xc901, 0x09 },
	{ 0xc900, 0xd0 },
	{ 0xde03, 0x05 },
	{ 0xdd0b, 0x0d },
	{ 0xdd0a, 0xff },
	{ 0xdd09, 0x0d },
	{ 0xdd08, 0xff },
	{ 0xc570, 0x08 },
	{ 0xc086, 0x02 },
	{ 0xc085, 0x7f },
	{ 0xc084, 0x00 },
	{ 0xc081, 0xfe },
	{ 0xf084, 0x0f },
	{ 0xf083, 0xff },
	{ 0xf082, 0xff },
	{ 0xf081, 0xff },
	{ 0xf080, 0xff },
	{ 0xe802, 0xf8 },
	{ 0xe803, 0xbe },
	{ 0xc003, 0xc0 },
	{ 0xd470, 0xec },
	{ 0xd471, 0x3a },
	{ 0xd474, 0x11 },
	{ 0xd475, 0x32 },
	{ 0xd478, 0x64 },
	{ 0xd479, 0x20 },
	{ 0xd47a, 0x10 },
	{ 0xd47c, 0xff },
	{ 0xc019, 0x10 },
	{ 0xd487, 0x0b },
	{ 0xd487, 0x3b },
	{ 0xd486, 0xc3 },
	{ 0xc598, 0x04 },
	{ 0xd500, 0x00 },
	{ 0xd500, 0x17 },
	{ 0xd600, 0x01 },
	{ 0xd601, 0x02 },
	{ 0xd602, 0x03 },
	{ 0xd603, 0x04 },
	{ 0xd64c, 0x03 },
	{ 0xd64d, 0x03 },
	{ 0xd64e, 0x03 },
	{ 0xd64f, 0x03 },
	{ 0xd650, 0x03 },
	{ 0xd651, 0x03 },
	{ 0xd652, 0x03 },
	{ 0xd610, 0x01 },
	{ 0xd608, 0x03 },
	{ 0xd609, 0x00 },
};

static const struct reg_sequence rt1320_vc_patch_code_write[] = {
	/*(DEBLOBBED)*/
};

/*
 * The 'patch code' is written to the patch code area.
 * The patch code area is used for SDCA register expansion flexibility.
 */
static const struct reg_sequence rt1320_patch_code_write[] = {
	{ /*(DEBLOBBED)*/ },
};

static const struct reg_default rt1320_reg_defaults[] = {
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_MUTE, CH_01), 0x01 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_MUTE, CH_02), 0x01 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE27, RT1320_SDCA_CTL_REQ_POWER_STATE, 0), 0x03 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23, RT1320_SDCA_CTL_REQ_POWER_STATE, 0), 0x03 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PPU21, RT1320_SDCA_CTL_POSTURE_NUMBER, 0), 0x00 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS113, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0), 0x09 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS14, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0), 0x0b },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS21, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0), 0x09 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23, RT1320_SDCA_CTL_ACTUAL_POWER_STATE, 0), 0x03 },
};

static const struct reg_default rt1320_mbq_defaults[] = {
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_VOLUME, CH_01), 0x0000 },
	{ SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_VOLUME, CH_02), 0x0000 },
};

static bool rt1320_readable_register(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case 0xc000 ... 0xc086:
	case 0xc400 ... 0xc409:
	case 0xc480 ... 0xc48f:
	case 0xc4c0 ... 0xc4c4:
	case 0xc4e0 ... 0xc4e7:
	case 0xc500:
	case 0xc560 ... 0xc56b:
	case 0xc570:
	case 0xc580 ... 0xc59a:
	case 0xc5b0 ... 0xc60f:
	case 0xc640 ... 0xc64f:
	case 0xc670:
	case 0xc680 ... 0xc683:
	case 0xc700 ... 0xc76f:
	case 0xc800 ... 0xc801:
	case 0xc820:
	case 0xc900 ... 0xc901:
	case 0xc920 ... 0xc921:
	case 0xca00 ... 0xca07:
	case 0xca20 ... 0xca27:
	case 0xca40 ... 0xca4b:
	case 0xca60 ... 0xca68:
	case 0xca80 ... 0xca88:
	case 0xcb00 ... 0xcb0c:
	case 0xcc00 ... 0xcc12:
	case 0xcc80 ... 0xcc81:
	case 0xcd00:
	case 0xcd80 ... 0xcd82:
	case 0xce00 ... 0xce4d:
	case 0xcf00 ... 0xcf25:
	case 0xd000 ... 0xd0ff:
	case 0xd100 ... 0xd1ff:
	case 0xd200 ... 0xd2ff:
	case 0xd300 ... 0xd3ff:
	case 0xd400 ... 0xd403:
	case 0xd410 ... 0xd417:
	case 0xd470 ... 0xd497:
	case 0xd4dc ... 0xd50f:
	case 0xd520 ... 0xd543:
	case 0xd560 ... 0xd5ef:
	case 0xd600 ... 0xd663:
	case 0xda00 ... 0xda6e:
	case 0xda80 ... 0xda9e:
	case 0xdb00 ... 0xdb7f:
	case 0xdc00:
	case 0xdc20 ... 0xdc21:
	case 0xdd00 ... 0xdd17:
	case 0xde00 ... 0xde09:
	case 0xdf00 ... 0xdf1b:
	case 0xe000 ... 0xe847:
	case 0xf01e:
	case 0xf717 ... 0xf719:
	case 0xf720 ... 0xf723:
	case 0x1000cd91 ... 0x1000cd96:
	case 0x1000f008:
	case 0x1000f021:
	case 0x3fe2e000 ... 0x3fe2e003:
	case 0x3fc2ab80 ... 0x3fc2abd4:
	/* 0x41000189/0x4100018a */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_MUTE, CH_01):
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_MUTE, CH_02):
	/* 0x41001388 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE27, RT1320_SDCA_CTL_REQ_POWER_STATE, 0):
	/* 0x41001988 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23, RT1320_SDCA_CTL_REQ_POWER_STATE, 0):
	/* 0x41080000 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT0, RT1320_SDCA_CTL_FUNC_STATUS, 0):
	/* 0x41080200 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PPU21, RT1320_SDCA_CTL_POSTURE_NUMBER, 0):
	/* 0x41080900 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS113, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0):
	/* 0x41080980 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS14, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0):
	/* 0x41081080 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS21, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0):
	/* 0x41081480/0x41081488 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_SAPU, RT1320_SDCA_CTL_SAPU_PROTECTION_MODE, 0):
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_SAPU, RT1320_SDCA_CTL_SAPU_PROTECTION_STATUS, 0):
	/* 0x41081980 */
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23, RT1320_SDCA_CTL_ACTUAL_POWER_STATE, 0):
		return true;
	default:
		return false;
	}
}

static bool rt1320_volatile_register(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case 0xc000:
	case 0xc003:
	case 0xc081:
	case 0xc402 ... 0xc406:
	case 0xc48c ... 0xc48f:
	case 0xc560:
	case 0xc5b5 ... 0xc5b7:
	case 0xc5fc ... 0xc5ff:
	case 0xc820:
	case 0xc900:
	case 0xc920:
	case 0xca42:
	case 0xca62:
	case 0xca82:
	case 0xcd00:
	case 0xce03:
	case 0xce10:
	case 0xce14 ... 0xce17:
	case 0xce44 ... 0xce49:
	case 0xce4c ... 0xce4d:
	case 0xcf0c:
	case 0xcf10 ... 0xcf25:
	case 0xd486 ... 0xd487:
	case 0xd4e5 ... 0xd4e6:
	case 0xd4e8 ... 0xd4ff:
	case 0xd530:
	case 0xd540:
	case 0xd543:
	case 0xdb58 ... 0xdb5f:
	case 0xdb60 ... 0xdb63:
	case 0xdb68 ... 0xdb69:
	case 0xdb6d:
	case 0xdb70 ... 0xdb71:
	case 0xdb76:
	case 0xdb7a:
	case 0xdb7c ... 0xdb7f:
	case 0xdd0c ... 0xdd13:
	case 0xde02:
	case 0xdf14 ... 0xdf1b:
	case 0xe83c ... 0xe847:
	case 0xf01e:
	case 0xf717 ... 0xf719:
	case 0xf720 ... 0xf723:
	case 0x10000000 ... 0x10007fff:
	case 0x1000c000 ... 0x1000dfff:
	case 0x1000f008:
	case 0x1000f021:
	case 0x3fc2ab80 ... 0x3fc2abd4:
	case 0x3fc2bf80 ... 0x3fc2bf83:
	case 0x3fc2bfc0 ... 0x3fc2bfc7:
	case 0x3fe2e000 ... 0x3fe2e003:
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT0, RT1320_SDCA_CTL_FUNC_STATUS, 0):
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_SAPU, RT1320_SDCA_CTL_SAPU_PROTECTION_MODE, 0):
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_SAPU, RT1320_SDCA_CTL_SAPU_PROTECTION_STATUS, 0):
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23, RT1320_SDCA_CTL_ACTUAL_POWER_STATE, 0):
		return true;
	default:
		return false;
	}
}

static bool rt1320_mbq_readable_register(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_VOLUME, CH_01):
	case SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_VOLUME, CH_02):
		return true;
	default:
		return false;
	}
}

static const struct regmap_config rt1320_sdw_regmap = {
	.reg_bits = 32,
	.val_bits = 8,
	.readable_reg = rt1320_readable_register,
	.volatile_reg = rt1320_volatile_register,
	.max_register = 0x41081980,
	.reg_defaults = rt1320_reg_defaults,
	.num_reg_defaults = ARRAY_SIZE(rt1320_reg_defaults),
	.cache_type = REGCACHE_MAPLE,
	.use_single_read = true,
	.use_single_write = true,
};

static const struct regmap_config rt1320_mbq_regmap = {
	.name = "sdw-mbq",
	.reg_bits = 32,
	.val_bits = 16,
	.readable_reg = rt1320_mbq_readable_register,
	.max_register = 0x41000192,
	.reg_defaults = rt1320_mbq_defaults,
	.num_reg_defaults = ARRAY_SIZE(rt1320_mbq_defaults),
	.cache_type = REGCACHE_MAPLE,
	.use_single_read = true,
	.use_single_write = true,
};

static int rt1320_read_prop(struct sdw_slave *slave)
{
	struct sdw_slave_prop *prop = &slave->prop;
	int nval;
	int i, j;
	u32 bit;
	unsigned long addr;
	struct sdw_dpn_prop *dpn;

	/*
	 * Due to support the multi-lane, we call 'sdw_slave_read_prop' to get the lane mapping
	 */
	sdw_slave_read_prop(slave);

	prop->scp_int1_mask = SDW_SCP_INT1_BUS_CLASH | SDW_SCP_INT1_PARITY;
	prop->quirks = SDW_SLAVE_QUIRKS_INVALID_INITIAL_PARITY;

	prop->paging_support = true;
	prop->lane_control_support = true;

	/* first we need to allocate memory for set bits in port lists */
	prop->source_ports = BIT(4);
	prop->sink_ports = BIT(1);

	nval = hweight32(prop->source_ports);
	prop->src_dpn_prop = devm_kcalloc(&slave->dev, nval,
		sizeof(*prop->src_dpn_prop), GFP_KERNEL);
	if (!prop->src_dpn_prop)
		return -ENOMEM;

	i = 0;
	dpn = prop->src_dpn_prop;
	addr = prop->source_ports;
	for_each_set_bit(bit, &addr, 32) {
		dpn[i].num = bit;
		dpn[i].type = SDW_DPN_FULL;
		dpn[i].simple_ch_prep_sm = true;
		dpn[i].ch_prep_timeout = 10;
		i++;
	}

	/* do this again for sink now */
	nval = hweight32(prop->sink_ports);
	prop->sink_dpn_prop = devm_kcalloc(&slave->dev, nval,
		sizeof(*prop->sink_dpn_prop), GFP_KERNEL);
	if (!prop->sink_dpn_prop)
		return -ENOMEM;

	j = 0;
	dpn = prop->sink_dpn_prop;
	addr = prop->sink_ports;
	for_each_set_bit(bit, &addr, 32) {
		dpn[j].num = bit;
		dpn[j].type = SDW_DPN_FULL;
		dpn[j].simple_ch_prep_sm = true;
		dpn[j].ch_prep_timeout = 10;
		j++;
	}

	/* set the timeout values */
	prop->clk_stop_timeout = 64;

	return 0;
}

static int rt1320_pde_transition_delay(struct rt1320_sdw_priv *rt1320, unsigned char ps)
{
	unsigned int delay = 1000, val;

	pm_runtime_mark_last_busy(&rt1320->sdw_slave->dev);

	/* waiting for Actual PDE becomes to PS0/PS3 */
	while (delay) {
		regmap_read(rt1320->regmap,
			SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23,
			RT1320_SDCA_CTL_ACTUAL_POWER_STATE, 0), &val);
		if (val == ps)
			break;

		usleep_range(1000, 1500);
		delay--;
	}
	if (!delay) {
		dev_warn(&rt1320->sdw_slave->dev, "%s PDE to %s is NOT ready", __func__, ps?"PS3":"PS0");
		return -ETIMEDOUT;
	}

	return 0;
}

static void rt1320_vc_preset(struct rt1320_sdw_priv *rt1320)
{
	struct sdw_slave *slave = rt1320->sdw_slave;
	unsigned int i, reg, val, delay, retry, tmp;

	regmap_multi_reg_write(rt1320->regmap, rt1320_vc_blind_write, ARRAY_SIZE(rt1320_vc_blind_write));

	for (i = 0; i < ARRAY_SIZE(rt1320_vc_patch_code_write); i++) {
		reg = rt1320_vc_patch_code_write[i].reg;
		val = rt1320_vc_patch_code_write[i].def;
		delay = rt1320_vc_patch_code_write[i].delay_us;

		if ((reg == SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23, RT1320_SDCA_CTL_REQ_POWER_STATE, 0)) &&
			(val == 0x00)) {
			retry = 200;
			while (retry) {
				regmap_read(rt1320->regmap, RT1320_KR0_INT_READY, &tmp);
				dev_dbg(&slave->dev, "%s, RT1320_KR0_INT_READY=0x%x\n", __func__, tmp);
				if (tmp == 0x1f)
					break;
				usleep_range(1000, 1500);
				retry--;
			}
			if (!retry)
				dev_warn(&slave->dev, "%s MCU is NOT ready!", __func__);
		}
		regmap_write(rt1320->regmap, reg, val);
		if (delay)
			usleep_range(delay, delay + 1000);
	}
}

static int rt1320_io_init(struct device *dev, struct sdw_slave *slave)
{
	struct rt1320_sdw_priv *rt1320 = dev_get_drvdata(dev);
	unsigned int amp_func_status, val, tmp;

	if (rt1320->hw_init)
		return 0;

	regcache_cache_only(rt1320->regmap, false);
	regcache_cache_only(rt1320->mbq_regmap, false);
	if (rt1320->first_hw_init) {
		regcache_cache_bypass(rt1320->regmap, true);
		regcache_cache_bypass(rt1320->mbq_regmap, true);
	} else {
		/*
		 * PM runtime status is marked as 'active' only when a Slave reports as Attached
		 */
		/* update count of parent 'active' children */
		pm_runtime_set_active(&slave->dev);
	}

	pm_runtime_get_noresume(&slave->dev);

	if (rt1320->version_id < 0) {
		regmap_read(rt1320->regmap, RT1320_DEV_VERSION_ID_1, &val);
		rt1320->version_id = val;
	}

	regmap_read(rt1320->regmap,
		SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT0, RT1320_SDCA_CTL_FUNC_STATUS, 0), &amp_func_status);
	dev_dbg(dev, "%s amp func_status=0x%x\n", __func__, amp_func_status);

	/* initialization write */
	if ((amp_func_status & FUNCTION_NEEDS_INITIALIZATION)) {
		if (rt1320->version_id < RT1320_VC) {
			regmap_multi_reg_write(rt1320->regmap, rt1320_blind_write, ARRAY_SIZE(rt1320_blind_write));
			regmap_multi_reg_write(rt1320->regmap, rt1320_patch_code_write,
				ARRAY_SIZE(rt1320_patch_code_write));
		} else if (rt1320->version_id == RT1320_VC) {
			rt1320_vc_preset(rt1320);
		}

		regmap_write(rt1320->regmap,
			SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT0, RT1320_SDCA_CTL_FUNC_STATUS, 0),
			FUNCTION_NEEDS_INITIALIZATION);
	}
	if (!rt1320->first_hw_init && rt1320->version_id == RT1320_VA) {
		regmap_write(rt1320->regmap, SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23,
			RT1320_SDCA_CTL_REQ_POWER_STATE, 0), 0);
		regmap_read(rt1320->regmap, RT1320_HIFI_VER_0, &val);
		regmap_read(rt1320->regmap, RT1320_HIFI_VER_1, &tmp);
		val = (tmp << 8) | val;
		regmap_read(rt1320->regmap, RT1320_HIFI_VER_2, &tmp);
		val = (tmp << 16) | val;
		regmap_read(rt1320->regmap, RT1320_HIFI_VER_3, &tmp);
		val = (tmp << 24) | val;
		dev_dbg(dev, "%s ROM version=0x%x\n", __func__, val);
		/*
		 * We call the version b which has the new DSP ROM code against version a.
		 * Therefore, we read the DSP address to check the ID.
		 */
		if (val == RT1320_VER_B_ID)
			rt1320->version_id = RT1320_VB;
		regmap_write(rt1320->regmap, SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23,
			RT1320_SDCA_CTL_REQ_POWER_STATE, 0), 3);
	}
	dev_dbg(dev, "%s version_id=%d\n", __func__, rt1320->version_id);

	if (rt1320->first_hw_init) {
		regcache_cache_bypass(rt1320->regmap, false);
		regcache_cache_bypass(rt1320->mbq_regmap, false);
		regcache_mark_dirty(rt1320->regmap);
		regcache_mark_dirty(rt1320->mbq_regmap);
	}

	/* Mark Slave initialization complete */
	rt1320->first_hw_init = true;
	rt1320->hw_init = true;

	pm_runtime_mark_last_busy(&slave->dev);
	pm_runtime_put_autosuspend(&slave->dev);

	dev_dbg(&slave->dev, "%s hw_init complete\n", __func__);
	return 0;
}

static int rt1320_update_status(struct sdw_slave *slave,
					enum sdw_slave_status status)
{
	struct  rt1320_sdw_priv *rt1320 = dev_get_drvdata(&slave->dev);

	if (status == SDW_SLAVE_UNATTACHED)
		rt1320->hw_init = false;

	/*
	 * Perform initialization only if slave status is present and
	 * hw_init flag is false
	 */
	if (rt1320->hw_init || status != SDW_SLAVE_ATTACHED)
		return 0;

	/* perform I/O transfers required for Slave initialization */
	return rt1320_io_init(&slave->dev, slave);
}

static int rt1320_pde23_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component =
		snd_soc_dapm_to_component(w->dapm);
	struct rt1320_sdw_priv *rt1320 = snd_soc_component_get_drvdata(component);
	unsigned char ps0 = 0x0, ps3 = 0x3;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		regmap_write(rt1320->regmap,
			SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23,
				RT1320_SDCA_CTL_REQ_POWER_STATE, 0), ps0);
		rt1320_pde_transition_delay(rt1320, ps0);
		break;
	case SND_SOC_DAPM_PRE_PMD:
		regmap_write(rt1320->regmap,
			SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PDE23,
				RT1320_SDCA_CTL_REQ_POWER_STATE, 0), ps3);
		rt1320_pde_transition_delay(rt1320, ps3);
		break;
	default:
		break;
	}

	return 0;
}

static int rt1320_set_gain_put(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_kcontrol_chip(kcontrol);
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	struct rt1320_sdw_priv *rt1320 = snd_soc_component_get_drvdata(component);
	unsigned int gain_l_val, gain_r_val;
	unsigned int lvalue, rvalue;
	const unsigned int interval_offset = 0xc0;

	regmap_read(rt1320->mbq_regmap, mc->reg, &lvalue);
	regmap_read(rt1320->mbq_regmap, mc->rreg, &rvalue);

	/* L Channel */
	gain_l_val = ucontrol->value.integer.value[0];
	if (gain_l_val > mc->max)
		gain_l_val = mc->max;
	gain_l_val = 0 - ((mc->max - gain_l_val) * interval_offset);
	gain_l_val &= 0xffff;

	/* R Channel */
	gain_r_val = ucontrol->value.integer.value[1];
	if (gain_r_val > mc->max)
		gain_r_val = mc->max;
	gain_r_val = 0 - ((mc->max - gain_r_val) * interval_offset);
	gain_r_val &= 0xffff;

	if (lvalue == gain_l_val && rvalue == gain_r_val)
		return 0;

	/* Lch*/
	regmap_write(rt1320->mbq_regmap, mc->reg, gain_l_val);
	/* Rch */
	regmap_write(rt1320->mbq_regmap, mc->rreg, gain_r_val);

	return 1;
}

static int rt1320_set_gain_get(struct snd_kcontrol *kcontrol,
		struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_component *component = snd_kcontrol_chip(kcontrol);
	struct rt1320_sdw_priv *rt1320 = snd_soc_component_get_drvdata(component);
	struct soc_mixer_control *mc =
		(struct soc_mixer_control *)kcontrol->private_value;
	unsigned int read_l, read_r, ctl_l = 0, ctl_r = 0;
	const unsigned int interval_offset = 0xc0;

	regmap_read(rt1320->mbq_regmap, mc->reg, &read_l);
	regmap_read(rt1320->mbq_regmap, mc->rreg, &read_r);

	ctl_l = mc->max - (((0 - read_l) & 0xffff) / interval_offset);

	if (read_l != read_r)
		ctl_r = mc->max - (((0 - read_r) & 0xffff) / interval_offset);
	else
		ctl_r = ctl_l;

	ucontrol->value.integer.value[0] = ctl_l;
	ucontrol->value.integer.value[1] = ctl_r;
	return 0;
}

static const char * const rt1320_rx_data_ch_select[] = {
	"L,R",
	"R,L",
	"L,L",
	"R,R",
	"L,L+R",
	"R,L+R",
	"L+R,L",
	"L+R,R",
	"L+R,L+R",
};

static SOC_ENUM_SINGLE_DECL(rt1320_rx_data_ch_enum,
	SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_PPU21, RT1320_SDCA_CTL_POSTURE_NUMBER, 0), 0,
	rt1320_rx_data_ch_select);

static const DECLARE_TLV_DB_SCALE(out_vol_tlv, -6525, 75, 0);

static const struct snd_kcontrol_new rt1320_snd_controls[] = {
	SOC_DOUBLE_R_EXT_TLV("FU21 Playback Volume",
		SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_VOLUME, CH_01),
		SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_VOLUME, CH_02),
		0, 0x57, 0, rt1320_set_gain_get, rt1320_set_gain_put, out_vol_tlv),
	SOC_ENUM("RX Channel Select", rt1320_rx_data_ch_enum),
};

static const struct snd_kcontrol_new rt1320_spk_l_dac =
	SOC_DAPM_SINGLE_AUTODISABLE("Switch",
		SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_MUTE, CH_01),
		0, 1, 1);
static const struct snd_kcontrol_new rt1320_spk_r_dac =
	SOC_DAPM_SINGLE_AUTODISABLE("Switch",
		SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_FU21, RT1320_SDCA_CTL_FU_MUTE, CH_02),
		0, 1, 1);

static const struct snd_soc_dapm_widget rt1320_dapm_widgets[] = {
	/* Audio Interface */
	SND_SOC_DAPM_AIF_IN("DP1RX", "DP1 Playback", 0, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_AIF_OUT("DP4TX", "DP4 Capture", 0, SND_SOC_NOPM, 0, 0),

	/* Digital Interface */
	SND_SOC_DAPM_PGA("FU21", SND_SOC_NOPM, 0, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("PDE 23", SND_SOC_NOPM, 0, 0,
		rt1320_pde23_event,
		SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),

	/* Output */
	SND_SOC_DAPM_SWITCH("OT23 L", SND_SOC_NOPM, 0, 0, &rt1320_spk_l_dac),
	SND_SOC_DAPM_SWITCH("OT23 R", SND_SOC_NOPM, 0, 0, &rt1320_spk_r_dac),
	SND_SOC_DAPM_OUTPUT("SPOL"),
	SND_SOC_DAPM_OUTPUT("SPOR"),

	/* Input */
	SND_SOC_DAPM_PGA("AEC Data", SND_SOC_NOPM, 0, 0, NULL, 0),
	SND_SOC_DAPM_SIGGEN("AEC Gen"),
};

static const struct snd_soc_dapm_route rt1320_dapm_routes[] = {
	{ "FU21", NULL, "DP1RX" },
	{ "FU21", NULL, "PDE 23" },
	{ "OT23 L", "Switch", "FU21" },
	{ "OT23 R", "Switch", "FU21" },
	{ "SPOL", NULL, "OT23 L" },
	{ "SPOR", NULL, "OT23 R" },

	{ "AEC Data", NULL, "AEC Gen" },
	{ "DP4TX", NULL, "AEC Data" },
};

static int rt1320_set_sdw_stream(struct snd_soc_dai *dai, void *sdw_stream,
				int direction)
{
	snd_soc_dai_dma_data_set(dai, direction, sdw_stream);
	return 0;
}

static void rt1320_sdw_shutdown(struct snd_pcm_substream *substream,
				struct snd_soc_dai *dai)
{
	snd_soc_dai_set_dma_data(dai, substream, NULL);
}

static int rt1320_sdw_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params, struct snd_soc_dai *dai)
{
	struct snd_soc_component *component = dai->component;
	struct rt1320_sdw_priv *rt1320 =
		snd_soc_component_get_drvdata(component);
	struct sdw_stream_config stream_config;
	struct sdw_port_config port_config;
	struct sdw_stream_runtime *sdw_stream;
	int retval;
	unsigned int sampling_rate;

	dev_dbg(dai->dev, "%s %s", __func__, dai->name);
	sdw_stream = snd_soc_dai_get_dma_data(dai, substream);

	if (!sdw_stream)
		return -EINVAL;

	if (!rt1320->sdw_slave)
		return -EINVAL;

	/* SoundWire specific configuration */
	snd_sdw_params_to_config(substream, params, &stream_config, &port_config);

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK) {
		if (dai->id == RT1320_AIF1)
			port_config.num = 1;
		else
			return -EINVAL;
	} else {
		if (dai->id == RT1320_AIF1)
			port_config.num = 4;
		else
			return -EINVAL;
	}

	retval = sdw_stream_add_slave(rt1320->sdw_slave, &stream_config,
				&port_config, 1, sdw_stream);
	if (retval) {
		dev_err(dai->dev, "%s: Unable to configure port\n", __func__);
		return retval;
	}

	/* sampling rate configuration */
	switch (params_rate(params)) {
	case 16000:
		sampling_rate = RT1320_SDCA_RATE_16000HZ;
		break;
	case 32000:
		sampling_rate = RT1320_SDCA_RATE_32000HZ;
		break;
	case 44100:
		sampling_rate = RT1320_SDCA_RATE_44100HZ;
		break;
	case 48000:
		sampling_rate = RT1320_SDCA_RATE_48000HZ;
		break;
	case 96000:
		sampling_rate = RT1320_SDCA_RATE_96000HZ;
		break;
	case 192000:
		sampling_rate = RT1320_SDCA_RATE_192000HZ;
		break;
	default:
		dev_err(component->dev, "%s: Rate %d is not supported\n",
			__func__, params_rate(params));
		return -EINVAL;
	}

	/* set sampling frequency */
	regmap_write(rt1320->regmap,
		SDW_SDCA_CTL(FUNC_NUM_AMP, RT1320_SDCA_ENT_CS21, RT1320_SDCA_CTL_SAMPLE_FREQ_INDEX, 0),
		sampling_rate);

	return 0;
}

static int rt1320_sdw_pcm_hw_free(struct snd_pcm_substream *substream,
				struct snd_soc_dai *dai)
{
	struct snd_soc_component *component = dai->component;
	struct rt1320_sdw_priv *rt1320 =
		snd_soc_component_get_drvdata(component);
	struct sdw_stream_runtime *sdw_stream =
		snd_soc_dai_get_dma_data(dai, substream);

	if (!rt1320->sdw_slave)
		return -EINVAL;

	sdw_stream_remove_slave(rt1320->sdw_slave, sdw_stream);
	return 0;
}

/*
 * slave_ops: callbacks for get_clock_stop_mode, clock_stop and
 * port_prep are not defined for now
 */
static const struct sdw_slave_ops rt1320_slave_ops = {
	.read_prop = rt1320_read_prop,
	.update_status = rt1320_update_status,
};

static int rt1320_sdw_component_probe(struct snd_soc_component *component)
{
	int ret;
	struct rt1320_sdw_priv *rt1320 = snd_soc_component_get_drvdata(component);

	rt1320->component = component;

	if (!rt1320->first_hw_init)
		return 0;

	ret = pm_runtime_resume(component->dev);
	dev_dbg(&rt1320->sdw_slave->dev, "%s pm_runtime_resume, ret=%d", __func__, ret);
	if (ret < 0 && ret != -EACCES)
		return ret;

	return 0;
}

static const struct snd_soc_component_driver soc_component_sdw_rt1320 = {
	.probe = rt1320_sdw_component_probe,
	.controls = rt1320_snd_controls,
	.num_controls = ARRAY_SIZE(rt1320_snd_controls),
	.dapm_widgets = rt1320_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(rt1320_dapm_widgets),
	.dapm_routes = rt1320_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(rt1320_dapm_routes),
	.endianness = 1,
};

static const struct snd_soc_dai_ops rt1320_aif_dai_ops = {
	.hw_params = rt1320_sdw_hw_params,
	.hw_free	= rt1320_sdw_pcm_hw_free,
	.set_stream	= rt1320_set_sdw_stream,
	.shutdown	= rt1320_sdw_shutdown,
};

#define RT1320_STEREO_RATES (SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | \
	SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_96000 | SNDRV_PCM_RATE_192000)
#define RT1320_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE | \
	SNDRV_PCM_FMTBIT_S32_LE)

static struct snd_soc_dai_driver rt1320_sdw_dai[] = {
	{
		.name = "rt1320-aif1",
		.id = RT1320_AIF1,
		.playback = {
			.stream_name = "DP1 Playback",
			.channels_min = 1,
			.channels_max = 2,
			.rates = RT1320_STEREO_RATES,
			.formats = RT1320_FORMATS,
		},
		.capture = {
			.stream_name = "DP4 Capture",
			.channels_min = 1,
			.channels_max = 2,
			.rates = RT1320_STEREO_RATES,
			.formats = RT1320_FORMATS,
		},
		.ops = &rt1320_aif_dai_ops,
	},
};

static int rt1320_sdw_init(struct device *dev, struct regmap *regmap,
				struct regmap *mbq_regmap, struct sdw_slave *slave)
{
	struct rt1320_sdw_priv *rt1320;
	int ret;

	rt1320 = devm_kzalloc(dev, sizeof(*rt1320), GFP_KERNEL);
	if (!rt1320)
		return -ENOMEM;

	dev_set_drvdata(dev, rt1320);
	rt1320->sdw_slave = slave;
	rt1320->mbq_regmap = mbq_regmap;
	rt1320->regmap = regmap;

	regcache_cache_only(rt1320->regmap, true);
	regcache_cache_only(rt1320->mbq_regmap, true);

	/*
	 * Mark hw_init to false
	 * HW init will be performed when device reports present
	 */
	rt1320->hw_init = false;
	rt1320->first_hw_init = false;
	rt1320->version_id = -1;

	ret =  devm_snd_soc_register_component(dev,
				&soc_component_sdw_rt1320,
				rt1320_sdw_dai,
				ARRAY_SIZE(rt1320_sdw_dai));
	if (ret < 0)
		return ret;

	/* set autosuspend parameters */
	pm_runtime_set_autosuspend_delay(dev, 3000);
	pm_runtime_use_autosuspend(dev);

	/* make sure the device does not suspend immediately */
	pm_runtime_mark_last_busy(dev);

	pm_runtime_enable(dev);

	/* important note: the device is NOT tagged as 'active' and will remain
	 * 'suspended' until the hardware is enumerated/initialized. This is required
	 * to make sure the ASoC framework use of pm_runtime_get_sync() does not silently
	 * fail with -EACCESS because of race conditions between card creation and enumeration
	 */

	dev_dbg(dev, "%s\n", __func__);

	return ret;
}

static int rt1320_sdw_probe(struct sdw_slave *slave,
				const struct sdw_device_id *id)
{
	struct regmap *regmap, *mbq_regmap;

	/* Regmap Initialization */
	mbq_regmap = devm_regmap_init_sdw_mbq(slave, &rt1320_mbq_regmap);
	if (IS_ERR(mbq_regmap))
		return PTR_ERR(mbq_regmap);

	regmap = devm_regmap_init_sdw(slave, &rt1320_sdw_regmap);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	return rt1320_sdw_init(&slave->dev, regmap, mbq_regmap, slave);
}

static int rt1320_sdw_remove(struct sdw_slave *slave)
{
	pm_runtime_disable(&slave->dev);

	return 0;
}

/*
 * Version A/B will use the class id 0
 * The newer version than A/B will use the class id 1, so add it in advance
 */
static const struct sdw_device_id rt1320_id[] = {
	SDW_SLAVE_ENTRY_EXT(0x025d, 0x1320, 0x3, 0x0, 0),
	SDW_SLAVE_ENTRY_EXT(0x025d, 0x1320, 0x3, 0x1, 0),
	{},
};
MODULE_DEVICE_TABLE(sdw, rt1320_id);

static int __maybe_unused rt1320_dev_suspend(struct device *dev)
{
	struct rt1320_sdw_priv *rt1320 = dev_get_drvdata(dev);

	if (!rt1320->hw_init)
		return 0;

	regcache_cache_only(rt1320->regmap, true);
	regcache_cache_only(rt1320->mbq_regmap, true);
	return 0;
}

#define RT1320_PROBE_TIMEOUT 5000

static int __maybe_unused rt1320_dev_resume(struct device *dev)
{
	struct sdw_slave *slave = dev_to_sdw_dev(dev);
	struct rt1320_sdw_priv *rt1320 = dev_get_drvdata(dev);
	unsigned long time;

	if (!rt1320->first_hw_init)
		return 0;

	if (!slave->unattach_request)
		goto regmap_sync;

	time = wait_for_completion_timeout(&slave->initialization_complete,
				msecs_to_jiffies(RT1320_PROBE_TIMEOUT));
	if (!time) {
		dev_err(&slave->dev, "%s: Initialization not complete, timed out\n", __func__);
		return -ETIMEDOUT;
	}

regmap_sync:
	slave->unattach_request = 0;
	regcache_cache_only(rt1320->regmap, false);
	regcache_sync(rt1320->regmap);
	regcache_cache_only(rt1320->mbq_regmap, false);
	regcache_sync(rt1320->mbq_regmap);
	return 0;
}

static const struct dev_pm_ops rt1320_pm = {
	SET_SYSTEM_SLEEP_PM_OPS(rt1320_dev_suspend, rt1320_dev_resume)
	SET_RUNTIME_PM_OPS(rt1320_dev_suspend, rt1320_dev_resume, NULL)
};

static struct sdw_driver rt1320_sdw_driver = {
	.driver = {
		.name = "rt1320-sdca",
		.pm = &rt1320_pm,
	},
	.probe = rt1320_sdw_probe,
	.remove = rt1320_sdw_remove,
	.ops = &rt1320_slave_ops,
	.id_table = rt1320_id,
};
module_sdw_driver(rt1320_sdw_driver);

MODULE_DESCRIPTION("ASoC RT1320 driver SDCA SDW");
MODULE_AUTHOR("Shuming Fan <shumingf@realtek.com>");
MODULE_LICENSE("GPL");

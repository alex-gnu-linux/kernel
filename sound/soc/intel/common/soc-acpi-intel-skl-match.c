// SPDX-License-Identifier: GPL-2.0-only
/*
 * soc-acpi-intel-skl-match.c - tables and support for SKL ACPI enumeration.
 *
 * Copyright (c) 2018, Intel Corporation.
 *
 */

#include <sound/soc-acpi.h>
#include <sound/soc-acpi-intel-match.h>

static const struct snd_soc_acpi_codecs skl_codecs = {
	.num_codecs = 1,
	.codecs = {"10508825"}
};

struct snd_soc_acpi_mach snd_soc_acpi_intel_skl_machines[] = {
	{
		.id = "INT343A",
		.drv_name = "skl_alc286s_i2s",
		.fw_filename = "/*(DEBLOBBED)*/",
	},
	{
		.id = "INT343B",
		.drv_name = "skl_n88l25_s4567",
		.fw_filename = "/*(DEBLOBBED)*/",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &skl_codecs,
	},
	{
		.id = "MX98357A",
		.drv_name = "skl_n88l25_m98357a",
		.fw_filename = "/*(DEBLOBBED)*/",
		.machine_quirk = snd_soc_acpi_codec_list,
		.quirk_data = &skl_codecs,
	},
	{},
};
EXPORT_SYMBOL_GPL(snd_soc_acpi_intel_skl_machines);

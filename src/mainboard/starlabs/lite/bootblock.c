/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include <intelblocks/lpc_lib.h>
#include <gpio.h>
#include <variants.h>

void bootblock_mainboard_init(void)
{
	const struct pad_config *pads;
	size_t num;

	pads = variant_early_gpio_table(&num);
	gpio_configure_pads(pads, num);

	if (CONFIG(EC_STARLABS_NUVOTON))
		lpc_open_mmio_window(0xfe800000, 0x10000);
}

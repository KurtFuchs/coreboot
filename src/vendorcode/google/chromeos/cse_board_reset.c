/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <arch/cache.h>
#include <arch/io.h>
#include <cf9_reset.h>
#include <console/console.h>
#include <drivers/spi/tpm/tpm.h>
#include <ec/google/chromeec/ec.h>
#include <halt.h>
#include <intelblocks/cse.h>
#include <security/tpm/tss.h>
#include <vb2_api.h>

void cse_board_reset(void)
{
	tpm_result_t rc;
	struct cr50_firmware_version version;

	if (CONFIG(CSE_RESET_CLEAR_EC_AP_IDLE_FLAG))
		google_chromeec_clear_ec_ap_idle();

	/*
	 * Assuming that if particular TPM implementation is enabled at compile
	 * time, it's the one being used.  This isn't generic code, so can
	 * probably get away with it.
	 */
	if (CONFIG(TPM2) && CONFIG(TPM_GOOGLE_CR50)) {
		/* Initialize TPM and get the cr50 firmware version. */
		rc = tlcl_lib_init();
		if (rc != TPM_SUCCESS) {
			printk(BIOS_ERR, "tlcl_lib_init() failed: %#x\n", rc);
			return;
		}

		cr50_get_firmware_version(&version);

		/*
		 * Cr50 firmware versions 0.[3|4].20 or newer support strap
		 * config 0xe where PLTRST from AP is connected to cr50's
		 * PLTRST# signal. So return immediately and trigger a global
		 * reset.
		 */
		if (version.epoch != 0 || version.major > 4 ||
		    (version.major >= 3 && version.minor >= 20))
			return;
	}
	if (CONFIG(TPM_GOOGLE_TI50)) {
		/* All versions of Ti50 firmware support the above PLTRST wiring. */
		return;
	}

	printk(BIOS_INFO, "Initiating request to EC to trigger cold reset\n");
	/*
	 * Clean the data cache and set the full reset bit, so that when EC toggles
	 * SYS_RESET# pin, AP makes a trip to S5 and then to S0.
	 */
	dcache_clean_all();
	outb(FULL_RST, RST_CNT);
	if (!google_chromeec_ap_reset())
		halt();
}

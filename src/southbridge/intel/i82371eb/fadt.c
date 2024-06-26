/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 * Based on src/southbridge/via/vt8237r/vt8237_fadt.c
 */

#include <acpi/acpi.h>
#include <device/device.h>
#include <device/pci.h>

#include "i82371eb.h"

/**
 * Create the Fixed ACPI Description Tables (FADT) for any board with this SB.
 * Reference: ACPIspec40a, 5.2.9, page 118
 */
void acpi_fill_fadt(acpi_fadt_t *fadt)
{
	fadt->pm1a_evt_blk = DEFAULT_PMBASE;
	fadt->pm1a_cnt_blk = DEFAULT_PMBASE + PMCNTRL;

	fadt->pm_tmr_blk = DEFAULT_PMBASE + PMTMR;
	fadt->gpe0_blk = DEFAULT_PMBASE + GPSTS;

	/* *_len define register width in bytes */
	fadt->pm1_evt_len = 4;
	fadt->pm1_cnt_len = 2;
	fadt->pm_tmr_len = 4;
	fadt->gpe0_blk_len = 4;

	fill_fadt_extended_pm_io(fadt);

	/*
	 * bit  meaning
	 * 0    1: We have user-visible legacy devices
	 * 1    1: 8042
	 * 2    0: VGA is ok to probe
	 * 3    1: MSI are not supported
	 */
	fadt->iapc_boot_arch = ACPI_FADT_LEGACY_DEVICES | ACPI_FADT_8042 |
			       ACPI_FADT_MSI_NOT_SUPPORTED;
	/*
	 * bit  meaning
	 * 0    WBINVD
	 *      Processors in new ACPI-compatible systems are required to
	 *      support this function and indicate this to OSPM by setting
	 *      this field.
	 * 1    WBINVD_FLUSH
	 *      If set, indicates that the hardware flushes all caches on the
	 *      WBINVD instruction and maintains memory coherency, but does
	 *      not guarantee the caches are invalidated.
	 * 2    PROC_C1
	 *      C1 power state (x86 hlt instruction) is supported on all cpus
	 * 3    P_LVL2_UP
	 *      0: C2 only on uniprocessor, 1: C2 on uni- and multiprocessor
	 * 4    PWR_BUTTON
	 *      0: pwr button is fixed feature
	 *      1: pwr button has control method device if present
	 * 5    SLP_BUTTON
	 *      0: sleep button is fixed feature
	 *      1: sleep button has control method device if present
	 * 6    FIX_RTC
	 *      0: RTC wake status supported in fixed register spce
	 * 7    RTC_S4
	 *      1: RTC can wake from S4
	 * 8    TMR_VAL_EXT
	 *      1: pmtimer is 32bit, 0: pmtimer is 24bit
	 * 9    DCK_CAP
	 *      1: system supports docking station
	 * 10   RESET_REG_SUPPORT
	 *      1: fadt describes reset register for system reset
	 * 11   SEALED_CASE
	 *      1: No expansion possible, sealed case
	 * 12   HEADLESS
	 *      1: Video output, keyboard and mouse are not connected
	 * 13   CPU_SW_SLP
	 *      1: Special processor instruction needs to be executed
	 *      after writing SLP_TYP
	 * 14   PCI_EXP_WAK
	 *      1: PM1 regs support PCIEXP_WAKE_(STS|EN), must be set
	 *      on platforms with pci express support
	 * 15   USE_PLATFORM_CLOCK
	 *      1: OS should prefer platform clock over processor internal
	 *      clock.
	 * 16   S4_RTC_STS_VALID
	 * 17   REMOTE_POWER_ON_CAPABLE
	 *      1: platform correctly supports OSPM leaving GPE wake events
	 *      armed prior to an S5 transition.
	 * 18   FORCE_APIC_CLUSTER_MODEL
	 * 19   FORCE_APIC_PHYSICAL_DESTINATION_MODE
	 */
	fadt->flags |= ACPI_FADT_WBINVD | ACPI_FADT_C1_SUPPORTED | ACPI_FADT_SLEEP_BUTTON |
		       ACPI_FADT_S4_RTC_WAKE;
}

/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#ifndef __GIC_V3__H
#define __GIC_V3__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GIC_IRQ_ID_NUM_SGI
/** @brief Max number of Software Generated Interrupts (SGI) supported */
#define GIC_IRQ_ID_NUM_SGI (16)
#endif // GIC_IRQ_ID_NUM_SGI

#ifndef GIC_IRQ_ID_NUM_PPI
/** @brief Max number of Private Peripheral Interrupts (PPI) supported */
#define GIC_IRQ_ID_NUM_PPI (32)
#endif // GIC_IRQ_ID_NUM_PPI

/** @brief Software generated interrupt starting interrupt id */
#define GIC_SGI_INT_ID_START (0)

/** @brief Private generated interrupt starting interrupt id */
#define GIC_PPI_INT_ID_START (16)

/** @brief Shared generated interrupt starting interrupt id */
#define GIC_SPI_INT_ID_START (32)

#ifndef GIC_V3_1
#define GIC_IS_SPI(n) ((n >= 32u) && (n <= 1019u))
#else
#define GIC_IS_SPI(n) ((n >= 32u) && (n <= 1019u)) || ((n >= 4096u) && (n <= 5119u))
#endif // GIC_V3_1

#define GIC_IS_LPI(n) (n >= 8192u)

/* Address access from GIC Distributor base address */
#define GICD_CTLR(base)           (base + 0x0000)
#define GICD_TYPER(base)          (base + 0x0004)
#define GICD_IIDR(base)           (base + 0x0008)
#define GICD_TYPER2(base)         (base + 0x0010)
#define GICD_ISENABLERn(base, n)  (base + 0x0100 + (n * 4))
#define GICD_IGROUPRn(base, n)    (base + 0x0080 + (n * 4))
#define GICD_ICENABLERn(base, n)  (base + 0x0180 + (n * 4))
#define GICD_ISPENDRn(base, n)    (base + 0x0200 + (n * 4))
#define GICD_ICPENDRn(base, n)    (base + 0x0280 + (n * 4))
#define GICD_ISACTIVERn(base, n)  (base + 0x0300 + (n * 4))
#define GICD_ICACTIVERn(base, n)  (base + 0x0380 + (n * 4))
#define GICD_IPRIORITYRn(base, n) (base + 0x0400 + (n))
#define GICD_ITARGETSRn(base, n)  (base + 0x0800 + (n * 4))
#define GICD_ICFGRn(base, n)      (base + 0x0c00 + (n * 4))
#define GICD_IGROUPMODRn(base, n) (base + 0x0d00 + (n * 4))
#define GICD_NSACRn(base, n)      (base + 0x0e00 + (n * 4))

/* Address access from GIC Redistributor base address */
#define GICR_CTLR(base)           (base + 0x0000)
#define GICR_IIDR(base)           (base + 0x0004)
#define GICR_TYPER(base)          (base + 0x0008)
#define GICR_STATUSR(base)        (base + 0x0010)
#define GICR_WAKER(base)          (base + 0x0014)
#define GICR_IGROUPR0(base)       (base + 0x0080)
#define GICR_ISENABLER0(base)     (base + 0x0100)
#define GICR_ICENABLER0(base)     (base + 0x0180)
#define GICR_ISPENDR0(base)       (base + 0x0200)
#define GICR_ICPENDR0(base)       (base + 0x0280)
#define GICR_ISACTIVER0(base)     (base + 0x0300)
#define GICR_ICACTIVER0(base)     (base + 0x0380)
#define GICR_IPRIORITYRn(base, n) (base + 0x0400 + (n * 4))
#define GICR_ICFGR0(base)         (base + 0x0c00)
#define GICR_ICFGR1(base)         (base + 0x0c04)
#define GICR_IGROUPMODR0(base)    (base + 0x0d00)
#define GICR_NSACR(base, n)       (base + 0x0e00)

/* GICR Register bit defintions */
#define GICR_WAKER_CA_BIT_MSK (1u << 1) /* children asleep */
#define GICR_WAKER_PS_BIT_MSK (1u << 1) /* processor sleep */

/* GICR_CTLR Register bit definitions */
#define GICR_CTLR_RWP (1u << 3)

/* GICD_CTLR Interrupt group definitions */
#define GICD_CTLR_ENABLE_G0   (1u << 0)
#define GICD_CTLR_ENABLE_G1NS (1u << 1)
#define GICD_CTLR_ENABLE_G1S  (1u << 2)
#define GICD_CTRL_ARE_S       (1u << 4)
#define GICD_CTRL_ARE_NS      (1u << 5)
#define GICD_CTRL_NS          (1u << 6)
#define GICD_CGRL_E1NWF       (1u << 7)
#define GICD_CTLR_RWP         (1u << 31) /* GICD_CTLR Register write progress bit */

/* Non-secure view */
#define GICD_CTLR_ENABLE_G1A (1u << 1)

/* CPUIF bit fields */
#define ICC_SRE_SRE_ENABLE    (1ull << 0) /* sysreg gic interface enable */
#define ICC_SRE_DFB_ENABLE    (1ull << 1) /* disable fiq bypass */
#define ICC_SRE_DIB_ENABLE    (1ull << 2) /* disable irq bypass */
#define ICC_SRE_LOW_EL_ENABLE (1ull << 3) /* low EL access enable */

#define ICC_CTLR_PMHE (1ull << 6) /* priority mask hint enable */
#define ICC_CTLR_RM   (1ull << 5) /* set if EL3 can ack or observe interrupts */
#define ICC_CTLR_EOI_MODE_EL1_NS                                                                   \
	(1ull << 4) /* write to EOI at EL1 secured deactivate the interrupt  */
#define ICC_CTLR_EOI_MODE_EL1_S                                                                    \
	(1ull << 3) /* write to EOI at EL1 secured deactivate the interrupt  */
#define ICC_CTLR_EOI_MODE_EL3_S (1ull << 2) /* write to EOI at EL3 deactivate the interrupt  */
#define ICC_CTLR_CBPR_EL1_NS    (1ull << 2) /* common binary point reg enable at EL1 non-secure */
#define ICC_CTLR_CBPR_EL1_S     (1ull << 1) /* common binary point reg enable at EL1 secure */

#ifdef __cplusplus
}
#endif

#endif // __GIC_V3__H

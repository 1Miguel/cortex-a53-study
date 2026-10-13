/*******************************************************************************
 * @file uart_pl011.h
 * @brief UART PL011 Primecell UART.
 *    for more info refer to refer to PrimeCell UART (PL011)
 *    r1p5 Technical Reference Manual
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#ifndef UART_Pl011_REGS__H
#define UART_Pl011_REGS__H

#include <stdint.h>

#define UARTDR(base)     ((uintptr_t)(base + 0x0000)) /* data register */
#define UARTRSR(base)    ((uintptr_t)(base + 0x0004)) /* receive status register / error clear */
#define UARTFR(base)     ((uintptr_t)(base + 0x0018)) /* flag register */
#define UARTILPR(base)   ((uintptr_t)(base + 0x0020)) /* irDa low-power counter */
#define UARTIBRD(base)   ((uintptr_t)(base + 0x0024)) /* integer baudrate */
#define UARTFBRD(base)   ((uintptr_t)(base + 0x0028)) /* fractional baudrate */
#define UARTLCR_H(base)  ((uintptr_t)(base + 0x002C)) /* line control register */
#define UARTCR(base)     ((uintptr_t)(base + 0x0030)) /* control register */
#define UARTIFLS(base)   ((uintptr_t)(base + 0x0034)) /* interrupt fifo level select */
#define UARTIMSC(base)   ((uintptr_t)(base + 0x0038)) /* interrupt mask set/clear */
#define UARTRIS(base)    ((uintptr_t)(base + 0x003C)) /* raw interrupt Status */
#define UARTMIS(base)    ((uintptr_t)(base + 0x0040)) /* masked interrupt Status */
#define UARTICR(base)    ((uintptr_t)(base + 0x0044)) /* interrupt Clear */
#define UARTIDMACR(base) ((uintptr_t)(base + 0x0048)) /* dma Control */

/* ============================================================================
 * DR - Data Register
 * ========================================================================== */

#define UART_PL011_DR_DATA_BIT_POS (0U)
#define UART_PL011_DR_DATA_BIT_MSK (0xFFU << UART_PL011_DR_DATA_BIT_POS)
#define UART_PL011_DR_DATA_RST_VAL (0x00U)

#define UART_PL011_DR_FE_BIT_POS (8U)
#define UART_PL011_DR_FE_BIT_MSK (1U << UART_PL011_DR_FE_BIT_POS)
#define UART_PL011_DR_FE_RST_VAL (0x00U)

#define UART_PL011_DR_PE_BIT_POS (9U)
#define UART_PL011_DR_PE_BIT_MSK (1U << UART_PL011_DR_PE_BIT_POS)
#define UART_PL011_DR_PE_RST_VAL (0x00U)

#define UART_PL011_DR_BE_BIT_POS (10U)
#define UART_PL011_DR_BE_BIT_MSK (1U << UART_PL011_DR_BE_BIT_POS)
#define UART_PL011_DR_BE_RST_VAL (0x00U)

#define UART_PL011_DR_OE_BIT_POS (11U)
#define UART_PL011_DR_OE_BIT_MSK (1U << UART_PL011_DR_OE_BIT_POS)
#define UART_PL011_DR_OE_RST_VAL (0x00U)

/* ============================================================================
 * RSR_ECR - Receive Status / Error Clear Register
 * ========================================================================== */

#define UART_PL011_RSR_ECR_FE_BIT_POS (0U)
#define UART_PL011_RSR_ECR_FE_BIT_MSK (1U << UART_PL011_RSR_ECR_FE_BIT_POS)
#define UART_PL011_RSR_ECR_FE_RST_VAL (0x00U)

#define UART_PL011_RSR_ECR_PE_BIT_POS (1U)
#define UART_PL011_RSR_ECR_PE_BIT_MSK (1U << UART_PL011_RSR_ECR_PE_BIT_POS)
#define UART_PL011_RSR_ECR_PE_RST_VAL (0x00U)

#define UART_PL011_RSR_ECR_BE_BIT_POS (2U)
#define UART_PL011_RSR_ECR_BE_BIT_MSK (1U << UART_PL011_RSR_ECR_BE_BIT_POS)
#define UART_PL011_RSR_ECR_BE_RST_VAL (0x00U)

#define UART_PL011_RSR_ECR_OE_BIT_POS (3U)
#define UART_PL011_RSR_ECR_OE_BIT_MSK (1U << UART_PL011_RSR_ECR_OE_BIT_POS)
#define UART_PL011_RSR_ECR_OE_RST_VAL (0x00U)

/* ============================================================================
 * FR - Flag Register
 * ========================================================================== */

#define UART_PL011_FR_CTS_BIT_POS (0U)
#define UART_PL011_FR_CTS_BIT_MSK (1U << UART_PL011_FR_CTS_BIT_POS)
#define UART_PL011_FR_CTS_RST_VAL (0x00U)

#define UART_PL011_FR_DSR_BIT_POS (1U)
#define UART_PL011_FR_DSR_BIT_MSK (1U << UART_PL011_FR_DSR_BIT_POS)
#define UART_PL011_FR_DSR_RST_VAL (0x00U)

#define UART_PL011_FR_DCD_BIT_POS (2U)
#define UART_PL011_FR_DCD_BIT_MSK (1U << UART_PL011_FR_DCD_BIT_POS)
#define UART_PL011_FR_DCD_RST_VAL (0x00U)

#define UART_PL011_FR_BUSY_BIT_POS (3U)
#define UART_PL011_FR_BUSY_BIT_MSK (1U << UART_PL011_FR_BUSY_BIT_POS)
#define UART_PL011_FR_BUSY_RST_VAL (0x00U)

#define UART_PL011_FR_RXFE_BIT_POS (4U)
#define UART_PL011_FR_RXFE_BIT_MSK (1U << UART_PL011_FR_RXFE_BIT_POS)
#define UART_PL011_FR_RXFE_RST_VAL (0x01U)

#define UART_PL011_FR_TXFF_BIT_POS (5U)
#define UART_PL011_FR_TXFF_BIT_MSK (1U << UART_PL011_FR_TXFF_BIT_POS)
#define UART_PL011_FR_TXFF_RST_VAL (0x01U)

#define UART_PL011_FR_RXFF_BIT_POS (6U)
#define UART_PL011_FR_RXFF_BIT_MSK (1U << UART_PL011_FR_RXFF_BIT_POS)
#define UART_PL011_FR_RXFF_RST_VAL (0x00U)

#define UART_PL011_FR_TXFE_BIT_POS (7U)
#define UART_PL011_FR_TXFE_BIT_MSK (1U << UART_PL011_FR_TXFE_BIT_POS)
#define UART_PL011_FR_TXFE_RST_VAL (0x01U)

#define UART_PL011_FR_RI_BIT_POS (8U)
#define UART_PL011_FR_RI_BIT_MSK (1U << UART_PL011_FR_RI_BIT_POS)
#define UART_PL011_FR_RI_RST_VAL (0x00U)

/* ============================================================================
 * ILPR - IrDA Low-Power Counter Register
 * ========================================================================== */

#define UART_PL011_ILPR_ILPDVSR_BIT_POS (0U)
#define UART_PL011_ILPR_ILPDVSR_BIT_MSK (0xFFU << UART_PL011_ILPR_ILPDVSR_BIT_POS)
#define UART_PL011_ILPR_ILPDVSR_RST_VAL (0x00U)

/* ============================================================================
 * IBRD - Integer Baud Rate Divisor
 * ========================================================================== */

#define UART_PL011_IBRD_BAUD_DIVINT_BIT_POS (0U)
#define UART_PL011_IBRD_BAUD_DIVINT_BIT_MSK (0xFFFFU << UART_PL011_IBRD_BAUD_DIVINT_BIT_POS)
#define UART_PL011_IBRD_BAUD_DIVINT_RST_VAL (0x0000U)

/* ============================================================================
 * FBRD - Fractional Baud Rate Divisor
 * ========================================================================== */

#define UART_PL011_FBRD_BAUD_DIVFRAC_BIT_POS (0U)
#define UART_PL011_FBRD_BAUD_DIVFRAC_BIT_MSK (0x3FU << UART_PL011_FBRD_BAUD_DIVFRAC_BIT_POS)
#define UART_PL011_FBRD_BAUD_DIVFRAC_RST_VAL (0x00U)

/* ============================================================================
 * LCR_H - Line Control Register
 * ========================================================================== */

#define UART_PL011_LCR_H_BRK_BIT_POS (0U)
#define UART_PL011_LCR_H_BRK_BIT_MSK (1U << UART_PL011_LCR_H_BRK_BIT_POS)
#define UART_PL011_LCR_H_BRK_RST_VAL (0x00U)

#define UART_PL011_LCR_H_PEN_BIT_POS (1U)
#define UART_PL011_LCR_H_PEN_BIT_MSK (1U << UART_PL011_LCR_H_PEN_BIT_POS)
#define UART_PL011_LCR_H_PEN_RST_VAL (0x00U)

#define UART_PL011_LCR_H_EPS_BIT_POS (2U)
#define UART_PL011_LCR_H_EPS_BIT_MSK (1U << UART_PL011_LCR_H_EPS_BIT_POS)
#define UART_PL011_LCR_H_EPS_RST_VAL (0x00U)

#define UART_PL011_LCR_H_STP2_BIT_POS (3U)
#define UART_PL011_LCR_H_STP2_BIT_MSK (1U << UART_PL011_LCR_H_STP2_BIT_POS)
#define UART_PL011_LCR_H_STP2_RST_VAL (0x00U)

#define UART_PL011_LCR_H_FEN_BIT_POS (4U)
#define UART_PL011_LCR_H_FEN_BIT_MSK (1U << UART_PL011_LCR_H_FEN_BIT_POS)
#define UART_PL011_LCR_H_FEN_RST_VAL (0x00U)

#define UART_PL011_LCR_H_WLEN_BIT_POS (5U)
#define UART_PL011_LCR_H_WLEN_BIT_MSK (3U << UART_PL011_LCR_H_WLEN_BIT_POS)
#define UART_PL011_LCR_H_WLEN_RST_VAL (0x00U)

#define UART_PL011_LCR_H_SPS_BIT_POS (7U)
#define UART_PL011_LCR_H_SPS_BIT_MSK (1U << UART_PL011_LCR_H_SPS_BIT_POS)
#define UART_PL011_LCR_H_SPS_RST_VAL (0x00U)

/* ============================================================================
 * CR - Control Register
 * ========================================================================== */

#define UART_PL011_CR_UARTEN_BIT_POS (0U)
#define UART_PL011_CR_UARTEN_BIT_MSK (1U << UART_PL011_CR_UARTEN_BIT_POS)
#define UART_PL011_CR_UARTEN_RST_VAL (0x00U)

#define UART_PL011_CR_SIREN_BIT_POS (1U)
#define UART_PL011_CR_SIREN_BIT_MSK (1U << UART_PL011_CR_SIREN_BIT_POS)
#define UART_PL011_CR_SIREN_RST_VAL (0x00U)

#define UART_PL011_CR_SIRLP_BIT_POS (2U)
#define UART_PL011_CR_SIRLP_BIT_MSK (1U << UART_PL011_CR_SIRLP_BIT_POS)
#define UART_PL011_CR_SIRLP_RST_VAL (0x00U)

#define UART_PL011_CR_LBE_BIT_POS (7U)
#define UART_PL011_CR_LBE_BIT_MSK (1U << UART_PL011_CR_LBE_BIT_POS)
#define UART_PL011_CR_LBE_RST_VAL (0x00U)

#define UART_PL011_CR_TXE_BIT_POS (8U)
#define UART_PL011_CR_TXE_BIT_MSK (1U << UART_PL011_CR_TXE_BIT_POS)
#define UART_PL011_CR_TXE_RST_VAL (0x00U)

#define UART_PL011_CR_RXE_BIT_POS (9U)
#define UART_PL011_CR_RXE_BIT_MSK (1U << UART_PL011_CR_RXE_BIT_POS)
#define UART_PL011_CR_RXE_RST_VAL (0x00U)

#define UART_PL011_CR_DTR_BIT_POS (10U)
#define UART_PL011_CR_DTR_BIT_MSK (1U << UART_PL011_CR_DTR_BIT_POS)
#define UART_PL011_CR_DTR_RST_VAL (0x00U)

#define UART_PL011_CR_RTS_BIT_POS (11U)
#define UART_PL011_CR_RTS_BIT_MSK (1U << UART_PL011_CR_RTS_BIT_POS)
#define UART_PL011_CR_RTS_RST_VAL (0x00U)

#define UART_PL011_CR_OUT1_BIT_POS (12U)
#define UART_PL011_CR_OUT1_BIT_MSK (1U << UART_PL011_CR_OUT1_BIT_POS)
#define UART_PL011_CR_OUT1_RST_VAL (0x00U)

#define UART_PL011_CR_OUT2_BIT_POS (13U)
#define UART_PL011_CR_OUT2_BIT_MSK (1U << UART_PL011_CR_OUT2_BIT_POS)
#define UART_PL011_CR_OUT2_RST_VAL (0x00U)

#define UART_PL011_CR_RTSEN_BIT_POS (14U)
#define UART_PL011_CR_RTSEN_BIT_MSK (1U << UART_PL011_CR_RTSEN_BIT_POS)
#define UART_PL011_CR_RTSEN_RST_VAL (0x00U)

#define UART_PL011_CR_CTSEN_BIT_POS (15U)
#define UART_PL011_CR_CTSEN_BIT_MSK (1U << UART_PL011_CR_CTSEN_BIT_POS)
#define UART_PL011_CR_CTSEN_RST_VAL (0x00U)

/* ============================================================================
 * IFLS - Interrupt FIFO Level Select Register
 * ========================================================================== */

#define UART_PL011_IFLS_TXIFLSEL_BIT_POS (0U)
#define UART_PL011_IFLS_TXIFLSEL_BIT_MSK (7U << UART_PL011_IFLS_TXIFLSEL_BIT_POS)
#define UART_PL011_IFLS_TXIFLSEL_RST_VAL (0x02U)

#define UART_PL011_IFLS_RXIFLSEL_BIT_POS (3U)
#define UART_PL011_IFLS_RXIFLSEL_BIT_MSK (7U << UART_PL011_IFLS_RXIFLSEL_BIT_POS)
#define UART_PL011_IFLS_RXIFLSEL_RST_VAL (0x02U)

/* ============================================================================
 * IMSC - Interrupt Mask Set/Clear Register
 * ========================================================================== */

#define UART_PL011_IMSC_RIMIM_BIT_POS (0U)
#define UART_PL011_IMSC_RIMIM_BIT_MSK (1U << UART_PL011_IMSC_RIMIM_BIT_POS)
#define UART_PL011_IMSC_RIMIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_CTSMIM_BIT_POS (1U)
#define UART_PL011_IMSC_CTSMIM_BIT_MSK (1U << UART_PL011_IMSC_CTSMIM_BIT_POS)
#define UART_PL011_IMSC_CTSMIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_DCDMIM_BIT_POS (2U)
#define UART_PL011_IMSC_DCDMIM_BIT_MSK (1U << UART_PL011_IMSC_DCDMIM_BIT_POS)
#define UART_PL011_IMSC_DCDMIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_DSRMIM_BIT_POS (3U)
#define UART_PL011_IMSC_DSRMIM_BIT_MSK (1U << UART_PL011_IMSC_DSRMIM_BIT_POS)
#define UART_PL011_IMSC_DSRMIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_RXIM_BIT_POS (4U)
#define UART_PL011_IMSC_RXIM_BIT_MSK (1U << UART_PL011_IMSC_RXIM_BIT_POS)
#define UART_PL011_IMSC_RXIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_TXIM_BIT_POS (5U)
#define UART_PL011_IMSC_TXIM_BIT_MSK (1U << UART_PL011_IMSC_TXIM_BIT_POS)
#define UART_PL011_IMSC_TXIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_RTIM_BIT_POS (6U)
#define UART_PL011_IMSC_RTIM_BIT_MSK (1U << UART_PL011_IMSC_RTIM_BIT_POS)
#define UART_PL011_IMSC_RTIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_FEIM_BIT_POS (7U)
#define UART_PL011_IMSC_FEIM_BIT_MSK (1U << UART_PL011_IMSC_FEIM_BIT_POS)
#define UART_PL011_IMSC_FEIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_PEIM_BIT_POS (8U)
#define UART_PL011_IMSC_PEIM_BIT_MSK (1U << UART_PL011_IMSC_PEIM_BIT_POS)
#define UART_PL011_IMSC_PEIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_BEIM_BIT_POS (9U)
#define UART_PL011_IMSC_BEIM_BIT_MSK (1U << UART_PL011_IMSC_BEIM_BIT_POS)
#define UART_PL011_IMSC_BEIM_RST_VAL (0x00U)

#define UART_PL011_IMSC_OEIM_BIT_POS (10U)
#define UART_PL011_IMSC_OEIM_BIT_MSK (1U << UART_PL011_IMSC_OEIM_BIT_POS)
#define UART_PL011_IMSC_OEIM_RST_VAL (0x00U)

/* ============================================================================
 * RIS - Raw Interrupt Status Register
 * ========================================================================== */

#define UART_PL011_RIS_RIRMIS_BIT_POS (0U)
#define UART_PL011_RIS_RIRMIS_BIT_MSK (1U << UART_PL011_RIS_RIRMIS_BIT_POS)
#define UART_PL011_RIS_RIRMIS_RST_VAL (0x00U)

#define UART_PL011_RIS_CTSRMIS_BIT_POS (1U)
#define UART_PL011_RIS_CTSRMIS_BIT_MSK (1U << UART_PL011_RIS_CTSRMIS_BIT_POS)
#define UART_PL011_RIS_CTSRMIS_RST_VAL (0x00U)

#define UART_PL011_RIS_DCDRMIS_BIT_POS (2U)
#define UART_PL011_RIS_DCDRMIS_BIT_MSK (1U << UART_PL011_RIS_DCDRMIS_BIT_POS)
#define UART_PL011_RIS_DCDRMIS_RST_VAL (0x00U)

#define UART_PL011_RIS_DSRRMIS_BIT_POS (3U)
#define UART_PL011_RIS_DSRRMIS_BIT_MSK (1U << UART_PL011_RIS_DSRRMIS_BIT_POS)
#define UART_PL011_RIS_DSRRMIS_RST_VAL (0x00U)

#define UART_PL011_RIS_RXRIS_BIT_POS (4U)
#define UART_PL011_RIS_RXRIS_BIT_MSK (1U << UART_PL011_RIS_RXRIS_BIT_POS)
#define UART_PL011_RIS_RXRIS_RST_VAL (0x00U)

#define UART_PL011_RIS_TXRIS_BIT_POS (5U)
#define UART_PL011_RIS_TXRIS_BIT_MSK (1U << UART_PL011_RIS_TXRIS_BIT_POS)
#define UART_PL011_RIS_TXRIS_RST_VAL (0x00U)

#define UART_PL011_RIS_RTRIS_BIT_POS (6U)
#define UART_PL011_RIS_RTRIS_BIT_MSK (1U << UART_PL011_RIS_RTRIS_BIT_POS)
#define UART_PL011_RIS_RTRIS_RST_VAL (0x00U)

#define UART_PL011_RIS_FERIS_BIT_POS (7U)
#define UART_PL011_RIS_FERIS_BIT_MSK (1U << UART_PL011_RIS_FERIS_BIT_POS)
#define UART_PL011_RIS_FERIS_RST_VAL (0x00U)

#define UART_PL011_RIS_PERIS_BIT_POS (8U)
#define UART_PL011_RIS_PERIS_BIT_MSK (1U << UART_PL011_RIS_PERIS_BIT_POS)
#define UART_PL011_RIS_PERIS_RST_VAL (0x00U)

#define UART_PL011_RIS_BERIS_BIT_POS (9U)
#define UART_PL011_RIS_BERIS_BIT_MSK (1U << UART_PL011_RIS_BERIS_BIT_POS)
#define UART_PL011_RIS_BERIS_RST_VAL (0x00U)

#define UART_PL011_RIS_OERIS_BIT_POS (10U)
#define UART_PL011_RIS_OERIS_BIT_MSK (1U << UART_PL011_RIS_OERIS_BIT_POS)
#define UART_PL011_RIS_OERIS_RST_VAL (0x00U)

/* ============================================================================
 * MIS - Masked Interrupt Status Register
 * ========================================================================== */

#define UART_PL011_MIS_RIMMIS_BIT_POS (0U)
#define UART_PL011_MIS_RIMMIS_BIT_MSK (1U << UART_PL011_MIS_RIMMIS_BIT_POS)
#define UART_PL011_MIS_RIMMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_CTSMMIS_BIT_POS (1U)
#define UART_PL011_MIS_CTSMMIS_BIT_MSK (1U << UART_PL011_MIS_CTSMMIS_BIT_POS)
#define UART_PL011_MIS_CTSMMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_DCDMMIS_BIT_POS (2U)
#define UART_PL011_MIS_DCDMMIS_BIT_MSK (1U << UART_PL011_MIS_DCDMMIS_BIT_POS)
#define UART_PL011_MIS_DCDMMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_DSRMMIS_BIT_POS (3U)
#define UART_PL011_MIS_DSRMMIS_BIT_MSK (1U << UART_PL011_MIS_DSRMMIS_BIT_POS)
#define UART_PL011_MIS_DSRMMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_RXMIS_BIT_POS (4U)
#define UART_PL011_MIS_RXMIS_BIT_MSK (1U << UART_PL011_MIS_RXMIS_BIT_POS)
#define UART_PL011_MIS_RXMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_TXMIS_BIT_POS (5U)
#define UART_PL011_MIS_TXMIS_BIT_MSK (1U << UART_PL011_MIS_TXMIS_BIT_POS)
#define UART_PL011_MIS_TXMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_RTMIS_BIT_POS (6U)
#define UART_PL011_MIS_RTMIS_BIT_MSK (1U << UART_PL011_MIS_RTMIS_BIT_POS)
#define UART_PL011_MIS_RTMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_FEMIS_BIT_POS (7U)
#define UART_PL011_MIS_FEMIS_BIT_MSK (1U << UART_PL011_MIS_FEMIS_BIT_POS)
#define UART_PL011_MIS_FEMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_PEMIS_BIT_POS (8U)
#define UART_PL011_MIS_PEMIS_BIT_MSK (1U << UART_PL011_MIS_PEMIS_BIT_POS)
#define UART_PL011_MIS_PEMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_BEMIS_BIT_POS (9U)
#define UART_PL011_MIS_BEMIS_BIT_MSK (1U << UART_PL011_MIS_BEMIS_BIT_POS)
#define UART_PL011_MIS_BEMIS_RST_VAL (0x00U)

#define UART_PL011_MIS_OEMIS_BIT_POS (10U)
#define UART_PL011_MIS_OEMIS_BIT_MSK (1U << UART_PL011_MIS_OEMIS_BIT_POS)
#define UART_PL011_MIS_OEMIS_RST_VAL (0x00U)

/* ============================================================================
 * ICR - Interrupt Clear Register
 * ========================================================================== */

#define UART_PL011_ICR_RIMIC_BIT_POS (0U)
#define UART_PL011_ICR_RIMIC_BIT_MSK (1U << UART_PL011_ICR_RIMIC_BIT_POS)
#define UART_PL011_ICR_RIMIC_RST_VAL (0x00U)

#define UART_PL011_ICR_CTSMIC_BIT_POS (1U)
#define UART_PL011_ICR_CTSMIC_BIT_MSK (1U << UART_PL011_ICR_CTSMIC_BIT_POS)
#define UART_PL011_ICR_CTSMIC_RST_VAL (0x00U)

#define UART_PL011_ICR_DCDMIC_BIT_POS (2U)
#define UART_PL011_ICR_DCDMIC_BIT_MSK (1U << UART_PL011_ICR_DCDMIC_BIT_POS)
#define UART_PL011_ICR_DCDMIC_RST_VAL (0x00U)

#define UART_PL011_ICR_DSRMIC_BIT_POS (3U)
#define UART_PL011_ICR_DSRMIC_BIT_MSK (1U << UART_PL011_ICR_DSRMIC_BIT_POS)
#define UART_PL011_ICR_DSRMIC_RST_VAL (0x00U)

#define UART_PL011_ICR_RXIC_BIT_POS (4U)
#define UART_PL011_ICR_RXIC_BIT_MSK (1U << UART_PL011_ICR_RXIC_BIT_POS)
#define UART_PL011_ICR_RXIC_RST_VAL (0x00U)

#define UART_PL011_ICR_TXIC_BIT_POS (5U)
#define UART_PL011_ICR_TXIC_BIT_MSK (1U << UART_PL011_ICR_TXIC_BIT_POS)
#define UART_PL011_ICR_TXIC_RST_VAL (0x00U)

#define UART_PL011_ICR_RTIC_BIT_POS (6U)
#define UART_PL011_ICR_RTIC_BIT_MSK (1U << UART_PL011_ICR_RTIC_BIT_POS)
#define UART_PL011_ICR_RTIC_RST_VAL (0x00U)

#define UART_PL011_ICR_FEIC_BIT_POS (7U)
#define UART_PL011_ICR_FEIC_BIT_MSK (1U << UART_PL011_ICR_FEIC_BIT_POS)
#define UART_PL011_ICR_FEIC_RST_VAL (0x00U)

#define UART_PL011_ICR_PEIC_BIT_POS (8U)
#define UART_PL011_ICR_PEIC_BIT_MSK (1U << UART_PL011_ICR_PEIC_BIT_POS)
#define UART_PL011_ICR_PEIC_RST_VAL (0x00U)

#define UART_PL011_ICR_BEIC_BIT_POS (9U)
#define UART_PL011_ICR_BEIC_BIT_MSK (1U << UART_PL011_ICR_BEIC_BIT_POS)
#define UART_PL011_ICR_BEIC_RST_VAL (0x00U)

#define UART_PL011_ICR_OEIC_BIT_POS (10U)
#define UART_PL011_ICR_OEIC_BIT_MSK (1U << UART_PL011_ICR_OEIC_BIT_POS)
#define UART_PL011_ICR_OEIC_RST_VAL (0x00U)

/* ============================================================================
 * DMACR - DMA Control Register
 * ========================================================================== */

#define UART_PL011_DMACR_RXDMAE_BIT_POS (0U)
#define UART_PL011_DMACR_RXDMAE_BIT_MSK (1U << UART_PL011_DMACR_RXDMAE_BIT_POS)
#define UART_PL011_DMACR_RXDMAE_RST_VAL (0x00U)

#define UART_PL011_DMACR_TXDMAE_BIT_POS (1U)
#define UART_PL011_DMACR_TXDMAE_BIT_MSK (1U << UART_PL011_DMACR_TXDMAE_BIT_POS)
#define UART_PL011_DMACR_TXDMAE_RST_VAL (0x00U)

#define UART_PL011_DMACR_DMAONERR_BIT_POS (2U)
#define UART_PL011_DMACR_DMAONERR_BIT_MSK (1U << UART_PL011_DMACR_DMAONERR_BIT_POS)
#define UART_PL011_DMACR_DMAONERR_RST_VAL (0x00U)

#endif // UART_Pl011_REGS__H

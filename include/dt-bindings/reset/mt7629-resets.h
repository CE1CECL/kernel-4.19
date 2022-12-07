/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _DT_BINDINGS_RESET_CONTROLLER_MT7629
#define _DT_BINDINGS_RESET_CONTROLLER_MT7629

/* INFRACFG resets */
#define MT7629_INFRA_EMI_MPU_RST		0
#define MT7629_INFRA_UART5_RST			2
#define MT7629_INFRA_CIRQ_EINT_RST		3
#define MT7629_INFRA_APXGPT_RST			4
#define MT7629_INFRA_SCPSYS_RST			5
#define MT7629_INFRA_KP_RST			6
#define MT7629_INFRA_SPI1_RST			7
#define MT7629_INFRA_SPI4_RST			8
#define MT7629_INFRA_SYSTIMER_RST		9
#define MT7629_INFRA_IRRX_RST			10
#define MT7629_INFRA_AO_BUS_RST			16
#define MT7629_INFRA_EMI_RST			32
#define MT7629_INFRA_APMIXED_RST		35
#define MT7629_INFRA_MIPI_RST			36
#define MT7629_INFRA_TRNG_RST			37
#define MT7629_INFRA_SYSCIRQ_RST		38
#define MT7629_INFRA_MIPI_CSI_RST		39
#define MT7629_INFRA_GCE_FAXI_RST		40
#define MT7629_INFRA_I2C_SRAM_RST		41
#define MT7629_INFRA_IOMMU_RST			47

/* PERICFG resets */
#define MT7629_PERI_UART0_SW_RST		0
#define MT7629_PERI_UART1_SW_RST		1
#define MT7629_PERI_UART2_SW_RST		2
#define MT7629_PERI_BTIF_SW_RST			6
#define MT7629_PERI_PWN_SW_RST			8
#define MT7629_PERI_DMA_SW_RST			11
#define MT7629_PERI_NFI_SW_RST			14
#define MT7629_PERI_I2C0_SW_RST			22
#define MT7629_PERI_SPI0_SW_RST			33
#define MT7629_PERI_SPI1_SW_RST			34
#define MT7629_PERI_FLASHIF_SW_RST		36

/* PCIe Subsystem resets */
#define MT7629_PCIE1_CORE_RST			19
#define MT7629_PCIE1_MMIO_RST			20
#define MT7629_PCIE1_HRST			21
#define MT7629_PCIE1_USER_RST			22
#define MT7629_PCIE1_PIPE_RST			23
#define MT7629_PCIE0_CORE_RST			27
#define MT7629_PCIE0_MMIO_RST			28
#define MT7629_PCIE0_HRST			29
#define MT7629_PCIE0_USER_RST			30
#define MT7629_PCIE0_PIPE_RST			31

/* SSUSB Subsystem resets */
#define MT7629_SSUSB_PHY_PWR_RST		3
#define MT7629_SSUSB_MAC_PWR_RST		4

/* ETH Subsystem resets */
#define MT7629_ETHSYS_SYS_RST			0
#define MT7629_ETHSYS_MCM_RST			2
#define MT7629_ETHSYS_HSDMA_RST			5
#define MT7629_ETHSYS_FE_RST			6
#define MT7629_ETHSYS_ESW_RST			16
#define MT7629_ETHSYS_GMAC_RST			23
#define MT7629_ETHSYS_EPHY_RST			24
#define MT7629_ETHSYS_CRYPTO_RST		29
#define MT7629_ETHSYS_PPE_RST			31

#endif  /* _DT_BINDINGS_RESET_CONTROLLER_MT7629 */

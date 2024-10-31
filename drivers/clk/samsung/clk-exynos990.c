// SPDX-License-Identifier: GPL-2.0-only

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#include <dt-bindings/clock/exynos990.h>

#include "clk.h"
#include "clk-cpu.h"
#include "clk-exynos-arm64.h"

#define CLKS_NR_TOP (CLK_GOUT_CLKCMU_APM_BUS + 1)
#define CLKS_NR_HSI0 (CLK_GOUT_XIU_D_HSI0_IPCLKPORT_ACLK + 1)

/* CMU_TOP (FILL THIS IN)*/
#define PLL_LOCKTIME_PLL_G3D 0x00
#define PLL_LOCKTIME_PLL_MMC 0x04
#define PLL_LOCKTIME_PLL_SHARED0 0x08
#define PLL_LOCKTIME_PLL_SHARED1 0x0c
#define PLL_LOCKTIME_PLL_SHARED2 0x10
#define PLL_LOCKTIME_PLL_SHARED3 0x14
#define PLL_LOCKTIME_PLL_SHARED4 0x18
#define PLL_CON3_PLL_G3D 0x10c
#define PLL_CON3_PLL_MMC 0x14c
#define PLL_CON5_PLL_MMC 0x154
#define PLL_CON3_PLL_SHARED0 0x18c
#define PLL_CON3_PLL_SHARED1 0x1cc
#define PLL_CON3_PLL_SHARED2 0x20c
#define PLL_CON3_PLL_SHARED3 0x24c
#define PLL_CON3_PLL_SHARED4 0x28c
#define CLK_CON_MUX_CLKCMU_DPU_BUS 0x1000
#define CLK_CON_MUX_MUX_CLKCMU_APM_BUS 0x1004
#define CLK_CON_MUX_MUX_CLKCMU_AUD_CPU 0x1008
#define CLK_CON_MUX_MUX_CLKCMU_BUS0_BUS 0x100c
#define CLK_CON_MUX_MUX_CLKCMU_BUS1_BUS 0x1010
#define CLK_CON_MUX_MUX_CLKCMU_BUS1_SSS 0x1014
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0 0x1018
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1 0x101c
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2 0x1020
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3 0x1024
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4 0x1028
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5 0x102c
#define CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST 0x1030
#define CLK_CON_MUX_MUX_CLKCMU_CORE_BUS 0x1034
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG_BUS 0x1038
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH 0x103c
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH 0x1040
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL2_BUSP 0x1044
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL2_SWITCH 0x1048
#define CLK_CON_MUX_MUX_CLKCMU_CSIS_BUS 0x104c
#define CLK_CON_MUX_MUX_CLKCMU_CSIS_OIS_MCU 0x1050
#define CLK_CON_MUX_MUX_CLKCMU_DNC_BUS 0x1054
#define CLK_CON_MUX_MUX_CLKCMU_DNC_BUSM 0x1058
#define CLK_CON_MUX_MUX_CLKCMU_DNS_BUS 0x105c
#define CLK_CON_MUX_MUX_CLKCMU_DPU 0x1060
#define CLK_CON_MUX_MUX_CLKCMU_DPU_ALT 0x1064
#define CLK_CON_MUX_MUX_CLKCMU_DSP_BUS 0x1068
#define CLK_CON_MUX_MUX_CLKCMU_G2D_G2D 0x106c
#define CLK_CON_MUX_MUX_CLKCMU_G2D_MSCL 0x1070
#define CLK_CON_MUX_MUX_CLKCMU_HPM 0x1074
#define CLK_CON_MUX_MUX_CLKCMU_HSI0_BUS 0x1078
#define CLK_CON_MUX_MUX_CLKCMU_HSI0_DPGTC 0x107c
#define CLK_CON_MUX_MUX_CLKCMU_HSI0_USB31DRD 0x1080
#define CLK_CON_MUX_MUX_CLKCMU_HSI0_USBDP_DEBUG 0x1084
#define CLK_CON_MUX_MUX_CLKCMU_HSI1_BUS 0x1088
#define CLK_CON_MUX_MUX_CLKCMU_HSI1_MMC_CARD 0x108c
#define CLK_CON_MUX_MUX_CLKCMU_HSI1_PCIE 0x1090
#define CLK_CON_MUX_MUX_CLKCMU_HSI1_UFS_CARD 0x1094
#define CLK_CON_MUX_MUX_CLKCMU_HSI1_UFS_EMBD 0x1098
#define CLK_CON_MUX_MUX_CLKCMU_HSI2_BUS 0x109c
#define CLK_CON_MUX_MUX_CLKCMU_HSI2_PCIE 0x10a0
#define CLK_CON_MUX_MUX_CLKCMU_IPP_BUS 0x10a4
#define CLK_CON_MUX_MUX_CLKCMU_ITP_BUS 0x10a8
#define CLK_CON_MUX_MUX_CLKCMU_MCSC_BUS 0x10ac
#define CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC 0x10b0
#define CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST_CPU 0x10b4
#define CLK_CON_MUX_MUX_CLKCMU_MFC0_MFC0 0x10b8
#define CLK_CON_MUX_MUX_CLKCMU_MFC0_WFD 0x10bc
#define CLK_CON_MUX_MUX_CLKCMU_MIF_BUSP 0x10c0
#define CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH 0x10c4
#define CLK_CON_MUX_MUX_CLKCMU_NPU_BUS 0x10c8
#define CLK_CON_MUX_MUX_CLKCMU_PERIC0_BUS 0x10cc
#define CLK_CON_MUX_MUX_CLKCMU_PERIC0_IP 0x10d0
#define CLK_CON_MUX_MUX_CLKCMU_PERIC1_BUS 0x10d4
#define CLK_CON_MUX_MUX_CLKCMU_PERIC1_IP 0x10d8
#define CLK_CON_MUX_MUX_CLKCMU_PERIS_BUS 0x10dc
#define CLK_CON_MUX_MUX_CLKCMU_SSP_BUS 0x10e0
#define CLK_CON_MUX_MUX_CLKCMU_TNR_BUS 0x10e4
#define CLK_CON_MUX_MUX_CLKCMU_VRA_BUS 0x10e8
#define CLK_CON_MUX_MUX_CLK_CMU_CMUREF 0x10f0
#define CLK_CON_MUX_MUX_CMU_CMUREF 0x10f4
#define CLK_CON_DIV_CLKCMU_APM_BUS 0x1800
#define CLK_CON_DIV_CLKCMU_AUD_CPU 0x1804
#define CLK_CON_DIV_CLKCMU_BUS0_BUS 0x1808
#define CLK_CON_DIV_CLKCMU_BUS1_BUS 0x180c
#define CLK_CON_DIV_CLKCMU_BUS1_SSS 0x1810
#define CLK_CON_DIV_CLKCMU_CIS_CLK0 0x1814
#define CLK_CON_DIV_CLKCMU_CIS_CLK1 0x1818
#define CLK_CON_DIV_CLKCMU_CIS_CLK2 0x181c
#define CLK_CON_DIV_CLKCMU_CIS_CLK3 0x1820
#define CLK_CON_DIV_CLKCMU_CIS_CLK4 0x1824
#define CLK_CON_DIV_CLKCMU_CIS_CLK5 0x1828
#define CLK_CON_DIV_CLKCMU_CMU_BOOST 0x182c
#define CLK_CON_DIV_CLKCMU_CORE_BUS 0x1830
#define CLK_CON_DIV_CLKCMU_CPUCL0_DBG_BUS 0x1834
#define CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH 0x1838
#define CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH 0x183c
#define CLK_CON_DIV_CLKCMU_CPUCL2_BUSP 0x1840
#define CLK_CON_DIV_CLKCMU_CPUCL2_SWITCH 0x1844
#define CLK_CON_DIV_CLKCMU_CSIS_BUS 0x1848
#define CLK_CON_DIV_CLKCMU_CSIS_OIS_MCU 0x184c
#define CLK_CON_DIV_CLKCMU_DNC_BUS 0x1850
#define CLK_CON_DIV_CLKCMU_DNC_BUSM 0x1854
#define CLK_CON_DIV_CLKCMU_DNS_BUS 0x1858
#define CLK_CON_DIV_CLKCMU_DSP_BUS 0x185c
#define CLK_CON_DIV_CLKCMU_G2D_G2D 0x1860
#define CLK_CON_DIV_CLKCMU_G2D_MSCL 0x1864
#define CLK_CON_DIV_CLKCMU_G3D_SWITCH 0x1868
#define CLK_CON_DIV_CLKCMU_HPM 0x186c
#define CLK_CON_DIV_CLKCMU_HSI0_BUS 0x1870
#define CLK_CON_DIV_CLKCMU_HSI0_DPGTC 0x1874
#define CLK_CON_DIV_CLKCMU_HSI0_USB31DRD 0x1878
#define CLK_CON_DIV_CLKCMU_HSI0_USBDP_DEBUG 0x187c
#define CLK_CON_DIV_CLKCMU_HSI1_BUS 0x1880
#define CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD 0x1884
#define CLK_CON_DIV_CLKCMU_HSI1_PCIE 0x1888
#define CLK_CON_DIV_CLKCMU_HSI1_UFS_CARD 0x188c
#define CLK_CON_DIV_CLKCMU_HSI1_UFS_EMBD 0x1890
#define CLK_CON_DIV_CLKCMU_HSI2_BUS 0x1894
#define CLK_CON_DIV_CLKCMU_HSI2_PCIE 0x1898
#define CLK_CON_DIV_CLKCMU_IPP_BUS 0x189c
#define CLK_CON_DIV_CLKCMU_ITP_BUS 0x18a0
#define CLK_CON_DIV_CLKCMU_MCSC_BUS 0x18a4
#define CLK_CON_DIV_CLKCMU_MCSC_GDC 0x18a8
#define CLK_CON_DIV_CLKCMU_CMU_BOOST_CPU 0x18ac
#define CLK_CON_DIV_CLKCMU_MFC0_MFC0 0x18b0
#define CLK_CON_DIV_CLKCMU_MFC0_WFD 0x18b4
#define CLK_CON_DIV_CLKCMU_MIF_BUSP 0x18b8
#define CLK_CON_DIV_CLKCMU_NPU_BUS 0x18bc
#define CLK_CON_DIV_CLKCMU_OTP 0x18c0
#define CLK_CON_DIV_CLKCMU_PERIC0_BUS 0x18c4
#define CLK_CON_DIV_CLKCMU_PERIC0_IP 0x18c8
#define CLK_CON_DIV_CLKCMU_PERIC1_BUS 0x18cc
#define CLK_CON_DIV_CLKCMU_PERIC1_IP 0x18d0
#define CLK_CON_DIV_CLKCMU_PERIS_BUS 0x18d4
#define CLK_CON_DIV_CLKCMU_SSP_BUS 0x18d8
#define CLK_CON_DIV_CLKCMU_TNR_BUS 0x18dc
#define CLK_CON_DIV_CLKCMU_VRA_BUS 0x18e0
#define CLK_CON_DIV_DIV_CLKCMU_DPU 0x18e8
#define CLK_CON_DIV_DIV_CLKCMU_DPU_ALT 0x18ec
#define CLK_CON_DIV_DIV_CLK_CMU_CMUREF 0x18f0
#define CLK_CON_DIV_PLL_SHARED0_DIV2 0x18f4
#define CLK_CON_DIV_PLL_SHARED0_DIV3 0x18f8
#define CLK_CON_DIV_PLL_SHARED0_DIV4 0x18fc
#define CLK_CON_DIV_PLL_SHARED1_DIV2 0x1900
#define CLK_CON_DIV_PLL_SHARED1_DIV3 0x1904
#define CLK_CON_DIV_PLL_SHARED1_DIV4 0x1908
#define CLK_CON_DIV_PLL_SHARED2_DIV2 0x190c
#define CLK_CON_DIV_PLL_SHARED4_DIV2 0x1910
#define CLK_CON_DIV_PLL_SHARED4_DIV3 0x1914
#define CLK_CON_DIV_PLL_SHARED4_DIV4 0x1918
#define CLK_CON_GAT_CLKCMU_G3D_BUS 0x2000
#define CLK_CON_GAT_CLKCMU_MIF_SWITCH 0x2004
#define CLK_CON_GAT_GATE_CLKCMU_APM_BUS 0x2008
#define CLK_CON_GAT_GATE_CLKCMU_AUD_CPU 0x200c
#define CLK_CON_GAT_GATE_CLKCMU_BUS0_BUS 0x2010
#define CLK_CON_GAT_GATE_CLKCMU_BUS1_BUS 0x2014
#define CLK_CON_GAT_GATE_CLKCMU_BUS1_SSS 0x2018
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0 0x201c
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1 0x2020
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2 0x2024
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3 0x2028
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4 0x202c
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5 0x2030
#define CLK_CON_GAT_GATE_CLKCMU_CORE_BUS 0x2034
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_BUS 0x2038
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH 0x203c
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH 0x2040
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL2_BUSP 0x2044
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL2_SWITCH 0x2048
#define CLK_CON_GAT_GATE_CLKCMU_CSIS_BUS 0x204c
#define CLK_CON_GAT_GATE_CLKCMU_CSIS_OIS_MCU 0x2050
#define CLK_CON_GAT_GATE_CLKCMU_DNC_BUS 0x2054
#define CLK_CON_GAT_GATE_CLKCMU_DNC_BUSM 0x2058
#define CLK_CON_GAT_GATE_CLKCMU_DNS_BUS 0x205c
#define CLK_CON_GAT_GATE_CLKCMU_DPU 0x2060
#define CLK_CON_GAT_GATE_CLKCMU_DPU_BUS 0x2064
#define CLK_CON_GAT_GATE_CLKCMU_DSP_BUS 0x2068
#define CLK_CON_GAT_GATE_CLKCMU_G2D_G2D 0x206c
#define CLK_CON_GAT_GATE_CLKCMU_G2D_MSCL 0x2070
#define CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH 0x2074
#define CLK_CON_GAT_GATE_CLKCMU_HPM 0x2078
#define CLK_CON_GAT_GATE_CLKCMU_HSI0_BUS 0x207c
#define CLK_CON_GAT_GATE_CLKCMU_HSI0_DPGTC 0x2080
#define CLK_CON_GAT_GATE_CLKCMU_HSI0_USB31DRD 0x2084
#define CLK_CON_GAT_GATE_CLKCMU_HSI0_USBDP_DEBUG 0x2088
#define CLK_CON_GAT_GATE_CLKCMU_HSI1_BUS 0x208c
#define CLK_CON_GAT_GATE_CLKCMU_HSI1_MMC_CARD 0x2090
#define CLK_CON_GAT_GATE_CLKCMU_HSI1_PCIE 0x2094
#define CLK_CON_GAT_GATE_CLKCMU_HSI1_UFS_CARD 0x2098
#define CLK_CON_GAT_GATE_CLKCMU_HSI1_UFS_EMBD 0x209c
#define CLK_CON_GAT_GATE_CLKCMU_HSI2_BUS 0x20a0
#define CLK_CON_GAT_GATE_CLKCMU_HSI2_PCIE 0x20a4
#define CLK_CON_GAT_GATE_CLKCMU_IPP_BUS 0x20a8
#define CLK_CON_GAT_GATE_CLKCMU_ITP_BUS 0x20ac
#define CLK_CON_GAT_GATE_CLKCMU_MCSC_BUS 0x20b0
#define CLK_CON_GAT_GATE_CLKCMU_MCSC_GDC 0x20b4
#define CLK_CON_GAT_GATE_CLKCMU_MFC0_MFC0 0x20bc
#define CLK_CON_GAT_GATE_CLKCMU_MFC0_WFD 0x20c0
#define CLK_CON_GAT_GATE_CLKCMU_MIF_BUSP 0x20c4
#define CLK_CON_GAT_GATE_CLKCMU_NPU_BUS 0x20c8
#define CLK_CON_GAT_GATE_CLKCMU_PERIC0_BUS 0x20cc
#define CLK_CON_GAT_GATE_CLKCMU_PERIC0_IP 0x20d0
#define CLK_CON_GAT_GATE_CLKCMU_PERIC1_BUS 0x20d4
#define CLK_CON_GAT_GATE_CLKCMU_PERIC1_IP 0x20d8
#define CLK_CON_GAT_GATE_CLKCMU_PERIS_BUS 0x20dc
#define CLK_CON_GAT_GATE_CLKCMU_SSP_BUS 0x20e0
#define CLK_CON_GAT_GATE_CLKCMU_TNR_BUS 0x20e4
#define CLK_CON_GAT_GATE_CLKCMU_VRA_BUS 0x20e8

static const unsigned long top_clk_regs[] __initconst = {
	PLL_LOCKTIME_PLL_G3D,
	PLL_LOCKTIME_PLL_MMC,
	PLL_LOCKTIME_PLL_SHARED0,
	PLL_LOCKTIME_PLL_SHARED1,
	PLL_LOCKTIME_PLL_SHARED2,
	PLL_LOCKTIME_PLL_SHARED3,
	PLL_LOCKTIME_PLL_SHARED4,
	PLL_CON3_PLL_G3D,
	PLL_CON3_PLL_MMC,
	PLL_CON5_PLL_MMC,
	PLL_CON3_PLL_SHARED0,
	PLL_CON3_PLL_SHARED1,
	PLL_CON3_PLL_SHARED2,
	PLL_CON3_PLL_SHARED3,
	PLL_CON3_PLL_SHARED4,
	CLK_CON_MUX_MUX_CLKCMU_APM_BUS,
	CLK_CON_MUX_MUX_CLKCMU_AUD_CPU,
	CLK_CON_MUX_MUX_CLKCMU_BUS0_BUS,
	CLK_CON_MUX_MUX_CLKCMU_BUS1_BUS,
	CLK_CON_MUX_MUX_CLKCMU_BUS1_SSS,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5,
	CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST,
	CLK_CON_MUX_MUX_CLKCMU_CORE_BUS,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG_BUS,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL2_BUSP,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL2_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_CSIS_BUS,
	CLK_CON_MUX_MUX_CLKCMU_CSIS_OIS_MCU,
	CLK_CON_MUX_MUX_CLKCMU_DNC_BUS,
	CLK_CON_MUX_MUX_CLKCMU_DNC_BUSM,
	CLK_CON_MUX_MUX_CLKCMU_DNS_BUS,
	CLK_CON_MUX_MUX_CLKCMU_DPU,
	CLK_CON_MUX_MUX_CLKCMU_DPU_ALT,
	CLK_CON_MUX_MUX_CLKCMU_DSP_BUS,
	CLK_CON_MUX_MUX_CLKCMU_G2D_G2D,
	CLK_CON_MUX_MUX_CLKCMU_G2D_MSCL,
	CLK_CON_MUX_MUX_CLKCMU_HPM,
	CLK_CON_MUX_MUX_CLKCMU_HSI0_BUS,
	CLK_CON_MUX_MUX_CLKCMU_HSI0_DPGTC,
	CLK_CON_MUX_MUX_CLKCMU_HSI0_USB31DRD,
	CLK_CON_MUX_MUX_CLKCMU_HSI0_USBDP_DEBUG,
	CLK_CON_MUX_MUX_CLKCMU_HSI1_BUS,
	CLK_CON_MUX_MUX_CLKCMU_HSI1_MMC_CARD,
	CLK_CON_MUX_MUX_CLKCMU_HSI1_PCIE,
	CLK_CON_MUX_MUX_CLKCMU_HSI1_UFS_CARD,
	CLK_CON_MUX_MUX_CLKCMU_HSI1_UFS_EMBD,
	CLK_CON_MUX_MUX_CLKCMU_HSI2_BUS,
	CLK_CON_MUX_MUX_CLKCMU_HSI2_PCIE,
	CLK_CON_MUX_MUX_CLKCMU_IPP_BUS,
	CLK_CON_MUX_MUX_CLKCMU_ITP_BUS,
	CLK_CON_MUX_MUX_CLKCMU_MCSC_BUS,
	CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC,
	CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST_CPU,
	CLK_CON_MUX_MUX_CLKCMU_MFC0_MFC0,
	CLK_CON_MUX_MUX_CLKCMU_MFC0_WFD,
	CLK_CON_MUX_MUX_CLKCMU_MIF_BUSP,
	CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_NPU_BUS,
	CLK_CON_MUX_MUX_CLKCMU_PERIC0_BUS,
	CLK_CON_MUX_MUX_CLKCMU_PERIC0_IP,
	CLK_CON_MUX_MUX_CLKCMU_PERIC1_BUS,
	CLK_CON_MUX_MUX_CLKCMU_PERIC1_IP,
	CLK_CON_MUX_MUX_CLKCMU_PERIS_BUS,
	CLK_CON_MUX_MUX_CLKCMU_SSP_BUS,
	CLK_CON_MUX_MUX_CLKCMU_TNR_BUS,
	CLK_CON_MUX_MUX_CLKCMU_VRA_BUS,
	CLK_CON_MUX_MUX_CLK_CMU_CMUREF,
	CLK_CON_MUX_MUX_CMU_CMUREF,
	CLK_CON_DIV_CLKCMU_APM_BUS,
	CLK_CON_DIV_CLKCMU_AUD_CPU,
	CLK_CON_DIV_CLKCMU_BUS0_BUS,
	CLK_CON_DIV_CLKCMU_BUS1_BUS,
	CLK_CON_DIV_CLKCMU_BUS1_SSS,
	CLK_CON_DIV_CLKCMU_CIS_CLK0,
	CLK_CON_DIV_CLKCMU_CIS_CLK1,
	CLK_CON_DIV_CLKCMU_CIS_CLK2,
	CLK_CON_DIV_CLKCMU_CIS_CLK3,
	CLK_CON_DIV_CLKCMU_CIS_CLK4,
	CLK_CON_DIV_CLKCMU_CIS_CLK5,
	CLK_CON_DIV_CLKCMU_CMU_BOOST,
	CLK_CON_DIV_CLKCMU_CORE_BUS,
	CLK_CON_DIV_CLKCMU_CPUCL0_DBG_BUS,
	CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH,
	CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH,
	CLK_CON_DIV_CLKCMU_CPUCL2_BUSP,
	CLK_CON_DIV_CLKCMU_CPUCL2_SWITCH,
	CLK_CON_DIV_CLKCMU_CSIS_BUS,
	CLK_CON_DIV_CLKCMU_CSIS_OIS_MCU,
	CLK_CON_DIV_CLKCMU_DNC_BUS,
	CLK_CON_DIV_CLKCMU_DNC_BUSM,
	CLK_CON_DIV_CLKCMU_DNS_BUS,
	CLK_CON_DIV_CLKCMU_DSP_BUS,
	CLK_CON_DIV_CLKCMU_G2D_G2D,
	CLK_CON_DIV_CLKCMU_G2D_MSCL,
	CLK_CON_DIV_CLKCMU_G3D_SWITCH,
	CLK_CON_DIV_CLKCMU_HPM,
	CLK_CON_DIV_CLKCMU_HSI0_BUS,
	CLK_CON_DIV_CLKCMU_HSI0_DPGTC,
	CLK_CON_DIV_CLKCMU_HSI0_USB31DRD,
	CLK_CON_DIV_CLKCMU_HSI0_USBDP_DEBUG,
	CLK_CON_DIV_CLKCMU_HSI1_BUS,
	CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD,
	CLK_CON_DIV_CLKCMU_HSI1_PCIE,
	CLK_CON_DIV_CLKCMU_HSI1_UFS_CARD,
	CLK_CON_DIV_CLKCMU_HSI1_UFS_EMBD,
	CLK_CON_DIV_CLKCMU_HSI2_BUS,
	CLK_CON_DIV_CLKCMU_HSI2_PCIE,
	CLK_CON_DIV_CLKCMU_IPP_BUS,
	CLK_CON_DIV_CLKCMU_ITP_BUS,
	CLK_CON_DIV_CLKCMU_MCSC_BUS,
	CLK_CON_DIV_CLKCMU_MCSC_GDC,
	CLK_CON_DIV_CLKCMU_CMU_BOOST_CPU,
	CLK_CON_DIV_CLKCMU_MFC0_MFC0,
	CLK_CON_DIV_CLKCMU_MFC0_WFD,
	CLK_CON_DIV_CLKCMU_MIF_BUSP,
	CLK_CON_DIV_CLKCMU_NPU_BUS,
	CLK_CON_DIV_CLKCMU_OTP,
	CLK_CON_DIV_CLKCMU_PERIC0_BUS,
	CLK_CON_DIV_CLKCMU_PERIC0_IP,
	CLK_CON_DIV_CLKCMU_PERIC1_BUS,
	CLK_CON_DIV_CLKCMU_PERIC1_IP,
	CLK_CON_DIV_CLKCMU_PERIS_BUS,
	CLK_CON_DIV_CLKCMU_SSP_BUS,
	CLK_CON_DIV_CLKCMU_TNR_BUS,
	CLK_CON_DIV_CLKCMU_VRA_BUS,
	CLK_CON_DIV_DIV_CLKCMU_DPU,
	CLK_CON_DIV_DIV_CLKCMU_DPU_ALT,
	CLK_CON_DIV_DIV_CLK_CMU_CMUREF,
	CLK_CON_DIV_PLL_SHARED0_DIV2,
	CLK_CON_DIV_PLL_SHARED0_DIV3,
	CLK_CON_DIV_PLL_SHARED0_DIV4,
	CLK_CON_DIV_PLL_SHARED1_DIV2,
	CLK_CON_DIV_PLL_SHARED1_DIV3,
	CLK_CON_DIV_PLL_SHARED1_DIV4,
	CLK_CON_DIV_PLL_SHARED2_DIV2,
	CLK_CON_DIV_PLL_SHARED4_DIV2,
	CLK_CON_DIV_PLL_SHARED4_DIV3,
	CLK_CON_DIV_PLL_SHARED4_DIV4,
	CLK_CON_GAT_CLKCMU_G3D_BUS,
	CLK_CON_GAT_CLKCMU_MIF_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_APM_BUS,
	CLK_CON_GAT_GATE_CLKCMU_AUD_CPU,
	CLK_CON_GAT_GATE_CLKCMU_BUS0_BUS,
	CLK_CON_GAT_GATE_CLKCMU_BUS1_BUS,
	CLK_CON_GAT_GATE_CLKCMU_BUS1_SSS,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5,
	CLK_CON_GAT_GATE_CLKCMU_CORE_BUS,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_BUS,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL2_BUSP,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL2_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_CSIS_BUS,
	CLK_CON_GAT_GATE_CLKCMU_CSIS_OIS_MCU,
	CLK_CON_GAT_GATE_CLKCMU_DNC_BUS,
	CLK_CON_GAT_GATE_CLKCMU_DNC_BUSM,
	CLK_CON_GAT_GATE_CLKCMU_DNS_BUS,
	CLK_CON_GAT_GATE_CLKCMU_DPU,
	CLK_CON_GAT_GATE_CLKCMU_DPU_BUS,
	CLK_CON_GAT_GATE_CLKCMU_DSP_BUS,
	CLK_CON_GAT_GATE_CLKCMU_G2D_G2D,
	CLK_CON_GAT_GATE_CLKCMU_G2D_MSCL,
	CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_HPM,
	CLK_CON_GAT_GATE_CLKCMU_HSI0_BUS,
	CLK_CON_GAT_GATE_CLKCMU_HSI0_DPGTC,
	CLK_CON_GAT_GATE_CLKCMU_HSI0_USB31DRD,
	CLK_CON_GAT_GATE_CLKCMU_HSI0_USBDP_DEBUG,
	CLK_CON_GAT_GATE_CLKCMU_HSI1_BUS,
	CLK_CON_GAT_GATE_CLKCMU_HSI1_MMC_CARD,
	CLK_CON_GAT_GATE_CLKCMU_HSI1_PCIE,
	CLK_CON_GAT_GATE_CLKCMU_HSI1_UFS_CARD,
	CLK_CON_GAT_GATE_CLKCMU_HSI1_UFS_EMBD,
	CLK_CON_GAT_GATE_CLKCMU_HSI2_BUS,
	CLK_CON_GAT_GATE_CLKCMU_HSI2_PCIE,
	CLK_CON_GAT_GATE_CLKCMU_IPP_BUS,
	CLK_CON_GAT_GATE_CLKCMU_ITP_BUS,
	CLK_CON_GAT_GATE_CLKCMU_MCSC_BUS,
	CLK_CON_GAT_GATE_CLKCMU_MCSC_GDC,
	CLK_CON_GAT_GATE_CLKCMU_MFC0_MFC0,
	CLK_CON_GAT_GATE_CLKCMU_MFC0_WFD,
	CLK_CON_GAT_GATE_CLKCMU_MIF_BUSP,
	CLK_CON_GAT_GATE_CLKCMU_NPU_BUS,
	CLK_CON_GAT_GATE_CLKCMU_PERIC0_BUS,
	CLK_CON_GAT_GATE_CLKCMU_PERIC0_IP,
	CLK_CON_GAT_GATE_CLKCMU_PERIC1_BUS,
	CLK_CON_GAT_GATE_CLKCMU_PERIC1_IP,
	CLK_CON_GAT_GATE_CLKCMU_PERIS_BUS,
	CLK_CON_GAT_GATE_CLKCMU_SSP_BUS,
	CLK_CON_GAT_GATE_CLKCMU_TNR_BUS,
	CLK_CON_GAT_GATE_CLKCMU_VRA_BUS,
};

/* Parents definition for top (shared) muxes*/
PNAME(mout_clkcmu_apm_bus_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_aud_cpu_p) = { "fout_shared0_pll", "fout_shared2_pll",
				 "fout_shared4_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_bus0_bus_p) = { "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_bus1_bus_p) = { "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_bus1_sss_p) = { "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_cis_clk0_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_cis_clk1_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_cis_clk2_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_cis_clk3_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_cis_clk4_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_cis_clk5_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_cmu_boost_p) = { "fout_shared0_pll", "fout_shared1_pll",
				   "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_core_bus_p) = { "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared2_pll", "fout_shared0_pll",
				  "fout_shared1_pll", "fout_shared0_pll",
				  "fout_shared3_pll", "oscclk" };
PNAME(mout_clkcmu_cpucl0_dbg_bus_p) = { "fout_shared2_pll", "fout_shared0_pll",
					"fout_shared0_pll", "oscclk" };
PNAME(mout_clkcmu_cpucl0_switch_p) = { "fout_shared4_pll", "fout_shared0_pll",
				       "fout_shared2_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_cpucl1_switch_p) = { "fout_shared4_pll", "fout_shared0_pll",
				       "fout_shared2_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_cpucl2_busp_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_cpucl2_switch_p) = { "fout_shared4_pll", "fout_shared0_pll",
				       "fout_shared2_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_csis_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				  "fout_shared0_pll", "fout_shared4_pll" };
PNAME(mout_clkcmu_csis_ois_mcu_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_dnc_bus_p) = { "fout_shared1_pll", "fout_shared2_pll",
				 "fout_shared4_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_dnc_busm_p) = { "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared2_pll", "fout_shared4_pll" };
PNAME(mout_clkcmu_dns_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				 "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared4_pll", "fout_shared2_pll",
				 "oscclk",	     "oscclk" };
PNAME(mout_clkcmu_dpu_p) = { "fout_shared0_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_dpu_alt_p) = { "fout_shared4_pll", "fout_shared4_pll",
				 "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_dsp_bus_p) = { "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared2_pll", "fout_shared4_pll",
				 "fout_shared3_pll", "oscclk",
				 "oscclk",	     "oscclk" };
PNAME(mout_clkcmu_g2d_g2d_p) = { "fout_shared0_pll", "fout_shared4_pll",
				 "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_g2d_mscl_p) = { "fout_shared0_pll", "fout_shared2_pll",
				  "fout_shared4_pll", "oscclk" };
PNAME(mout_clkcmu_hpm_p) = { "oscclk", "dout_shared0_div4", "dout_shared2_div2",
			     "oscclk" };
PNAME(mout_clkcmu_hsi0_bus_p) = { "dout_shared0_div4", "dout_shared2_div2" };
PNAME(mout_clkcmu_hsi0_dpgtc_p) = { "oscclk", "dout_shared0_div4",
				    "dout_shared2_div2", "oscclk" };
PNAME(mout_clkcmu_hsi0_usb31drd_p) = { "oscclk", "dout_shared0_div4",
				       "dout_shared2_div2", "oscclk" };
PNAME(mout_clkcmu_hsi0_usbdp_debug_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_hsi1_bus_p) = { "dout_shared0_div3", "dout_shared0_div4",
				  "dout_shared1_div4", "dout_shared4_div2",
				  "dout_shared2_div2", "fout_mmc_pll",
				  "oscclk",	      "oscclk" };
PNAME(mout_clkcmu_hsi1_mmc_card_p) = { "oscclk", "fout_shared2_pll",
				       "fout_mmc_pll", "fout_shared0_pll" };
PNAME(mout_clkcmu_hsi1_pcie_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_hsi1_ufs_card_p) = { "oscclk", "fout_shared0_pll",
				       "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_hsi1_ufs_embd_p) = { "oscclk", "dout_shared0_div4",
				       "dout_shared2_div2", "oscclk" };
PNAME(mout_clkcmu_hsi2_bus_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_hsi2_pcie_p) = { "oscclk", "fout_shared2_pll" };
PNAME(mout_clkcmu_ipp_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				 "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared4_pll", "oscclk",
				 "oscclk",	     "oscclk" };
PNAME(mout_clkcmu_itp_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				 "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared4_pll", "fout_shared2_pll",
				 "oscclk",	     "oscclk" };
PNAME(mout_clkcmu_mcsc_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				  "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared4_pll", "fout_shared2_pll",
				  "oscclk",	      "oscclk" };
PNAME(mout_clkcmu_mcsc_gdc_p) = { "fout_shared0_pll", "fout_shared4_pll",
				  "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared4_pll", "fout_shared2_pll",
				  "oscclk",	      "oscclk" };
PNAME(mout_clkcmu_cmu_boost_cpu_p) = { "fout_shared0_pll", "fout_shared1_pll",
				       "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_mfc0_mfc0_p) = { "fout_shared4_pll", "fout_shared0_pll",
				   "fout_shared4_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_mfc0_wfd_p) = { "fout_shared4_pll", "fout_shared0_pll",
				  "fout_shared4_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_mif_busp_p) = { "fout_shared0_pll", "fout_shared1_pll",
				  "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_mif_switch_p) = { "fout_shared0_pll", "fout_shared1_pll",
				    "fout_shared0_pll", "fout_shared1_pll",
				    "fout_shared2_pll", "fout_shared0_pll",
				    "fout_shared2_pll", "oscclk" };
PNAME(mout_clkcmu_npu_bus_p) = { "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared2_pll", "fout_shared4_pll",
				 "fout_shared3_pll", "oscclk",
				 "oscclk",	     "oscclk" };
PNAME(mout_clkcmu_peric0_bus_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_peric0_ip_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_peric1_bus_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_peric1_ip_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_peris_bus_p) = { "fout_shared0_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_ssp_bus_p) = { "fout_shared4_pll", "fout_shared0_pll",
				 "fout_shared4_pll", "fout_shared2_pll" };
PNAME(mout_clkcmu_tnr_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				 "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared4_pll", "fout_shared2_pll",
				 "oscclk",	     "oscclk" };
PNAME(mout_clkcmu_vra_bus_p) = { "fout_shared0_pll", "fout_shared4_pll",
				 "fout_shared0_pll", "fout_shared4_pll" };
PNAME(mout_clk_cmu_cmuref_p) = { "fout_shared0_pll", "fout_shared1_pll",
				 "fout_shared2_pll", "oscclk" };
PNAME(mout_cmu_cmuref_p) = { "oscclk", "dout_clk_cmu_cmuref" };

static const struct samsung_pll_clock top_pll_clks[] __initconst = {
	PLL(pll_0717x, CLK_FOUT_SHARED0_PLL, "fout_shared0_pll", "oscclk",
	    PLL_LOCKTIME_PLL_SHARED0, PLL_CON3_PLL_SHARED0, NULL),
	PLL(pll_0717x, CLK_FOUT_SHARED1_PLL, "fout_shared1_pll", "oscclk",
	    PLL_LOCKTIME_PLL_SHARED1, PLL_CON3_PLL_SHARED1, NULL),
	PLL(pll_0718x, CLK_FOUT_SHARED2_PLL, "fout_shared2_pll", "oscclk",
	    PLL_LOCKTIME_PLL_SHARED2, PLL_CON3_PLL_SHARED2, NULL),
	PLL(pll_0718x, CLK_FOUT_SHARED3_PLL, "fout_shared3_pll", "oscclk",
	    PLL_LOCKTIME_PLL_SHARED3, PLL_CON3_PLL_SHARED3, NULL),
	PLL(pll_0717x, CLK_FOUT_SHARED4_PLL, "fout_shared4_pll", "oscclk",
	    PLL_LOCKTIME_PLL_SHARED4, PLL_CON3_PLL_SHARED4, NULL),
	PLL(pll_0732x, CLK_FOUT_MMC_PLL, "fout_mmc_pll", "oscclk",
	    PLL_LOCKTIME_PLL_MMC, PLL_CON3_PLL_MMC, NULL),
};

static const struct samsung_mux_clock top_mux_clks[] __initconst = {
	MUX(CLK_MOUT_CLKCMU_APM_BUS, "mout_clkcmu_apm_bus",
	    mout_clkcmu_apm_bus_p, CLK_CON_MUX_MUX_CLKCMU_APM_BUS, 0, 1),
	MUX(CLK_MOUT_CLKCMU_AUD_CPU, "mout_clkcmu_aud_cpu",
	    mout_clkcmu_aud_cpu_p, CLK_CON_MUX_MUX_CLKCMU_AUD_CPU, 0, 2),
	MUX(CLK_MOUT_CLKCMU_BUS0_BUS, "mout_clkcmu_bus0_bus",
	    mout_clkcmu_bus0_bus_p, CLK_CON_MUX_MUX_CLKCMU_BUS0_BUS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_BUS1_BUS, "mout_clkcmu_bus1_bus",
	    mout_clkcmu_bus1_bus_p, CLK_CON_MUX_MUX_CLKCMU_BUS1_BUS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_BUS1_SSS, "mout_clkcmu_bus1_sss",
	    mout_clkcmu_bus1_sss_p, CLK_CON_MUX_MUX_CLKCMU_BUS1_SSS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_CIS_CLK0, "mout_clkcmu_cis_clk0",
	    mout_clkcmu_cis_clk0_p, CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0, 0, 1),
	MUX(CLK_MOUT_CLKCMU_CIS_CLK1, "mout_clkcmu_cis_clk1",
	    mout_clkcmu_cis_clk1_p, CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1, 0, 1),
	MUX(CLK_MOUT_CLKCMU_CIS_CLK2, "mout_clkcmu_cis_clk2",
	    mout_clkcmu_cis_clk2_p, CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2, 0, 1),
	MUX(CLK_MOUT_CLKCMU_CIS_CLK3, "mout_clkcmu_cis_clk3",
	    mout_clkcmu_cis_clk3_p, CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3, 0, 1),
	MUX(CLK_MOUT_CLKCMU_CIS_CLK4, "mout_clkcmu_cis_clk4",
	    mout_clkcmu_cis_clk4_p, CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4, 0, 1),
	MUX(CLK_MOUT_CLKCMU_CIS_CLK5, "mout_clkcmu_cis_clk5",
	    mout_clkcmu_cis_clk5_p, CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5, 0, 1),
	MUX(CLK_MOUT_CLKCMU_CMU_BOOST, "mout_clkcmu_cmu_boost",
	    mout_clkcmu_cmu_boost_p, CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST, 0, 2),
	MUX(CLK_MOUT_CLKCMU_CORE_BUS, "mout_clkcmu_core_bus",
	    mout_clkcmu_core_bus_p, CLK_CON_MUX_MUX_CLKCMU_CORE_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_CPUCL0_DBG_BUS, "mout_clkcmu_cpucl0_dbg_bus",
	    mout_clkcmu_cpucl0_dbg_bus_p, CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG_BUS,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_CPUCL0_SWITCH, "mout_clkcmu_cpucl0_switch",
	    mout_clkcmu_cpucl0_switch_p, CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_CPUCL1_SWITCH, "mout_clkcmu_cpucl1_switch",
	    mout_clkcmu_cpucl1_switch_p, CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_CPUCL2_BUSP, "mout_clkcmu_cpucl2_busp",
	    mout_clkcmu_cpucl2_busp_p, CLK_CON_MUX_MUX_CLKCMU_CPUCL2_BUSP, 0,
	    1),
	MUX(CLK_MOUT_CLKCMU_CPUCL2_SWITCH, "mout_clkcmu_cpucl2_switch",
	    mout_clkcmu_cpucl2_switch_p, CLK_CON_MUX_MUX_CLKCMU_CPUCL2_SWITCH,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_CSIS_BUS, "mout_clkcmu_csis_bus",
	    mout_clkcmu_csis_bus_p, CLK_CON_MUX_MUX_CLKCMU_CSIS_BUS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_CSIS_OIS_MCU, "mout_clkcmu_csis_ois_mcu",
	    mout_clkcmu_csis_ois_mcu_p, CLK_CON_MUX_MUX_CLKCMU_CSIS_OIS_MCU, 0,
	    1),
	MUX(CLK_MOUT_CLKCMU_DNC_BUS, "mout_clkcmu_dnc_bus",
	    mout_clkcmu_dnc_bus_p, CLK_CON_MUX_MUX_CLKCMU_DNC_BUS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_DNC_BUSM, "mout_clkcmu_dnc_busm",
	    mout_clkcmu_dnc_busm_p, CLK_CON_MUX_MUX_CLKCMU_DNC_BUSM, 0, 2),
	MUX(CLK_MOUT_CLKCMU_DNS_BUS, "mout_clkcmu_dns_bus",
	    mout_clkcmu_dns_bus_p, CLK_CON_MUX_MUX_CLKCMU_DNS_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_DPU, "mout_clkcmu_dpu", mout_clkcmu_dpu_p,
	    CLK_CON_MUX_MUX_CLKCMU_DPU, 0, 1),
	MUX(CLK_MOUT_CLKCMU_DPU_ALT, "mout_clkcmu_dpu_alt",
	    mout_clkcmu_dpu_alt_p, CLK_CON_MUX_MUX_CLKCMU_DPU_ALT, 0, 2),
	MUX(CLK_MOUT_CLKCMU_DSP_BUS, "mout_clkcmu_dsp_bus",
	    mout_clkcmu_dsp_bus_p, CLK_CON_MUX_MUX_CLKCMU_DSP_BUS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_G2D_G2D, "mout_clkcmu_g2d_g2d",
	    mout_clkcmu_g2d_g2d_p, CLK_CON_MUX_MUX_CLKCMU_G2D_G2D, 0, 2),
	MUX(CLK_MOUT_CLKCMU_G2D_MSCL, "mout_clkcmu_g2d_mscl",
	    mout_clkcmu_g2d_mscl_p, CLK_CON_MUX_MUX_CLKCMU_G2D_MSCL, 0, 1),
	MUX(CLK_MOUT_CLKCMU_HPM, "mout_clkcmu_hpm", mout_clkcmu_hpm_p,
	    CLK_CON_MUX_MUX_CLKCMU_HPM, 0, 2),
	MUX(CLK_MOUT_CLKCMU_HSI0_BUS, "mout_clkcmu_hsi0_bus",
	    mout_clkcmu_hsi0_bus_p, CLK_CON_MUX_MUX_CLKCMU_HSI0_BUS, 0, 1),
	MUX(CLK_MOUT_CLKCMU_HSI0_DPGTC, "mout_clkcmu_hsi0_dpgtc",
	    mout_clkcmu_hsi0_dpgtc_p, CLK_CON_MUX_MUX_CLKCMU_HSI0_DPGTC, 0, 2),
	MUX(CLK_MOUT_CLKCMU_HSI0_USB31DRD, "mout_clkcmu_hsi0_usb31drd",
	    mout_clkcmu_hsi0_usb31drd_p, CLK_CON_MUX_MUX_CLKCMU_HSI0_USB31DRD,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_HSI0_USBDP_DEBUG, "mout_clkcmu_hsi0_usbdp_debug",
	    mout_clkcmu_hsi0_usbdp_debug_p,
	    CLK_CON_MUX_MUX_CLKCMU_HSI0_USBDP_DEBUG, 0, 1),
	MUX(CLK_MOUT_CLKCMU_HSI1_BUS, "mout_clkcmu_hsi1_bus",
	    mout_clkcmu_hsi1_bus_p, CLK_CON_MUX_MUX_CLKCMU_HSI1_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_HSI1_MMC_CARD, "mout_clkcmu_hsi1_mmc_card",
	    mout_clkcmu_hsi1_mmc_card_p, CLK_CON_MUX_MUX_CLKCMU_HSI1_MMC_CARD,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_HSI1_PCIE, "mout_clkcmu_hsi1_pcie",
	    mout_clkcmu_hsi1_pcie_p, CLK_CON_MUX_MUX_CLKCMU_HSI1_PCIE, 0, 1),
	MUX(CLK_MOUT_CLKCMU_HSI1_UFS_CARD, "mout_clkcmu_hsi1_ufs_card",
	    mout_clkcmu_hsi1_ufs_card_p, CLK_CON_MUX_MUX_CLKCMU_HSI1_UFS_CARD,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_HSI1_UFS_EMBD, "mout_clkcmu_hsi1_ufs_embd",
	    mout_clkcmu_hsi1_ufs_embd_p, CLK_CON_MUX_MUX_CLKCMU_HSI1_UFS_EMBD,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_HSI2_BUS, "mout_clkcmu_hsi2_bus",
	    mout_clkcmu_hsi2_bus_p, CLK_CON_MUX_MUX_CLKCMU_HSI2_BUS, 0, 1),
	MUX(CLK_MOUT_CLKCMU_HSI2_PCIE, "mout_clkcmu_hsi2_pcie",
	    mout_clkcmu_hsi2_pcie_p, CLK_CON_MUX_MUX_CLKCMU_HSI2_PCIE, 0, 1),
	MUX(CLK_MOUT_CLKCMU_IPP_BUS, "mout_clkcmu_ipp_bus",
	    mout_clkcmu_ipp_bus_p, CLK_CON_MUX_MUX_CLKCMU_IPP_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_ITP_BUS, "mout_clkcmu_itp_bus",
	    mout_clkcmu_itp_bus_p, CLK_CON_MUX_MUX_CLKCMU_ITP_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_MCSC_BUS, "mout_clkcmu_mcsc_bus",
	    mout_clkcmu_mcsc_bus_p, CLK_CON_MUX_MUX_CLKCMU_MCSC_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_MCSC_GDC, "mout_clkc0u_mcsc_gdc",
	    mout_clkcmu_mcsc_gdc_p, CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC, 0, 3),
	MUX(CLK_MOUT_CLKCMU_CMU_BOOST_CPU, "mout_clkcmu_cmu_boost_cpu",
	    mout_clkcmu_cmu_boost_cpu_p, CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST_CPU,
	    0, 2),
	MUX(CLK_MOUT_CLKCMU_MFC0_MFC0, "mout_clkcmu_mfc0_mfc0",
	    mout_clkcmu_mfc0_mfc0_p, CLK_CON_MUX_MUX_CLKCMU_MFC0_MFC0, 0, 2),
	MUX(CLK_MOUT_CLKCMU_MFC0_WFD, "mout_clkcmu_mfc0_wfd",
	    mout_clkcmu_mfc0_wfd_p, CLK_CON_MUX_MUX_CLKCMU_MFC0_WFD, 0, 2),
	MUX(CLK_MOUT_CLKCMU_MIF_BUSP, "mout_clkcmu_mif_busp",
	    mout_clkcmu_mif_busp_p, CLK_CON_MUX_MUX_CLKCMU_MIF_BUSP, 0, 2),
	MUX(CLK_MOUT_CLKCMU_MIF_SWITCH, "mout_clkcmu_mif_switch",
	    mout_clkcmu_mif_switch_p, CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH, 0, 3),
	MUX(CLK_MOUT_CLKCMU_NPU_BUS, "mout_clkcmu_npu_bus",
	    mout_clkcmu_npu_bus_p, CLK_CON_MUX_MUX_CLKCMU_NPU_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_PERIC0_BUS, "mout_clkcmu_peric0_bus",
	    mout_clkcmu_peric0_bus_p, CLK_CON_MUX_MUX_CLKCMU_PERIC0_BUS, 0, 1),
	MUX(CLK_MOUT_CLKCMU_PERIC0_IP, "mout_clkcmu_peric0_ip",
	    mout_clkcmu_peric0_ip_p, CLK_CON_MUX_MUX_CLKCMU_PERIC0_IP, 0, 1),
	MUX(CLK_MOUT_CLKCMU_PERIC1_BUS, "mout_clkcmu_peric1_bus",
	    mout_clkcmu_peric1_bus_p, CLK_CON_MUX_MUX_CLKCMU_PERIC1_BUS, 0, 1),
	MUX(CLK_MOUT_CLKCMU_PERIC1_IP, "mout_clkcmu_peric1_ip",
	    mout_clkcmu_peric1_ip_p, CLK_CON_MUX_MUX_CLKCMU_PERIC1_IP, 0, 1),
	MUX(CLK_MOUT_CLKCMU_PERIS_BUS, "mout_clkcmu_peris_bus",
	    mout_clkcmu_peris_bus_p, CLK_CON_MUX_MUX_CLKCMU_PERIS_BUS, 0, 1),
	MUX(CLK_MOUT_CLKCMU_SSP_BUS, "mout_clkcmu_ssp_bus",
	    mout_clkcmu_ssp_bus_p, CLK_CON_MUX_MUX_CLKCMU_SSP_BUS, 0, 2),
	MUX(CLK_MOUT_CLKCMU_TNR_BUS, "mout_clkcmu_tnr_bus",
	    mout_clkcmu_tnr_bus_p, CLK_CON_MUX_MUX_CLKCMU_TNR_BUS, 0, 3),
	MUX(CLK_MOUT_CLKCMU_VRA_BUS, "mout_clkcmu_vra_bus",
	    mout_clkcmu_vra_bus_p, CLK_CON_MUX_MUX_CLKCMU_VRA_BUS, 0, 2),
	MUX(CLK_MOUT_CLK_CMU_CMUREF, "mout_clk_cmu_cmuref",
	    mout_clk_cmu_cmuref_p, CLK_CON_MUX_MUX_CLK_CMU_CMUREF, 0, 2),
	MUX(CLK_MOUT_CMU_CMUREF, "mout_cmu_cmuref", mout_cmu_cmuref_p,
	    CLK_CON_MUX_MUX_CMU_CMUREF, 0, 1),
};

static const struct samsung_div_clock top_div_clks[] __initconst = {
	/* SHARED0 region*/
	DIV(CLK_DOUT_SHARED0_DIV2, "dout_shared0_div2", "fout_shared0_pll",
	    CLK_CON_DIV_PLL_SHARED0_DIV2, 0, 1),
	DIV(CLK_DOUT_SHARED0_DIV3, "dout_shared0_div3", "fout_shared0_pll",
	    CLK_CON_DIV_PLL_SHARED0_DIV3, 0, 2),
	DIV(CLK_DOUT_SHARED0_DIV4, "dout_shared0_div4", "dout_shared0_div2",
	    CLK_CON_DIV_PLL_SHARED0_DIV4, 0, 1),

	/* SHARED1 region*/
	DIV(CLK_DOUT_SHARED1_DIV2, "dout_shared1_div2", "fout_shared1_pll",
	    CLK_CON_DIV_PLL_SHARED1_DIV2, 0, 1),
	DIV(CLK_DOUT_SHARED1_DIV3, "dout_shared1_div3", "fout_shared1_pll",
	    CLK_CON_DIV_PLL_SHARED1_DIV3, 0, 2),
	DIV(CLK_DOUT_SHARED1_DIV4, "dout_shared1_div4", "dout_shared1_div2",
	    CLK_CON_DIV_PLL_SHARED1_DIV4, 0, 1),

	/* SHARED2 region */
	DIV(CLK_DOUT_SHARED2_DIV2, "dout_shared2_div2", "fout_shared2_pll",
	    CLK_CON_DIV_PLL_SHARED2_DIV2, 0, 1),

	/* SHARED4 region*/
	DIV(CLK_DOUT_SHARED4_DIV2, "dout_shared4_div2", "fout_shared4_pll",
	    CLK_CON_DIV_PLL_SHARED4_DIV2, 0, 1),
	DIV(CLK_DOUT_SHARED4_DIV3, "dout_shared4_div3", "fout_shared4_pll",
	    CLK_CON_DIV_PLL_SHARED4_DIV3, 0, 2),
	DIV(CLK_DOUT_SHARED4_DIV4, "dout_shared4_div4", "fout_shared4_pll",
	    CLK_CON_DIV_PLL_SHARED4_DIV4, 0, 1),

	/* APM_BUS */
	DIV(CLK_DOUT_APM_BUS, "dout_apm_bus", "gout_clkcmu_apm_bus",
	    CLK_CON_DIV_CLKCMU_APM_BUS, 0, 2),

	/* AUD_CPU */
	DIV(CLK_DOUT_AUD_CPU, "dout_aud_cpu", "gout_clkcmu_aud_cpu",
	    CLK_CON_DIV_CLKCMU_AUD_CPU, 0, 3),

	/* BUS0 */
	DIV(CLK_DOUT_BUS0_BUS, "dout_bus0_bus", "gout_clkcmu_bus0_bus",
	    CLK_CON_DIV_CLKCMU_BUS0_BUS, 0, 4),

	/* BUS1 */
	DIV(CLK_DOUT_BUS1_BUS, "dout_bus1_bus", "gout_clkcmu_bus1_bus",
	    CLK_CON_DIV_CLKCMU_BUS1_BUS, 0, 4),
	DIV(CLK_DOUT_BUS1_SSS, "dout_bus1_sss", "gout_clkcmu_bus1_sss",
	    CLK_CON_DIV_CLKCMU_BUS1_SSS, 0, 4),

	/* CIS */
	DIV(CLK_DOUT_CIS_CLK0, "dout_cis_clk0", "gout_clkcmu_cis_clk0",
	    CLK_CON_DIV_CLKCMU_CIS_CLK0, 0, 5),
	DIV(CLK_DOUT_CIS_CLK1, "dout_cis_clk1", "gout_clkcmu_cis_clk1",
	    CLK_CON_DIV_CLKCMU_CIS_CLK1, 0, 5),
	DIV(CLK_DOUT_CIS_CLK2, "dout_cis_clk2", "gout_clkcmu_cis_clk2",
	    CLK_CON_DIV_CLKCMU_CIS_CLK2, 0, 5),
	DIV(CLK_DOUT_CIS_CLK3, "dout_cis_clk3", "gout_clkcmu_cis_clk3",
	    CLK_CON_DIV_CLKCMU_CIS_CLK3, 0, 5),
	DIV(CLK_DOUT_CIS_CLK4, "dout_cis_clk4", "gout_clkcmu_cis_clk4",
	    CLK_CON_DIV_CLKCMU_CIS_CLK4, 0, 5),
	DIV(CLK_DOUT_CIS_CLK5, "dout_cis_clk5", "gout_clkcmu_cis_clk5",
	    CLK_CON_DIV_CLKCMU_CIS_CLK5, 0, 5),

	/* CMU_BOOST */
	DIV(CLK_DOUT_CMU_BOOST, "dout_cmu_boost", "mout_clkcmu_cmu_boost",
	    CLK_CON_DIV_CLKCMU_CMU_BOOST, 0, 2),

	/* CORE_BUS */
	DIV(CLK_DOUT_CORE_BUS, "dout_core_bus", "gout_clkcmu_core_bus",
	    CLK_CON_DIV_CLKCMU_CORE_BUS, 0, 4),

	/* CPUCL0 */
	DIV(CLK_DOUT_CPUCL0_DBG, "dout_cpucl0_debug",
	    "gout_clkcmu_cpucl0_dbg_bus", CLK_CON_DIV_CLKCMU_CPUCL0_DBG_BUS, 0,
	    4),
	DIV(CLK_DOUT_CPUCL0_SWITCH, "dout_cpucl0_switch",
	    "gout_clkcmu_cpucl0_switch", CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH, 0,
	    3),

	/* CPUCL1 */
	DIV(CLK_DOUT_CPUCL1_SWITCH, "dout_cpucl1_switch",
	    "gout_clkcmu_cpucl1_switch", CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH, 0,
	    3),

	/* CPUCL2 */
	DIV(CLK_DOUT_CPUCL2_BUSP, "dout_cpucl2_busp", "gout_clkcmu_cpucl2_busp",
	    CLK_CON_DIV_CLKCMU_CPUCL2_BUSP, 0, 4),
	DIV(CLK_DOUT_CPUCL2_SWITCH, "dout_cpucl2_switch",
	    "gout_clkcmu_cpucl2_switch", CLK_CON_DIV_CLKCMU_CPUCL2_SWITCH, 0,
	    3),

	/* CSIS */
	DIV(CLK_DOUT_CSIS_BUS, "dout_csis_bus", "gout_clkcmu_csis_bus",
	    CLK_CON_DIV_CLKCMU_CSIS_BUS, 0, 4),
	DIV(CLK_DOUT_CSIS_OIS_MCU, "dout_csis_ois_mcu",
	    "gout_clkcmu_csis_ois_mcu", CLK_CON_DIV_CLKCMU_CSIS_OIS_MCU, 0, 4),

	/* DNC_BUS */
	DIV(CLK_DOUT_DNC_BUS, "dout_dnc_bus", "gout_clkcmu_dnc_bus",
	    CLK_CON_DIV_CLKCMU_DNC_BUS, 0, 4),
	DIV(CLK_DOUT_DNC_BUSM, "dout_dnc_busm", "gout_clkcmu_dnc_busm",
	    CLK_CON_DIV_CLKCMU_DNC_BUSM, 0, 4),
	DIV(CLK_DOUT_DNS_BUS, "dout_dns_bus", "gout_clkcmu_dns_bus",
	    CLK_CON_DIV_CLKCMU_DNS_BUS, 0, 4),
	DIV(CLK_DOUT_DSP_BUS, "dout_dsp_bus", "gout_clkcmu_dsp_bus",
	    CLK_CON_DIV_CLKCMU_DSP_BUS, 0, 4),
	DIV(CLK_DOUT_G2D_G2D, "dout_g2d_g2d", "gout_clkcmu_g2d_g2d",
	    CLK_CON_DIV_CLKCMU_G2D_G2D, 0, 4),
	DIV(CLK_DOUT_G2D_MSCL, "dout_g2d_mscl", "gout_clkcmu_g2d_mscl",
	    CLK_CON_DIV_CLKCMU_G2D_MSCL, 0, 4),
	DIV(CLK_DOUT_G3D_SWITCH, "dout_g3d_switch", "gout_clkcmu_g3d_switch",
	    CLK_CON_DIV_CLKCMU_G3D_SWITCH, 0, 3),
	DIV(CLK_DOUT_HPM, "dout_hpm", "gout_clkcmu_hpm", CLK_CON_DIV_CLKCMU_HPM,
	    0, 2),
	DIV(CLK_DOUT_HSI0_BUS, "dout_hsi0_bus", "gout_clkcmu_hsi0_bus",
	    CLK_CON_DIV_CLKCMU_HSI0_BUS, 0, 4),
	DIV(CLK_DOUT_HSI0_DPGTC, "dout_hsi0_dpgtc", "gout_clkcmu_hsi0_dpgtc",
	    CLK_CON_DIV_CLKCMU_HSI0_DPGTC, 0, 3),
	DIV(CLK_DOUT_HSI0_USB31DRD, "dout_hsi0_usb31drd",
	    "gout_clkcmu_hsi0_usb31drd", CLK_CON_DIV_CLKCMU_HSI0_USB31DRD, 0,
	    4),
	DIV(CLK_DOUT_HSI0_USBDP_DEBUG, "dout_hsi0_usbdp_debug",
	    "gout_clkcmu_hsi0_usbdp_debug", CLK_CON_DIV_CLKCMU_HSI0_USBDP_DEBUG,
	    0, 7),
	DIV(CLK_DOUT_HSI1_BUS, "dout_hsi1_bus", "gout_clkcmu_hsi1_bus",
	    CLK_CON_DIV_CLKCMU_HSI1_BUS, 0, 3),
	DIV(CLK_DOUT_HSI1_MMC_CARD, "dout_hsi1_mmc_card",
	    "gout_clkcmu_hsi1_mmc_card", CLK_CON_DIV_CLKCMU_HSI1_MMC_CARD, 0,
	    9),
	DIV(CLK_DOUT_HSI1_PCIE, "dout_hsi1_pcie", "gout_clkcmu_hsi1_pcie",
	    CLK_CON_DIV_CLKCMU_HSI1_PCIE, 0, 7),
	DIV(CLK_DOUT_HSI1_UFS_CARD, "dout_hsi1_ufs_card",
	    "gout_clkcmu_hsi1_ufs_card", CLK_CON_DIV_CLKCMU_HSI1_UFS_CARD, 0,
	    3),
	DIV(CLK_DOUT_HSI1_UFS_EMBD, "dout_hsi1_ufs_embd",
	    "gout_clkcmu_hsi1_ufs_embd", CLK_CON_DIV_CLKCMU_HSI1_UFS_EMBD, 0,
	    3),
	DIV(CLK_DOUT_HSI2_BUS, "dout_hsi2_bus", "gout_clkcmu_hsi2_bus",
	    CLK_CON_DIV_CLKCMU_HSI2_BUS, 0, 4),
	DIV(CLK_DOUT_HSI2_PCIE, "dout_hsi2_pcie", "gout_clkcmu_hsi2_pcie",
	    CLK_CON_DIV_CLKCMU_HSI2_PCIE, 0, 7),
	DIV(CLK_DOUT_IPP_BUS, "dout_ipp_bus", "gout_clkcmu_ipp_bus",
	    CLK_CON_DIV_CLKCMU_IPP_BUS, 0, 4),
	DIV(CLK_DOUT_ITP_BUS, "dout_itp_bus", "gout_clkcmu_itp_bus",
	    CLK_CON_DIV_CLKCMU_ITP_BUS, 0, 4),
	DIV(CLK_DOUT_MCSC_BUS, "dout_mcsc_bus", "gout_clkcmu_mcsc_bus",
	    CLK_CON_DIV_CLKCMU_MCSC_BUS, 0, 4),
	DIV(CLK_DOUT_MCSC_GDC, "dout_mcsc_gdc", "gout_clkcmu_mcsc_gdc",
	    CLK_CON_DIV_CLKCMU_MCSC_GDC, 0, 4),
	DIV(CLK_DOUT_CMU_BOOST_CPU, "dout_cmu_boost_cpu",
	    "mout_clkcmu_cmu_boost_cpu", CLK_CON_DIV_CLKCMU_CMU_BOOST_CPU, 0,
	    2),
	DIV(CLK_DOUT_MFC0_MFC0, "dout_mfc0_mfc0", "gout_clkcmu_mfc0_mfc0",
	    CLK_CON_DIV_CLKCMU_MFC0_MFC0, 0, 4),
	DIV(CLK_DOUT_MFC0_WFD, "dout_mfc0_wfd", "gout_clkcmu_mfc0_wfd",
	    CLK_CON_DIV_CLKCMU_MFC0_WFD, 0, 4),
	DIV(CLK_DOUT_MIF_BUSP, "dout_mif_busp", "gout_clkcmu_mif_busp",
	    CLK_CON_DIV_CLKCMU_MIF_BUSP, 0, 4),
	DIV(CLK_DOUT_NPU_BUS, "dout_npu_bus", "gout_clkcmu_npu_bus",
	    CLK_CON_DIV_CLKCMU_NPU_BUS, 0, 4),
	DIV(CLK_DOUT_PERIC0_BUS, "dout_peric0_bus", "gout_clkcmu_peric0_bus",
	    CLK_CON_DIV_CLKCMU_PERIC0_BUS, 0, 4),
	DIV(CLK_DOUT_PERIC0_IP, "dout_peric0_ip", "gout_clkcmu_peric0_ip",
	    CLK_CON_DIV_CLKCMU_PERIC0_IP, 0, 4),
	DIV(CLK_DOUT_PERIC1_BUS, "dout_peric1_bus", "gout_clkcmu_peric1_bus",
	    CLK_CON_DIV_CLKCMU_PERIC1_BUS, 0, 4),
	DIV(CLK_DOUT_PERIC1_IP, "dout_peric1_ip", "gout_clkcmu_peric1_ip",
	    CLK_CON_DIV_CLKCMU_PERIC1_IP, 0, 4),
	DIV(CLK_DOUT_PERIS_BUS, "dout_peris_bus", "gout_clkcmu_peris_bus",
	    CLK_CON_DIV_CLKCMU_PERIS_BUS, 0, 4),
	DIV(CLK_DOUT_SSP_BUS, "dout_ssp_bus", "gout_clkcmu_ssp_bus",
	    CLK_CON_DIV_CLKCMU_SSP_BUS, 0, 4),
	DIV(CLK_DOUT_TNR_BUS, "dout_tnr_bus", "gout_clkcmu_tnr_bus",
	    CLK_CON_DIV_CLKCMU_TNR_BUS, 0, 4),
	DIV(CLK_DOUT_VRA_BUS, "dout_vra_bus", "mout_clkcmu_vra_bus",
	    CLK_CON_DIV_CLKCMU_VRA_BUS, 0, 4),
	DIV(CLK_DOUT_CLKCMU_DPU, "dout_clkcmu_dpu", "gout_clkcmu_dpu",
	    CLK_CON_DIV_DIV_CLKCMU_DPU, 0, 4),
};

static const struct samsung_gate_clock top_gate_clks[] __initconst = {
	GATE(CLK_GOUT_CLKCMU_APM_BUS, "gout_clkcmu_apm_bus",
	     "mout_clkcmu_apm_bus", CLK_CON_GAT_GATE_CLKCMU_APM_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_AUD_CPU, "gout_clkcmu_aud_cpu",
	     "mout_clkcmu_aud_cpu", CLK_CON_GAT_GATE_CLKCMU_AUD_CPU, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_BUS0_BUS, "gout_clkcmu_bus0_bus",
	     "mout_clkcmu_bus0_bus", CLK_CON_GAT_GATE_CLKCMU_BUS0_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_BUS1_BUS, "gout_clkcmu_bus1_bus",
	     "mout_clkcmu_bus1_bus", CLK_CON_GAT_GATE_CLKCMU_BUS1_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_BUS1_SSS, "gout_clkcmu_bus1_sss",
	     "mout_clkcmu_bus1_sss", CLK_CON_GAT_GATE_CLKCMU_BUS1_SSS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CIS_CLK0, "gout_clkcmu_cis_clk0",
	     "mout_clkcmu_cis_clk0", CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CIS_CLK1, "gout_clkcmu_cis_clk1",
	     "mout_clkcmu_cis_clk1", CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CIS_CLK2, "gout_clkcmu_cis_clk2",
	     "mout_clkcmu_cis_clk2", CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CIS_CLK3, "gout_clkcmu_cis_clk3",
	     "mout_clkcmu_cis_clk3", CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CIS_CLK4, "gout_clkcmu_cis_clk4",
	     "mout_clkcmu_cis_clk4", CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CIS_CLK5, "gout_clkcmu_cis_clk5",
	     "mout_clkcmu_cis_clk5", CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CORE_BUS, "gout_clkcmu_core_bus",
	     "mout_clkcmu_core_bus", CLK_CON_GAT_GATE_CLKCMU_CORE_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CPUCL0_DBG_BUS, "gout_clkcmu_cpucl0_dbg_bus",
	     "mout_clkcmu_cpucl0_dbg_bus",
	     CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_BUS, 21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CPUCL0_SWITCH, "gout_clkcmu_cpucl0_switch",
	     "mout_clkcmu_cpucl0_switch", CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CPUCL1_SWITCH, "gout_clkcmu_cpucl1_switch",
	     "mout_clkcmu_cpucl1_switch", CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CPUCL2_BUSP, "gout_clkcmu_cpucl2_busp",
	     "mout_clkcmu_cpucl2_busp", CLK_CON_GAT_GATE_CLKCMU_CPUCL2_BUSP, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CPUCL2_SWITCH, "gout_clkcmu_cpucl2_switch",
	     "mout_clkcmu_cpucl2_switch", CLK_CON_GAT_GATE_CLKCMU_CPUCL2_SWITCH,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CSIS_BUS, "gout_clkcmu_csis_bus",
	     "mout_clkcmu_csis_bus", CLK_CON_GAT_GATE_CLKCMU_CSIS_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_CSIS_OIS_MCU, "gout_clkcmu_csis_ois_mcu",
	     "mout_clkcmu_csis_ois_mcu", CLK_CON_GAT_GATE_CLKCMU_CSIS_OIS_MCU,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_DNC_BUS, "gout_clkcmu_dnc_bus",
	     "mout_clkcmu_dnc_bus", CLK_CON_GAT_GATE_CLKCMU_DNC_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_DNC_BUSM, "gout_clkcmu_dnc_busm",
	     "mout_clkcmu_dnc_busm", CLK_CON_GAT_GATE_CLKCMU_DNC_BUSM, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_DNS_BUS, "gout_clkcmu_dns_bus",
	     "mout_clkcmu_dns_bus", CLK_CON_GAT_GATE_CLKCMU_DNS_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_DPU, "gout_clkcmu_dpu", "mout_clkcmu_dpu",
	     CLK_CON_GAT_GATE_CLKCMU_DPU, 21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_DPU_BUS, "gout_clkcmu_dpu_bus",
	     "mout_clkcmu_dpu_alt", CLK_CON_GAT_GATE_CLKCMU_DPU_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_DSP_BUS, "gout_clkcmu_dsp_bus",
	     "mout_clkcmu_dsp_bus", CLK_CON_GAT_GATE_CLKCMU_DSP_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_G2D_G2D, "gout_clkcmu_g2d_g2d",
	     "mout_clkcmu_g2d_g2d", CLK_CON_GAT_GATE_CLKCMU_G2D_G2D, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_G2D_MSCL, "gout_clkcmu_g2d_mscl",
	     "mout_clkcmu_g2d_mscl", CLK_CON_GAT_GATE_CLKCMU_G2D_MSCL, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_G3D_SWITCH, "gout_clkcmu_g3d_switch",
	     "fout_shared2_pll", CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HPM, "gout_clkcmu_hpm", "mout_clkcmu_hpm",
	     CLK_CON_GAT_GATE_CLKCMU_HPM, 21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI0_BUS, "gout_clkcmu_hsi0_bus",
	     "mout_clkcmu_hsi0_bus", CLK_CON_GAT_GATE_CLKCMU_HSI0_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI0_DPGTC, "gout_clkcmu_hsi0_dpgtc",
	     "mout_clkcmu_hsi0_dpgtc", CLK_CON_GAT_GATE_CLKCMU_HSI0_DPGTC, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI0_USB31DRD, "gout_clkcmu_hsi0_usb31drd",
	     "mout_clkcmu_hsi0_usb31drd", CLK_CON_GAT_GATE_CLKCMU_HSI0_USB31DRD,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI0_USBDP_DEBUG, "gout_clkcmu_hsi0_usbdp_debug",
	     "mout_clkcmu_hsi0_usbdp_debug",
	     CLK_CON_GAT_GATE_CLKCMU_HSI0_USBDP_DEBUG, 21, CLK_IGNORE_UNUSED,
	     0),
	GATE(CLK_GOUT_CLKCMU_HSI1_BUS, "gout_clkcmu_hsi1_bus",
	     "mout_clkcmu_hsi1_bus", CLK_CON_GAT_GATE_CLKCMU_HSI1_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI1_MMC_CARD, "gout_clkcmu_hsi1_mmc_card",
	     "mout_clkcmu_hsi1_mmc_card", CLK_CON_GAT_GATE_CLKCMU_HSI1_MMC_CARD,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI1_PCIE, "gout_clkcmu_hsi1_pcie",
	     "mout_clkcmu_hsi1_pcie", CLK_CON_GAT_GATE_CLKCMU_HSI1_PCIE, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI1_UFS_CARD, "gout_clkcmu_hsi1_ufs_card",
	     "mout_clkcmu_hsi1_ufs_card", CLK_CON_GAT_GATE_CLKCMU_HSI1_UFS_CARD,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI1_UFS_EMBD, "gout_clkcmu_hsi1_ufs_embd",
	     "mout_clkcmu_hsi1_ufs_embd", CLK_CON_GAT_GATE_CLKCMU_HSI1_UFS_EMBD,
	     21, CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI2_BUS, "gout_clkcmu_hsi2_bus",
	     "mout_clkcmu_hsi2_bus", CLK_CON_GAT_GATE_CLKCMU_HSI2_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_HSI2_PCIE, "gout_clkcmu_hsi2_pcie",
	     "mout_clkcmu_hsi2_pcie", CLK_CON_GAT_GATE_CLKCMU_HSI2_PCIE, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_IPP_BUS, "gout_clkcmu_ipp_bus",
	     "mout_clkcmu_ipp_bus", CLK_CON_GAT_GATE_CLKCMU_IPP_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_ITP_BUS, "gout_clkcmu_itp_bus",
	     "mout_clkcmu_itp_bus", CLK_CON_GAT_GATE_CLKCMU_ITP_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_MCSC_BUS, "gout_clkcmu_mcsc_bus",
	     "mout_clkcmu_mcsc_bus", CLK_CON_GAT_GATE_CLKCMU_MCSC_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_MCSC_GDC, "gout_clkcmu_mcsc_gdc",
	     "mout_clkcmu_mcsc_gdc", CLK_CON_GAT_GATE_CLKCMU_MCSC_GDC, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_MFC0_MFC0, "gout_clkcmu_mfc0_mfc0",
	     "mout_clkcmu_mfc0_mfc0", CLK_CON_GAT_GATE_CLKCMU_MFC0_MFC0, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_MFC0_WFD, "gout_clkcmu_mfc0_wfd",
	     "mout_clkcmu_mfc0_wfd", CLK_CON_GAT_GATE_CLKCMU_MFC0_WFD, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_MIF_BUSP, "gout_clkcmu_mif_busp",
	     "mout_clkcmu_mif_busp", CLK_CON_GAT_GATE_CLKCMU_MIF_BUSP, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_NPU_BUS, "gout_clkcmu_npu_bus",
	     "mout_clkcmu_npu_bus", CLK_CON_GAT_GATE_CLKCMU_NPU_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_PERIC0_BUS, "gout_clkcmu_peric0_bus",
	     "mout_clkcmu_peric0_bus", CLK_CON_GAT_GATE_CLKCMU_PERIC0_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_PERIC0_IP, "gout_clkcmu_peric0_ip",
	     "mout_clkcmu_peric0_ip", CLK_CON_GAT_GATE_CLKCMU_PERIC0_IP, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_PERIC1_BUS, "gout_clkcmu_peric1_bus",
	     "mout_clkcmu_peric1_bus", CLK_CON_GAT_GATE_CLKCMU_PERIC1_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_PERIC1_IP, "gout_clkcmu_peric1_ip",
	     "mout_clkcmu_peric1_ip", CLK_CON_GAT_GATE_CLKCMU_PERIC1_IP, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_PERIS_BUS, "gout_clkcmu_peris_bus",
	     "mout_clkcmu_peris_bus", CLK_CON_GAT_GATE_CLKCMU_PERIS_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_SSP_BUS, "gout_clkcmu_ssp_bus",
	     "mout_clkcmu_ssp_bus", CLK_CON_GAT_GATE_CLKCMU_SSP_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_TNR_BUS, "gout_clkcmu_tnr_bus",
	     "mout_clkcmu_tnr_bus", CLK_CON_GAT_GATE_CLKCMU_TNR_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
	GATE(CLK_GOUT_CLKCMU_VRA_BUS, "gout_clkcmu_vra_bus",
	     "mout_clkcmu_vra_bus", CLK_CON_GAT_GATE_CLKCMU_VRA_BUS, 21,
	     CLK_IGNORE_UNUSED, 0),
};

static const struct samsung_cmu_info top_cmu_info __initconst = {
	.pll_clks = top_pll_clks,
	.nr_pll_clks = ARRAY_SIZE(top_pll_clks),
	.mux_clks = top_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(top_mux_clks),
	.div_clks = top_div_clks,
	.nr_div_clks = ARRAY_SIZE(top_div_clks),
	.gate_clks = top_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(top_gate_clks),
	.nr_clk_ids = CLKS_NR_TOP,
	.clk_regs = top_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(top_clk_regs),
};

/* ---------- CMU_HSI0 ---------- */

#define PLL_CON0_MUX_CLKCMU_HSI0_BUS_USER 		0x600
#define PLL_CON1_MUX_CLKCMU_HSI0_BUS_USER 		0x604
#define PLL_CON0_MUX_CLKCMU_HSI0_USB31DRD_USER 		0x620
#define PLL_CON1_MUX_CLKCMU_HSI0_USB31DRD_USER 		0x624
#define PLL_CON0_MUX_CLKCMU_HSI0_USBDP_DEBUG_USER 	0x630
#define PLL_CON1_MUX_CLKCMU_HSI0_USBDP_DEBUG_USER 	0x634
#define PLL_CON0_MUX_CLKCMU_HSI0_DPGTC_USER 		0x610
#define PLL_CON1_MUX_CLKCMU_HSI0_DPGTC_USER 		0x614
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_DP_GTC_CLK 0x2004
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_BUS1_IPCLKPORT_ACLK 0x2018
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_LHS_ACEL_D_HSI0_IPCLKPORT_I_CLK 0x2014
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_BUS_IPCLKPORT_CLK 0x2020
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_VGEN_LITE_HSI0_IPCLKPORT_CLK 0x2044
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_PCLK 0x2008
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_D_TZPC_HSI0_IPCLKPORT_PCLK 0x200c
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_LHM_AXI_P_HSI0_IPCLKPORT_I_CLK 0x2010
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_BUS1_IPCLKPORT_PCLK 0x201c
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S2 0x2024
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSREG_HSI0_IPCLKPORT_PCLK 0x2028
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_ACLK_PHYCTRL 0x202c
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_REF_CLK_40 0x2034
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_SCL_APB_PCLK 0x203c
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBPCS_APB_CLK 0x2040
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_BUS_CLK_EARLY 0x2030
#define CLK_CON_GAT_CLK_BLK_HSI0_UID_HSI0_CMU_HSI0_IPCLKPORT_PCLK 0x2000
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D_HSI0_IPCLKPORT_ACLK 0x2048
#define CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_REF_SOC_PLL 0x2038

static const unsigned long hsi0_clk_regs[] __initconst = {
	PLL_CON0_MUX_CLKCMU_HSI0_BUS_USER,
	PLL_CON1_MUX_CLKCMU_HSI0_BUS_USER,
	PLL_CON0_MUX_CLKCMU_HSI0_USB31DRD_USER,
	PLL_CON1_MUX_CLKCMU_HSI0_USB31DRD_USER,
	PLL_CON0_MUX_CLKCMU_HSI0_USBDP_DEBUG_USER,
	PLL_CON1_MUX_CLKCMU_HSI0_USBDP_DEBUG_USER,
	PLL_CON0_MUX_CLKCMU_HSI0_DPGTC_USER,
	PLL_CON1_MUX_CLKCMU_HSI0_DPGTC_USER,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_DP_GTC_CLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_BUS1_IPCLKPORT_ACLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_LHS_ACEL_D_HSI0_IPCLKPORT_I_CLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_RSTNSYNC_CLK_HSI0_BUS_IPCLKPORT_CLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_VGEN_LITE_HSI0_IPCLKPORT_CLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_DP_LINK_IPCLKPORT_I_PCLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_D_TZPC_HSI0_IPCLKPORT_PCLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_LHM_AXI_P_HSI0_IPCLKPORT_I_CLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_PPMU_HSI0_BUS1_IPCLKPORT_PCLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSMMU_USB_IPCLKPORT_CLK_S2,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_SYSREG_HSI0_IPCLKPORT_PCLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_ACLK_PHYCTRL,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_SCL_APB_PCLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBPCS_APB_CLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_BUS_CLK_EARLY,
	CLK_CON_GAT_CLK_BLK_HSI0_UID_HSI0_CMU_HSI0_IPCLKPORT_PCLK,
	CLK_CON_GAT_GOUT_BLK_HSI0_UID_XIU_D_HSI0_IPCLKPORT_ACLK,
};

PNAME(mout_hsi0_bus_user_p) = { "oscclk", "dout_hsi0_bus" };
PNAME(mout_hsi0_usb31drd_user_p) = { "oscclk", "dout_hsi0_usb31drd" };
PNAME(mout_hsi0_usbdp_debug_user_p) = { "oscclk", "dout_hsi0_usbdp_debug" };
PNAME(mout_hsi0_dpgtc_user_p) = { "oscclk", "dout_hsi0_dpgtc" };

static const struct samsung_mux_clock hsi0_mux_clks[] __initconst = {
	MUX(CLK_MOUT_HSI0_BUS_USER, "mout_hsi0_bus_user", mout_hsi0_bus_user_p,
	    PLL_CON0_MUX_CLKCMU_HSI0_BUS_USER, 0, 1),
	MUX(CLK_MOUT_HSI0_USB31DRD_USER, "mout_hsi0_usb31drd_user",
	    mout_hsi0_usb31drd_user_p, PLL_CON0_MUX_CLKCMU_HSI0_USB31DRD_USER,
	    0, 1),
	MUX(CLK_MOUT_HSI0_USBDP_DEBUG_USER, "mout_hsi0_usbdp_debug_user",
	    mout_hsi0_bus_user_p, PLL_CON0_MUX_CLKCMU_HSI0_USBDP_DEBUG_USER,
	    0, 1),
	MUX(CLK_MOUT_HSI0_DPGTC_USER, "mout_hsi0_bus_user", mout_hsi0_bus_user_p,
	    PLL_CON0_MUX_CLKCMU_HSI0_DPGTC_USER, 0, 1),
};

static const struct samsung_gate_clock hsi0_gate_clks[] __initconst = {
	GATE(CLK_GOUT_USB31DRD_IPCLKPORT_ACLK_PHYCTRL, "gout_hsi0_usb31drd_ipclkport_aclk_phyctrl", "mout_hsi0_bus_user", CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_ACLK_PHYCTRL, 21, 0, 0),
	GATE(CLK_GOUT_USB31DRD_IPCLKPORT_BUS_CLK_EARLY, "gout_hsi0_usb31drd_ipclkport_bus_clk_early", "mout_hsi0_bus_user", CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_BUS_CLK_EARLY, 21, 0, 0),
	GATE(CLK_GOUT_USB31DRD_IPCLKPORT_I_USB31DRD_REF_CLK_40,
	     "gout_hsi0_usb31drd_ipclkport_usb31drd_ref_clk_40", "mout_hsi0_usb31drd_user",
	     CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USB31DRD_REF_CLK_40,
	     21, 0, 0),
	GATE(CLK_GOUT_USB31DRD_IPCLKPORT_I_USBDPPHY_REF_SOC_PLL,
	     "gout_hsi0_usb31drd_ipclkport_i_usbdpphy_ref_soc_pll",
	     "dout_hsi0_usbdp_debug",
	     CLK_CON_GAT_GOUT_BLK_HSI0_UID_USB31DRD_IPCLKPORT_I_USBDPPHY_REF_SOC_PLL,
	     21, 0, 0),
};

static const struct samsung_cmu_info hsi0_cmu_info __initconst = {
	.mux_clks = hsi0_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(hsi0_mux_clks),
	.gate_clks = hsi0_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(hsi0_gate_clks),
	.nr_clk_ids = CLKS_NR_HSI0,
	.clk_regs = hsi0_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(hsi0_clk_regs),
};

/* hsi1 piece of shit */
#define PLL_CON0_MUX_CLKCMU_HSI1_BUS_USER 0x600
#define PLL_CON0_MUX_CLKCMU_HSI1_UFS_EMBD_USER 0x640
#define CLK_CON_GAT_GOUT_BLK_HSI1_UID_UFS_EMBD_IPCLKPORT_I_ACLK 0x2080
#define CLK_CON_GAT_GOUT_BLK_HSI1_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO 0x2084

PNAME(mout_hsi1_bus_user_p) = { "oscclk", "dout_hsi1_bus" };
PNAME(mout_hsi1_ufs_embd_user_p) = { "oscclk", "dout_hsi1_ufs_embd" };

static const unsigned long hsi1_clk_regs[] __initconst = {
	PLL_CON0_MUX_CLKCMU_HSI1_BUS_USER,
	PLL_CON0_MUX_CLKCMU_HSI1_UFS_EMBD_USER,
	CLK_CON_GAT_GOUT_BLK_HSI1_UID_UFS_EMBD_IPCLKPORT_I_ACLK,
	CLK_CON_GAT_GOUT_BLK_HSI1_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO,
};

static const struct samsung_gate_clock hsi1_gate_clks[] __initconst = {
	GATE(2, "gout_hsi1_blk_hsi1_uid_ufs_embd_ipclkport_i_aclk",
	     "mout_hsi1_bus_user", CLK_CON_GAT_GOUT_BLK_HSI1_UID_UFS_EMBD_IPCLKPORT_I_ACLK,
	     21, 0, 0),
	GATE(3, "gout_hsi1_blk_hsi1_uid_ufs_embd_ipclkport_i_unipro",
	     "mout_hsi1_ufs_embd_user", CLK_CON_GAT_GOUT_BLK_HSI1_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO,
	     21, 0, 0),
};

static const struct samsung_mux_clock hsi1_mux_clks[] __initconst = {
	MUX(0, "mout_hsi1_bus_user", mout_hsi1_bus_user_p,
	    PLL_CON0_MUX_CLKCMU_HSI1_BUS_USER, CLK_IGNORE_UNUSED, 1),
	MUX(1, "mout_hsi1_ufs_embd_user", mout_hsi1_ufs_embd_user_p,
	    PLL_CON0_MUX_CLKCMU_HSI1_UFS_EMBD_USER, CLK_IGNORE_UNUSED, 1),
};

static const struct samsung_cmu_info hsi1_cmu_info __initconst = {
	.mux_clks = hsi1_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(hsi1_mux_clks),
	.gate_clks = hsi1_gate_clks,
	.nr_gate_clks = ARRAY_SIZE(hsi1_gate_clks),
	.nr_clk_ids = 4,
	.clk_regs = hsi1_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(hsi1_clk_regs),
};

/* ----- platform_driver ----- */

static int __init exynos990_cmu_probe(struct platform_device *pdev)
{
	const struct samsung_cmu_info *info;
	struct device *dev = &pdev->dev;

	info = of_device_get_match_data(dev);
	exynos_arm64_register_cmu(dev, dev->of_node, info);

	return 0;
}

static const struct of_device_id exynos990_cmu_of_match[] = {
	{
		.compatible = "samsung,exynos990-cmu-top",
		.data = &top_cmu_info,
	},
	{
		.compatible = "samsung,exynos990-cmu-hsi0",
		.data = &hsi0_cmu_info,
	},
	{
		.compatible = "samsung,exynos990-cmu-hsi1",
		.data = &hsi1_cmu_info,
	},
	{},
};

static struct platform_driver exynos990_cmu_driver __refdata = {
	.driver	= {
		.name = "exynos990-cmu",
		.of_match_table = exynos990_cmu_of_match,
		.suppress_bind_attrs = true,
	},
	.probe = exynos990_cmu_probe,
};

static int __init exynos990_cmu_init(void)
{
	return platform_driver_register(&exynos990_cmu_driver);
}

core_initcall(exynos990_cmu_init);

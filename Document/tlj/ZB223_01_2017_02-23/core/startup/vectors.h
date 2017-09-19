/******************************************************************************
* File:    vectors.h
*
* Purpose: Provide custom interrupt service routines for Kinetis. 
*
* NOTE: This vector table is a superset table, so interrupt sources might be 
*       listed that are not available on the specific Kinetis device you are 
*       using.
******************************************************************************/

#ifndef __VECTORS_H
#define __VECTORS_H     1

#include "common.h"
#include "ISR.H"
#include "ap_S32K1xx_FlexCAN.h"
// function prototype for default_isr in vectors.c
void default_isr(void);
extern void lpit0_isr(void);
extern void can0Recieve_isr(void);
extern void can1Recieve_isr(void);
extern void can2Recieve_isr(void);
extern void SysTick_Handler(void);
extern void FTM3_IRQHandler(void);
extern void LPTMR_Isr(void);




//void abort_isr(void);

//void hard_fault_handler_c(unsigned int * hardfault_args);

/* Interrupt Vector Table Function Pointers */
typedef void pointer(void);
#ifdef __GNUC__
	extern void __thumb_startup(void);
	#define STARTUP_FUNC  __thumb_startup
#else
	extern void __startup(void);
	#define STARTUP_FUNC  __startup
#endif
extern unsigned long __BOOT_STACK_ADDRESS[];
//extern void __iar_program_start(void);
extern void default_isr(void);
                                        // Address     Vector IRQ   Source module   Source description
#define VECTOR_000      (pointer*)__BOOT_STACK_ADDRESS	//          ARM core        Initial Supervisor SP
#define VECTOR_001      STARTUP_FUNC	// 0x0000_0004 1 -          ARM core        Initial Program Counter
#define VECTOR_002      default_isr     // 0x0000_0008 2 -          ARM core        Non-maskable Interrupt (NMI)
#define VECTOR_003      default_isr     // 0x0000_000C 3 -          ARM core        Hard Fault
#define VECTOR_004      default_isr     /* 0x04 Interrupt no. 2 (Unused)   - ivINT_Illegalinstruction  */
#define VECTOR_005      default_isr     /* 0x06 Interrupt no. 3 (Unused)   - ivINT_SW3  */
#define VECTOR_006      default_isr     /* 0x08 Interrupt no. 4 (Unused)   - ivINT_HWStackOverflow  */
#define VECTOR_007      default_isr     /* 0x0a Interrupt no. 5 (Unused)   - ivINT_MisalignedLongWordAccess  */
#define VECTOR_008      default_isr     /* 0x0c Interrupt no. 6 (Unused)   - ivINT_OnCE_StepCounter  */
#define VECTOR_009      default_isr     /* 0x0e Interrupt no. 7 (Unused)   - ivINT_OnCE_BreakpointUnit  */
#define VECTOR_010      default_isr     /* 0x10 Interrupt no. 8 (Unused)   - ivINT_OnCE_TraceBuffer  */
#define VECTOR_011      default_isr     /* 0x12 Interrupt no. 9 (Unused)   - ivINT_OnCE_TxREmpty  */
#define VECTOR_012      default_isr     /* 0x14 Interrupt no. 10 (Unused)  - ivINT_OnCE_RxRFull  */
#define VECTOR_013      default_isr     /* 0x16 Interrupt no. 11 (Unused)  - ivINT_Bus_ERR*/
#define VECTOR_014      default_isr     /* 0x18 Interrupt no. 12 (Unused)  - ivINT_Rsved  */
#define VECTOR_015      SysTick_Handler     /* 0x1a Interrupt no. 13 (Unused)  - ivINT_Rsved  */
#define VECTOR_016      default_isr     /* 0x1c Interrupt no. 14 (Unused)  - ivINT_Rsved  */
#define VECTOR_017      default_isr     /* 0x1e Interrupt no. 15 (Unused)  - ivINT_SW2  */
#define VECTOR_018      default_isr     /* 0x20 Interrupt no. 16 (Unused)  - ivINT_SW1  */
#define VECTOR_019      default_isr     /* 0x22 Interrupt no. 17 (Unused)  - ivINT_SW0  */
#define VECTOR_020      default_isr     /* 0x24 Interrupt no. 18 (Unused)  - ivINT_XBARA  */
#define VECTOR_021      default_isr     /* 0x26 Interrupt no. 19 (Unused)  - ivINT_LVI  */
#define VECTOR_022      default_isr     /* 0x28 Interrupt no. 20 (Unused)  - ivINT_OCCS  */
#define VECTOR_023      default_isr     /* 0x2a Interrupt no. 21 (Unused)  - ivINT_TMRB3  */
#define VECTOR_024      default_isr     /* 0x2c Interrupt no. 22 (Unused)  - ivINT_TMRB2  */
#define VECTOR_025      default_isr     /* 0x2e Interrupt no. 23 (Unused)  - ivINT_TMRB1  */
#define VECTOR_026      default_isr     /* 0x30 Interrupt no. 24 (Unused)  - ivINT_TMRB0  */
#define VECTOR_027      default_isr     /* 0x32 Interrupt no. 25 (Unused)  - ivINT_TMRA3  */
#define VECTOR_028      default_isr     /* 0x34 Interrupt no. 26 (Unused)  - ivINT_TMRA2  */
#define VECTOR_029      default_isr     /* 0x36 Interrupt no. 27 (Unused)  - ivINT_TMRA1  */
#define VECTOR_030      default_isr     /* 0x38 Interrupt no. 28 (Unused)  - ivINT_TMRA0  */
#define VECTOR_031      default_isr     /* 0x3a Interrupt no. 29 (Unused)  - ivINT_ADC_CC1  */
#define VECTOR_032      default_isr     /* 0x3c Interrupt no. 30 (Unused)  - ivINT_ADC_CC0  */
#define VECTOR_033      default_isr     /* 0x3e Interrupt no. 31 (Unused)  - ivINT_ADC_ERR  */
#define VECTOR_034      default_isr     /* 0x40 Interrupt no. 32 (Unused)  - ivINT_ADC_COCO  */
#define VECTOR_035      default_isr     /* 0x42 Interrupt no. 33 (Unused)  - ivINT_DMA3  */
#define VECTOR_036      default_isr     /* 0x44 Interrupt no. 34 (Unused)  - ivINT_DMA2  */
#define VECTOR_037      default_isr     /* 0x46 Interrupt no. 35 (Unused)  - ivINT_DMA1  */
#define VECTOR_038      default_isr     /* 0x48 Interrupt no. 36 (Unused)  - ivINT_DMA0  */
#define VECTOR_039      default_isr     /* 0x4a Interrupt no. 37 (Unused)  - ivINT_CAN_MB_OR  */
#define VECTOR_040      default_isr     /* 0x4c Interrupt no. 38 (Unused)  - ivINT_CAN_BUS_OFF  */
#define VECTOR_041      default_isr     /* 0x4e Interrupt no. 39 (Unused)  - ivINT_CAN_ERR  */
#define VECTOR_042      default_isr     /* 0x50 Interrupt no. 40 (Unused)  - ivINT_CAN_TXWARN  */
#define VECTOR_043      default_isr     /* 0x52 Interrupt no. 41 (Unused)  - ivINT_CAN_RXWARN  */
#define VECTOR_044      default_isr     /* 0x54 Interrupt no. 42 (Unused)  - ivINT_CAN_WakeUp  */
#define VECTOR_045      default_isr     /* 0x56 Interrupt no. 43 (Unused)  - ivINT_QSCI2_RxError  */
#define VECTOR_046      default_isr     /* 0x58 Interrupt no. 44 (Unused)  - ivINT_QSCI2_RxFull  */
#define VECTOR_047      default_isr     /* 0x5a Interrupt no. 45 (Unused)  - ivINT_QSCI2_TxIdle  */
#define VECTOR_048      default_isr     /* 0x5c Interrupt no. 46 (Unused)  - ivINT_QSCI2_TxEmpty  */
#define VECTOR_049      default_isr     /* 0x5e Interrupt no. 47 (Unused)  - ivINT_QSCI1_RxError  */
#define VECTOR_050      default_isr     /* 0x60 Interrupt no. 48 (Unused)  - ivINT_QSCI1_RxFull  */
#define VECTOR_051      default_isr     /* 0x62 Interrupt no. 49 (Unused)  - ivINT_QSCI1_TxIdle  */
#define VECTOR_052      default_isr     /* 0x64 Interrupt no. 50 (Unused)  - ivINT_QSCI1_TxEmpty  */
#define VECTOR_053      default_isr     /* 0x66 Interrupt no. 51 (Unused)  - ivINT_QSCI0_RxError  */
#define VECTOR_054      default_isr     /* 0x68 Interrupt no. 52 (Unused)  - ivINT_QSCI0_RxFull  */
#define VECTOR_055      default_isr     /* 0x6a Interrupt no. 53 (Unused)  - ivINT_QSCI0_TxIdle  */
#define VECTOR_056      default_isr     /* 0x6c Interrupt no. 54 (Unused)  - ivINT_QSCI0_TxEmpty  */
#define VECTOR_057      default_isr     /* 0x6e Interrupt no. 55 (Unused)  - ivINT_QSPI2_TxEmpty  */
#define VECTOR_058      default_isr     /* 0x70 Interrupt no. 56 (Unused)  - ivINT_QSPI2_RxFull  */
#define VECTOR_059      default_isr     /* 0x72 Interrupt no. 57 (Unused)  - ivINT_QSPI1_TxEmpty  */
#define VECTOR_060      default_isr     /* 0x74 Interrupt no. 58 (Unused)  - ivINT_QSPI1_RxFull  */
#define VECTOR_061      default_isr     /* 0x76 Interrupt no. 59 (Unused)  - ivINT_QSPI0_TxEmpty  */
#define VECTOR_062      default_isr     /* 0x78 Interrupt no. 60 (Unused)  - ivINT_QSPI0_RxFull  */
#define VECTOR_063      default_isr     /* 0x7a Interrupt no. 61 (Unused)  - ivINT_IIC1  */
#define VECTOR_064      lpit0_isr	    /* 0x7c Interrupt no. 62 (Unused)  - ivINT_IIC0  */
#define VECTOR_065      default_isr     /* 0x7e Interrupt no. 63 (Unused)  - ivINT_PWMNS_Fault  */
#define VECTOR_066      default_isr     /* 0x80 Interrupt no. 64 (Unused)  - ivINT_PWMNS_RERR  */
#define VECTOR_067      default_isr     /* 0x82 Interrupt no. 65 (Unused)  - ivINT_PWMNS_Reload3  */
#define VECTOR_068      default_isr     /* 0x84 Interrupt no. 66 (Unused)  - ivINT_PWMNS_CAP3  */
#define VECTOR_069      default_isr     /* 0x86 Interrupt no. 67 (Unused)  - ivINT_PWMNS_CMP3  */
#define VECTOR_070      default_isr     /* 0x88 Interrupt no. 68 (Unused)  - ivINT_PWMNS_Reload2  */
#define VECTOR_071      default_isr     /* 0x8a Interrupt no. 69 (Unused)  - ivINT_PWMNS_CAP2  */
#define VECTOR_072      default_isr     /* 0x8c Interrupt no. 70 (Unused)  - ivINT_PWMNS_CMP2  */
#define VECTOR_073      default_isr     /* 0x8e Interrupt no. 71 (Unused)  - ivINT_PWMNS_Reload1  */
#define VECTOR_074      LPTMR_Isr     /* 0x90 Interrupt no. 72 (Unused)  - ivINT_PWMNS_CAP1  */
#define VECTOR_075      default_isr     /* 0x92 Interrupt no. 73 (Unused)  - ivINT_PWMNS_CMP1  */
#define VECTOR_076      default_isr     /* 0x94 Interrupt no. 74 (Unused)  - ivINT_PWMNS_Reload0  */
#define VECTOR_077      default_isr     /* 0x96 Interrupt no. 75 (Unused)  - ivINT_PWMNS_CAP0  */
#define VECTOR_078      default_isr     /* 0x98 Interrupt no. 76 (Unused)  - ivINT_PWMNS_CMP0  */
#define VECTOR_079      default_isr     /* 0x9a Interrupt no. 77 (Unused)  - ivINT_PWMHS_Fault  */
#define VECTOR_080      default_isr     /* 0x9c Interrupt no. 78 (Unused)  - ivINT_PWMHS_RERR  */
#define VECTOR_081      default_isr     /* 0x9e Interrupt no. 79 (Unused)  - ivINT_PWMHS_Reload3  */
#define VECTOR_082      default_isr     /* 0xa0 Interrupt no. 80 (Unused)  - ivINT_PWMHS_CAP3  */
#define VECTOR_083      default_isr     /* 0xa2 Interrupt no. 81 (Unused)  - ivINT_PWMHS_CMP3  */
#define VECTOR_084      default_isr     /* 0xa4 Interrupt no. 82 (Unused)  - ivINT_PWMHS_Reload2  */
#define VECTOR_085      default_isr     /* 0xa6 Interrupt no. 83 (Unused)  - ivINT_PWMHS_CMP2  */
#define VECTOR_086      default_isr     /* 0xa8 Interrupt no. 84 (Unused)  - ivINT_PWMHS_Reload1  */
#define VECTOR_087      FTM3_IRQHandler   /* 0xaa Interrupt no. 85 (Unused)  - ivINT_PWMHS_CMP1  */
#define VECTOR_088      default_isr     /* 0xac Interrupt no. 86 (Unused)  - ivINT_PWMHS_Reload0  */
#define VECTOR_089      default_isr     /* 0xae Interrupt no. 87 (Unused)  - ivINT_PWMHS_CMP0  */
#define VECTOR_090      default_isr     /* 0xb0 Interrupt no. 88 (Unused)  - ivINT_FTFL_RDCOL  */
#define VECTOR_091      default_isr     /* 0xb2 Interrupt no. 89 (Unused)  - ivINT_FTFL_CC  */
#define VECTOR_092      default_isr     /* 0xb4 Interrupt no. 90 (Unused)  - ivINT_CMP3  */
#define VECTOR_093      default_isr     /* 0xb6 Interrupt no. 91 (Unused)  - ivINT_CMP2  */
#define VECTOR_094      default_isr     /* 0xb8 Interrupt no. 92 (Unused)  - ivINT_CMP1  */
#define VECTOR_095      default_isr     /* 0xba Interrupt no. 93 (Unused)  - ivINT_CMP0  */
#define VECTOR_096      default_isr     /* 0xbc Interrupt no. 94 (Unused)  - ivINT_PIT1  */
#define VECTOR_097      can0Recieve_isr/* 0xbe Interrupt no. 95 (Unused)  - ivINT_PIT0  */
#define VECTOR_098      default_isr     /* 0xc0 Interrupt no. 96 (Unused)  - ivINT_PDB1  */
#define VECTOR_099      default_isr     /* 0xc2 Interrupt no. 97 (Unused)  - ivINT_PDB0  */
#define VECTOR_100      default_isr     /* 0xc4 Interrupt no. 98 (Unused)  - ivINT_QDC_INDEX ENC */
#define VECTOR_101      default_isr     /* 0xc6 Interrupt no. 99 (Unused)  - ivINT_QDC_HOME_DOG ENC*/
#define VECTOR_102      default_isr     /* 0xc8 Interrupt no. 100 (Unused) - ivINT_QDC_COMP ENC */
#define VECTOR_103      default_isr     /* 0xca Interrupt no. 101 (Unused) - ivINT_GPIOG  */
#define VECTOR_104      can1Recieve_isr/* 0xcc Interrupt no. 102 (Unused) - ivINT_GPIOF  */
#define VECTOR_105      default_isr     /* 0xce Interrupt no. 103 (Unused) - ivINT_GPIOE  */
#define VECTOR_106      default_isr     /* 0xd0 Interrupt no. 104 (Unused) - ivINT_GPIOD  */
#define VECTOR_107      default_isr     /* 0xd2 Interrupt no. 105 (Unused) - ivINT_GPIOC  */
#define VECTOR_108      default_isr     /* 0xd4 Interrupt no. 106 (Unused) - ivINT_GPIOB  */
#define VECTOR_109      default_isr     /* 0xd6 Interrupt no. 107 (Unused) - ivINT_GPIOA  */
#define VECTOR_110      default_isr     /* 0xd8 Interrupt no. 108 (Unused) - ivINT_COP_INT  */
#define VECTOR_111      can2Recieve_isr/* 0xda Interrupt no. 109 (Unused) - ivINT_EWM_INT  */
#define VECTOR_112      default_isr     /* 0xdc Interrupt no. 110 (Unused) - ivINT_SWILP  */
#define VECTOR_113      default_isr     // 0x0000_01C4 113   97    
#define VECTOR_114      default_isr     // 0x0000_01C8 114   98    
#define VECTOR_115      default_isr     // 0x0000_01CC 115   99    
#define VECTOR_116      default_isr     // 0x0000_01D0 116   100
#define VECTOR_117      default_isr     // 0x0000_01D4 117   101
#define VECTOR_118      default_isr     // 0x0000_01D8 118   102
#define VECTOR_119      default_isr     // 0x0000_01DC 119   103
#define VECTOR_120      default_isr     // 
#define VECTOR_121      default_isr     // 
#define VECTOR_122      default_isr     // 
#define VECTOR_123      default_isr     // 
#define VECTOR_124      default_isr     // 
#define VECTOR_125      default_isr     // 
#define VECTOR_126      default_isr     // 
#define VECTOR_127      default_isr     // 
#define VECTOR_128      default_isr     // 
#define VECTOR_129      default_isr     // 
#define VECTOR_130      default_isr     // 
#define VECTOR_131      default_isr     // 
#define VECTOR_132      default_isr     // 
#define VECTOR_133      default_isr     // 
#define VECTOR_134      default_isr     // 
#define VECTOR_135      default_isr     // 
#define VECTOR_136      default_isr     // 
#define VECTOR_137      default_isr     // 
#define VECTOR_138      default_isr     // 
#define VECTOR_139      default_isr     // 
#define VECTOR_140      default_isr     // 
#define VECTOR_141      default_isr     // 
#define VECTOR_142      default_isr     // 
#define VECTOR_143      default_isr     // 
#define VECTOR_144      default_isr     // 
#define VECTOR_145      default_isr     // 
#define VECTOR_146      default_isr     // 
#define VECTOR_147      default_isr     // 
#define VECTOR_148      default_isr     // 
#define VECTOR_149      default_isr     // 
#define VECTOR_150      default_isr     // 
#define VECTOR_151      default_isr     // 
#define VECTOR_152      default_isr     // 
#define VECTOR_153      default_isr     // 
#define VECTOR_154      default_isr     // 
#define VECTOR_155      default_isr     // 
#define VECTOR_156      default_isr     // 
#define VECTOR_157      default_isr     // 
#define VECTOR_158      default_isr     // 
#define VECTOR_159      default_isr     // 
#define VECTOR_160      default_isr     // 
#define VECTOR_161      default_isr     // 
#define VECTOR_162      default_isr     // 
#define VECTOR_163      default_isr     // 
#define VECTOR_164      default_isr     // 
#define VECTOR_165      default_isr     // 
#define VECTOR_166      default_isr     // 
#define VECTOR_167      default_isr     // 
#define VECTOR_168      default_isr     // 
#define VECTOR_169      default_isr     // 
#define VECTOR_170      default_isr     // 
#define VECTOR_171      default_isr     // 
#define VECTOR_172      default_isr     // 
#define VECTOR_173      default_isr     // 
#define VECTOR_174      default_isr     // 
#define VECTOR_175      default_isr     // 
#define VECTOR_176      default_isr     // 
#define VECTOR_177      default_isr     // 
#define VECTOR_178      default_isr     // 
#define VECTOR_179      default_isr     // 
#define VECTOR_180      default_isr     // 
#define VECTOR_181      default_isr     // 
#define VECTOR_182      default_isr     // 
#define VECTOR_183      default_isr     // 
#define VECTOR_184      default_isr     // 
#define VECTOR_185      default_isr     // 
#define VECTOR_186      default_isr     // 
#define VECTOR_187      default_isr     // 
#define VECTOR_188      default_isr     // 
#define VECTOR_189      default_isr     // 
#define VECTOR_190      default_isr     // 
#define VECTOR_191      default_isr     // 
#define VECTOR_192      default_isr     // 
#define VECTOR_193      default_isr     // 
#define VECTOR_194      default_isr     // 
#define VECTOR_195      default_isr     // 
#define VECTOR_196      default_isr     // 
#define VECTOR_197      default_isr     // 
#define VECTOR_198      default_isr     // 
#define VECTOR_199      default_isr     // 
#define VECTOR_200      default_isr     // 
#define VECTOR_201      default_isr     // 
#define VECTOR_202      default_isr     // 
#define VECTOR_203      default_isr     // 
#define VECTOR_204      default_isr     // 
#define VECTOR_205      default_isr     // 
#define VECTOR_206      default_isr     // 
#define VECTOR_207      default_isr     // 
#define VECTOR_208      default_isr     // 
#define VECTOR_209      default_isr     // 
#define VECTOR_210      default_isr     // 
#define VECTOR_211      default_isr     // 
#define VECTOR_212      default_isr     // 
#define VECTOR_213      default_isr     // 
#define VECTOR_214      default_isr     // 
#define VECTOR_215      default_isr     // 
#define VECTOR_216      default_isr     // 
#define VECTOR_217      default_isr     // 
#define VECTOR_218      default_isr     // 
#define VECTOR_219      default_isr     // 
#define VECTOR_220      default_isr     // 
#define VECTOR_221      default_isr     // 
#define VECTOR_222      default_isr     // 
#define VECTOR_223      default_isr     // 
#define VECTOR_224      default_isr     // 
#define VECTOR_225      default_isr     // 
#define VECTOR_226      default_isr     // 
#define VECTOR_227      default_isr     // 
#define VECTOR_228      default_isr     // 
#define VECTOR_229      default_isr     // 
#define VECTOR_230      default_isr     // 
#define VECTOR_231      default_isr     // 
#define VECTOR_232      default_isr     // 
#define VECTOR_233      default_isr     // 
#define VECTOR_234      default_isr     // 
#define VECTOR_235      default_isr     // 
#define VECTOR_236      default_isr     // 
#define VECTOR_237      default_isr     // 
#define VECTOR_238      default_isr     // 
#define VECTOR_239      default_isr     // 
#define VECTOR_240      default_isr     // 
#define VECTOR_241      default_isr     // 
#define VECTOR_242      default_isr     // 
#define VECTOR_243      default_isr     // 
#define VECTOR_244      default_isr     // 
#define VECTOR_245      default_isr     // 
#define VECTOR_246      default_isr     // 
#define VECTOR_247      default_isr     // 
#define VECTOR_248      default_isr     // 
#define VECTOR_249      default_isr     // 
#define VECTOR_250      default_isr     // 
#define VECTOR_251      default_isr     // 
#define VECTOR_252      default_isr     // 
#define VECTOR_253      default_isr     // 
#define VECTOR_254      default_isr     // 
#define VECTOR_255      default_isr     // 

/* Flash configuration field values below */
/* Please be careful when modifying any of
 * the values below as it can secure the 
 * flash (possibly permanently).
 */
#define CONFIG_1		(pointer*)0xffffffff 
#define CONFIG_2		(pointer*)0xffffffff 
#define CONFIG_3		(pointer*)0xffffffff
#define CONFIG_4		(pointer*)0xffff3ffe

#endif /*__VECTORS_H*/

/* End of "vectors.h" */

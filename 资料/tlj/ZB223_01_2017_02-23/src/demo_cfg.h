/****************************************************************************
 Copyright (c) 2013-2014 Freescale Semiconductor, Inc.
 ALL RIGHTS RESERVED.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, 
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFL                        *
*                                                                       *
* FILE NAME     :  demo_cfg.h                                           *
* DATE          :  April 08, 2014                                         *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  r56611@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.1.0       04.23.2013      FPT Team        Initial Version
1.0.0       12.25.2013      FPT Team        Finalize to version 1.0.0
1.0.2       08.04.2014      FPT Team        Update to follow SDK convention(MISRA-C)
*************************************************************************/

#ifndef _DEMO_H_
#define _DEMO_H_

#ifdef __GNUC__
	#include "derivative.h"
#else
	#include "common.h"
#endif

#define BUFFER_SIZE_BYTE          0x100U

#define EE_ENABLE                 0x00U
#define RAM_ENABLE                0xFFU
#define DEBUGENABLE               0x00U

#define PSECTOR_SIZE              0x00001000U    /* 4 KB size */
#define DSECTOR_SIZE              0x00000800U    /* 2 KB size */

/* FTFL module base */
#define FTFx_REG_BASE             0x40020000U
#define PFLASH_BLOCK_BASE         0x00000000U
#define DEFLASH_BLOCK_BASE        0x10000000U  
#define EERAM_BLOCK_BASE          0x14000000U

#define PBLOCK_SIZE               0x00080000U     /* 512 KB size */
#define EERAM_BLOCK_SIZE          0x00001000U     /* 4 KB size */

#define PBLOCK_NUM                1             /* number of individual Pflash block */
#define DBLOCK_NUM                1             /* number of individual Dflash block */

/* destination to program security key back to flash location */
#define SECURITY_LOCATION         0x408U
#define BACKDOOR_KEY_LOCATION     0x400U

#define PFLASH_IFR                0x3C0U         /*Program flash IFR map*/
#define DFLASH_IFR                0x3F8U         /*Data flash IFR map*/

#define EEE_DATA_SIZE_CODE        0x33U
#define DE_PARTITION_CODE         0x03U

#define PASS                      0x00U
#define FAIL                      0xFFU

#define READ_NORMAL_MARGIN        0x00U
#define READ_USER_MARGIN          0x01U
#define READ_FACTORY_MARGIN       0x02U

#define PSWAP_INDICATOR_ADDR      0x2000U
#define PSWAP_LOWERDATA_ADDR      (PSWAP_INDICATOR_ADDR + PSECTOR_SIZE) 
#define PSWAP_UPPERDATA_ADDR      (PSWAP_LOWERDATA_ADDR + PBLOCK_SIZE/2 + PSECTOR_SIZE)

/* swap state in FCCOB5*/
#define FTFx_SWAP_STATE_UNINIT       0x00U
#define FTFx_SWAP_STATE_READY        0x01U
#define FTFx_SWAP_STATE_UPDATE       0x02U
#define FTFx_SWAP_STATE_UPDATE_ERS   0x03U
#define FTFx_SWAP_STATE_COMPLETE     0x04U

#define CC_ISR_NUM                34
#define RDCOL_ISR_NUM             35

/* Cache disable macro */
#define CACHE_DISABLE     MSCM_OCMDR0 |= MSCM_OCMDR_OCMC0(0xF) | MSCM_OCMDR_OCMC1(0xF); \
			  MSCM_OCMDR1 |= MSCM_OCMDR_OCMC0(0xF) | MSCM_OCMDR_OCMC1(0xF);

void ErrorTrap(uint32_t ret);

#endif /* _DEMO_H_ */

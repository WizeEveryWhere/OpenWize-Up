/**
  * @file bsp_boot.h
  * @brief: This file define functions to reboot and get last boot state
  * 
  * @details
  *
  * @copyright 2019, GRDF, Inc.  All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted (subject to the limitations in the disclaimer
  * below) provided that the following conditions are met:
  *    - Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *    - Redistributions in binary form must reproduce the above copyright 
  *      notice, this list of conditions and the following disclaimer in the 
  *      documentation and/or other materials provided with the distribution.
  *    - Neither the name of GRDF, Inc. nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  *
  * @par Revision history
  *
  * @par 1.0.0 : 2020/10/13 [GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup boot
 *  @ingroup bsp
 *  @{
 */

#ifndef _BSP_BOOT_H_
#define _BSP_BOOT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/*!
 * @cond INTERNAL
 * @{
 */

#ifndef BUILD_STANDALONE_APP
	#include "img.h"
#endif

#ifndef COLD_RESET
	#define COLD_RESET    0x0C // Cold reset (NRST pin or power lost)
#endif

#ifndef WARM_RESET
	#define WARM_RESET    0x10 // Warm reset (SW)
#endif

#ifndef INSTAB_DETECT
	#define INSTAB_DETECT 0x60 // Instability detected 'Application or other)
#endif

#ifndef UNAUTH_ACCESS
	#define UNAUTH_ACCESS 0x83 /* Unauthorized action :
 	 	 	  * - access to protected areas
 	 	 	  * - entering into forbidden low power mode (Standby, Stop, Shutdown)
 	 	 	  * - reset option byte
 	 	 	  */
#endif

// ----
#ifndef BOOT_STATE_BKPR
	#define BOOT_STATE_BKPR BKP30R
#endif

#ifndef BOOT_INFO_BKPR
	#define BOOT_INFO_BKPR BKP31R
#endif

/*
#ifndef BOOT_REASON_MSK
#define BOOT_REASON_MSK     0x00000FF
#endif

#ifndef STANDBY_WKUP_MSK
#define STANDBY_WKUP_MSK    0x0000100
#endif

#ifndef BACKUP_RESET_MSK
#define BACKUP_RESET_MSK    0x0000200
#endif

#ifndef INTERNAL_WKUP_MSK
#define INTERNAL_WKUP_MSK   0x0000400
#endif

#ifndef WKUP_PIN_MSK
#define WKUP_PIN_MSK        0x001F000
#endif

#ifndef WKUP_PIN_POS
#define WKUP_PIN_POS        0x0C
#endif
*/
#define ALARMA_WKUP_MSK     0x0100000
#define ALARMB_WKUP_MSK     0x0200000
#define TIMER_WKUP_MSK      0x0400000
#define CALENDAR_INIT_MSK   0x0800000

/*!
 * @}
 * @endcond
 */


/*!
 * @brief This enum define the reset (boot) reason as is in mcu reset register
 */
typedef enum
{
	FW_RSTF   = 0x01, /*!< Firewall reset*/
	OBL_RSTF  = 0x02, /*!< Option Byte reset*/
	PIN_RSTF  = 0x04, /*!< NRST Pin reset */
	BOR_RSTF  = 0x08, /*!< Power On Reset (power was lost or exit from Shutdown/Stdby LP)*/
	SFT_RSTF  = 0x10, /*!< Software reset */
	IWWG_RSTF = 0x20, /*!< Independent window watchdog reset */
	WWDG_RSTF = 0x40, /*!< Window watchdog reset */
	LPWR_RSTF = 0x80, /*!< Illegal Low power reset*/
} boot_reason_e;

/*!
 * @brief This union hold the boot state
 */
typedef union {
	uint32_t info; /*!< Boot Info  */
	struct
	{
		uint32_t reason:8;     /*!< boot reason (see @link boot_reason_e @endlink) */
		uint32_t wkup_pin:5;   /*!< wake-up from pin */
		uint32_t standby:1;    /*!< wake-up from standby */
		uint32_t internal:1;   /*!< wake-up from internal */
		uint32_t backup:1;     /*!< backup-up domain has been cleared */
		uint32_t unstab_cnt:8;  /*!< unstable boot count */
		uint32_t unauth_cnt:8;  /*!< unauthorized boot count */
	};
} boot_info_t;


typedef union
{
	uint32_t state; /*!< Boot counters  */
	struct
	{
		uint32_t wkup_alra:1;  /*!< wake-up from rtc alarm A */
		uint32_t wkup_alrb:1;  /*!< wake-up from rtc alarm B */
		uint32_t wkup_timer:1; /*!< wake-up from rtc wake-up timer */
		uint32_t calendar:1;   /*!< calendar configuration required */
	};
} boot_state_t;

void BSP_Boot_Reboot(uint8_t bReset);

uint32_t BSP_Boot_GetInfo(void);
void BSP_Boot_SetInfo(uint32_t info);

uint32_t BSP_Boot_GetState(void);
void BSP_Boot_SetSate(uint32_t state);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_BOOT_H_ */

/*! @} */

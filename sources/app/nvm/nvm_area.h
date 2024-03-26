
/*!
 * @addtogroup nvm
 * @ingroup app
 * @{
 */

#ifndef _NVM_AREA_H_
#define _NVM_AREA_H_

#include <stdint.h>

#include "flash_storage.h"
#include "crypto.h"
#include "key_priv.h"
#include "phy_layer_private.h"
#include "net_api.h"
#include "parameters_cfg.h"

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */
extern unsigned int __nvm_org__;
extern unsigned int __nvm_size__;

/*!
 * @brief Define the hard-coded flash address for the (NVM) storage area
 */
#define NVM_ORG (unsigned int)(&(__nvm_org__))
#define NVM_SIZE (unsigned int)(__nvm_size__)

#define HEADER_SZ          sizeof(struct flash_store_header_s)
#define KEY_TABLE_SZ       KEY_MAX_NB * sizeof(key_s)
#define CALIBRATION_SZ     sizeof(struct _store_special_s)
#define PARAMETER_TABLE_SZ PARAM_DEFAULT_SZ

struct _store_special_s
{
	device_id_t sDeviceInfo;
	uint8_t     bPaState;
#ifdef USE_PHY_POWER_RAMP
	uint8_t     ePaRampRate;
#else
	uint8_t     ND1;
#endif
	int16_t     i16PhyRssiOffset;
	phy_power_t aPhyPower[PHY_NB_PWR];
	uint8_t     ND2[3];
	uint8_t     aPhyCalRes[CAL_RES_SZ] __attribute__ ((aligned(8)));
	uint8_t     ND3[4];
};

/*!
 * @}
 * @endcond
 */

/******************************************************************************/

#endif /* _NVM_AREA_H_ */
/*! @} */

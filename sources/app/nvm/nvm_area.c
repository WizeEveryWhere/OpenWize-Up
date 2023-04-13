
#include "flash_storage.h"
#include "crypto.h"
#include "key_priv.h"
#include "phy_layer_private.h"
#include "net_api.h"
#include "parameters_cfg.h"

struct _store_special_s
{
	device_id_t sDeviceInfo;
	uint8_t     bPaState;
	uint8_t     ND1;
	int16_t     i16PhyRssiOffset;
	phy_power_t aPhyPower[PHY_NB_PWR];
	uint8_t     ND2[3];
	uint8_t     aPhyCalRes[CAL_RES_SZ] __attribute__ ((aligned(8)));
	uint8_t     ND3[4];
};

/******************************************************************************/
extern unsigned int __nvm_org__;
#define NVM_ORG (unsigned int)(&(__nvm_org__))
#define ATTR_SECTION(msection) __attribute__(( section(msection) ))
#define ATTR_AREA() __attribute__((used)) __attribute__(( aligned (8) ))
#define ATTR_PART() __attribute__(( aligned (2048) ))
#define ATTR_PARAM_TABLE() ATTR_SECTION(".nvm") ATTR_AREA()

#define HEADER_SZ          sizeof(struct flash_store_header_s)
#define KEY_TABLE_SZ       KEY_MAX_NB * sizeof(key_s)
#define CALIBRATION_SZ     sizeof(struct _store_special_s)
#define PARAMETER_TABLE_SZ PARAM_DEFAULT_SZ

#define START_ADDRESS_PART_1 (NVM_ORG + HEADER_SZ)
#define START_ADDRESS_PART_2 (START_ADDRESS_PART_1 + KEY_TABLE_SZ)
#define START_ADDRESS_PART_3 (START_ADDRESS_PART_2 + CALIBRATION_SZ)

#define INDEX_PART_1 0
#define INDEX_PART_2 START_ADDRESS_PART_2 - START_ADDRESS_PART_1
#define INDEX_PART_3 START_ADDRESS_PART_3 - START_ADDRESS_PART_2

/******************************************************************************/
/******************************************************************************/
// don't add any in NVM data before
ATTR_PART()
ATTR_AREA()
ATTR_SECTION(".nvm")
const struct flash_store_header_s sHeader =
{
	.u16Status = 0,
	.u16Crc = 0,
	.u32PartAddr =
	{
		START_ADDRESS_PART_1,
		START_ADDRESS_PART_2 ,
		START_ADDRESS_PART_3 ,
	}
};
// don't add any data in NVM between sHeader and sKey
ATTR_AREA()
ATTR_SECTION(".nvm")
const key_s sKey[KEY_MAX_NB] =
{
	[0] = {
	.key = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	}},
	[KEY_ENC_MIN+1] = {
	.key = {
		0x55, 0x22, 0x19, 0xe2, 0x65, 0xeb, 0xb4, 0x8c,
		0x8a, 0xdf, 0x58, 0x71, 0x79, 0xd9, 0xc6, 0xb0,
		0x63, 0xd5, 0x7c, 0xa8, 0xd3, 0x7e, 0xd6, 0xcb,
		0x11, 0x51, 0xa7, 0x59, 0xcc, 0xad, 0xba, 0x40
	}},
	[KEY_MAC_ID] = {
	.key = {
		0x88, 0xe3, 0x35, 0x63, 0x8f, 0x52, 0x19, 0x46,
		0xc3, 0x8e, 0x32, 0xee, 0xba, 0xa3, 0xc9, 0x9f,
		0x4a, 0xe7, 0x0b, 0xfb, 0x2b, 0xb2, 0x53, 0x40,
		0x25, 0x04, 0x85, 0x76, 0xe3, 0x81, 0xfe, 0xad
	}},
	[KEY_CHG_ID] = {
	.key = {
		0x51, 0x15, 0x1a, 0xb6, 0xa7, 0x47, 0x6b, 0xb1,
		0x53, 0x44, 0x61, 0xdf, 0x67, 0xa0, 0x8b, 0x04,
		0x38, 0x8c, 0xb2, 0x83, 0x96, 0x9c, 0xae, 0x27,
		0x68, 0xe1, 0x2d, 0x0d, 0x83, 0xa4, 0x89, 0xbe
	}}
};
// don't add any data in NVM between sKey and sCalibration
ATTR_AREA()
ATTR_SECTION(".nvm")
const struct _store_special_s sCalibration =
{
	.sDeviceInfo =
	{
		.aManuf = { 0xFF, 0xFF },
		.aNum = {0x00, 0x00, 0x00, 0x00},
		.u8Ver = 0x00,
		.u8Type = 0x00
	},
	.bPaState = 0,
	.i16PhyRssiOffset = 0,
	.aPhyPower =
	{
		[PHY_PMAX_minus_0db]  = {.coarse = 6, .fine = 20, .micro = 0}, //   0 dBm
		[PHY_PMAX_minus_6db]  = {.coarse = 6, .fine =  6, .micro = 0}, //  -6 dBm
		[PHY_PMAX_minus_12db] = {.coarse = 6, .fine =  3, .micro = 0}, // -12 dBm
	},
	.aPhyCalRes =
	{
		0, 0, 0, 0, 0, 0, 0, 0
	},
};
// don't add any data in NVM between sCalibration and aParamSetup
// don't move or remove it
#include "parameters_setup.c"

/******************************************************************************/

/*
 * TODO : Assert, that at end of link, the following is true
 * sHeader.u32PartAddr[0] == sKey
 * sHeader.u32PartAddr[1] == sCalibration
 * sHeader.u32PartAddr[2] == aParamSetup
 */

/******************************************************************************/
// just for gcc happiness
void Reset_Handler(void){ }
void main(void) {}
/******************************************************************************/

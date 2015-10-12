/**
 *******************************************************************************
 * @file	drv_BQ2060A.h
 * @author	j.daheron
 * @version	1.0.0
 * @date	02 dec. 2013
 * @brief   Registres du BMS BQ2060A.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DRV_BQ2060A_H_
#define DRV_BQ2060A_H_


/* Includes ------------------------------------------------------------------*/

#include "../BSP.h"


/* Exported types ------------------------------------------------------------*/


/* Exported constants --------------------------------------------------------*/

#define BQ2060A_SLAVE_ADDRESS		0x16

#define BQ2060A_CMD_ManufacturerAccess			0x00
#define BQ2060A_CMD_RemainingCapacityAlarm		0x01
#define BQ2060A_CMD_RemainingTimeAlarm			0x02
#define BQ2060A_CMD_BatteryMode					0x03
#define BQ2060A_CMD_AtRate						0x04
#define BQ2060A_CMD_AtRateTimeToFull			0x05
#define BQ2060A_CMD_AtRateTimeToEmpty			0x06
#define BQ2060A_CMD_AtRateOK					0x07
#define BQ2060A_CMD_Temperature					0x08
#define BQ2060A_CMD_Voltage						0x09
#define BQ2060A_CMD_Current						0x0A
#define BQ2060A_CMD_AverageCurrent				0x0B
#define BQ2060A_CMD_MaxError					0x0C
#define BQ2060A_CMD_RelativeStateOfCharge		0x0D
#define BQ2060A_CMD_AbsoluteStateOfCharge		0x0E
#define BQ2060A_CMD_RemainingCapacity			0x0F
#define BQ2060A_CMD_FullChargeCapacity			0x10
#define BQ2060A_CMD_RunTimeToEmpty				0x11
#define BQ2060A_CMD_AverageTimeToEmpty			0x12
#define BQ2060A_CMD_AverageTimeToFull			0x13
#define BQ2060A_CMD_ChargingCurrent				0x14
#define BQ2060A_CMD_ChargingVoltage				0x15
#define BQ2060A_CMD_BatteryStatus				0x16
#define BQ2060A_CMD_CycleCount					0x17
#define BQ2060A_CMD_DesignCapacity				0x18
#define BQ2060A_CMD_DesignVoltage				0x19
#define BQ2060A_CMD_SpecificationInfo			0x1A
#define BQ2060A_CMD_ManufactureDate				0x1B
#define BQ2060A_CMD_SerialNumber				0x1C
#define BQ2060A_CMD_DeviceName					0x21
#define BQ2060A_CMD_DeviceChemistry				0x22
#define BQ2060A_CMD_ManufacturerData			0x23
#define BQ2060A_CMD_PackStatus					0x2F
#define BQ2060A_CMD_PackConfiguration			0x2F
#define BQ2060A_CMD_VCELL4						0x3C
#define BQ2060A_CMD_VCELL3						0x3D
#define BQ2060A_CMD_VCELL2						0x3E
#define BQ2060A_CMD_VCELL1						0x3F


/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */


#endif /* DRV_BQ2060A_H_ */





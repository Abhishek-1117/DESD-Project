/* pmSensor header file */
#ifndef PM_SENSOR_H
#define PM_SENSOR_H

#include "supp_components.h"

#define PM_SLAVE_ADDR               0x19            // PM Sensor slave I2C address

#define PM_SLAVE_VERSION            0x1D            // PM slave firmware version
#define PM_SLAVE_LOW_POWER_REG      0x01            // Low power mode enable-disable register
#define PM_SLAVE_LOW_POWER_ENABLE   0x01
#define PM_SLAVE_LOW_POWER_DISABLE  0x02

#define PM_SLAVE_PM1_0_ATMOSPHERE   0x0B
#define PM_SLAVE_PM2_5_ATMOSPHERE   0x0D
#define PM_SLAVE_PM10_ATMOSPHERE    0x0F

#define PM_SLAVE_0_3_UM_EVERY0_1L_AIR    0x11
#define PM_SLAVE_0_5_UM_EVERY0_1L_AIR    0x13
#define PM_SLAVE_1_0_UM_EVERY0_1L_AIR    0x15
#define PM_SLAVE_2_5_UM_EVERY0_1L_AIR    0x17
#define PM_SLAVE_5_0_UM_EVERY0_1L_AIR    0x19
#define PM_SLAVE_10_UM_EVERY0_1L_AIR     0x1B

uint8_t pm_version (void);
void pm_lowPower (void);
void pm_wakeUp (void);
uint16_t pm_measure (uint8_t);

#endif              // pmSensor header file ends
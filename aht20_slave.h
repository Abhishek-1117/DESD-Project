/* AHT20 slave header */
#ifndef AHT20_SLAVE_H
#define AHT20_SLAVE_H

#include "supp_components.h"

#define AHT20_SLAVE_ADDR        0x38            // AHT20 slave I2C address

#define AHT20_STATUS_REG        0x71            // Address of status register of AHT20

/* Initialization command and its parameters */
#define AHT20_INIT_CMD          0xBE
#define AHT20_INIT_CMD_PARA1    0x08
#define AHT20_INIT_CMD_PARA2    0x00

/* AHT20 measure command and its parameters */
#define AHT20_MEASURE_CMD       0xAC
#define AHT20_MEASURE_CMD_PARA1 0x33
#define AHT20_MEASURE_CMD_PARA2 0x00

void aht20_init(void);
void aht20_measure(float*, float*);
uint8_t read_aht20_status_reg(void);

#endif      // AHT20 slave header ends
/* I2C Master init header */
#ifndef I2C_MASTER_INIT_H
#define I2C_MASTER_INIT_H

#include "supp_components.h"

/* I2C master configuration parameters */
#define I2C_MASTER_SCL_IO 22                        // I2C SCL Pin
#define I2C_MASTER_SDA_IO 21                        // I2C SDA Pin
#define I2C_MASTER_TXBUF_DISABLE 0                  // Master doesn't have Tx buffer
#define I2C_MASTER_RXBUF_DISABLE 0                  // Master doesn't have Rx buffer
#define I2C_MASTER_FREQ_HZ 100000                   // I2C clock frequency: 100 kH
#define I2C_MASTER_CONTROLLER 0                     // I2C Controller 0

/* I2C bus acknowledgement parameters */
#define ACK_CHECK_ENABLE 0X01
#define ACK_VALUE 0X00                              // Acknowledgement value
#define NACK_VALUE 0X01                             // Nack value
#define INTR_HANDLER_DISABLE 0X00                   // No interrupt is used

void i2c_master_init(void);
esp_err_t write_to_i2c_bus (const uint8_t, const uint8_t*, uint8_t);
esp_err_t read_from_i2c_bus (const uint8_t, uint8_t*, uint8_t);

#endif      // I2C Master init header ends
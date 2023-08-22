
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "pmSensor_slave.h"
#include "i2c_master.h"

uint8_t pm_version (void)
{
    uint8_t transmit_buf[]= {PM_SLAVE_VERSION};
    uint8_t receive_buf[1]= {0};

    err= write_to_i2c_bus(PM_SLAVE_ADDR, transmit_buf, sizeof(transmit_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    err= read_from_i2c_bus(PM_SLAVE_ADDR, receive_buf, sizeof(receive_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    return receive_buf[0];
}

void pm_lowPower (void)
{
    uint8_t transmit_buf[]= {PM_SLAVE_LOW_POWER_REG,
                             PM_SLAVE_LOW_POWER_ENABLE};

    err= write_to_i2c_bus(PM_SLAVE_ADDR, transmit_buf, sizeof(transmit_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
}

void pm_wakeUp (void)
{
    uint8_t transmit_buf[]= {PM_SLAVE_LOW_POWER_REG,
                             PM_SLAVE_LOW_POWER_DISABLE};

    err= write_to_i2c_bus(PM_SLAVE_ADDR, transmit_buf, sizeof(transmit_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
}

uint16_t pm_measure (uint8_t pm_type)
{
    uint8_t transmit_buf[]= {pm_type};
    uint8_t receive_buf[2]= {0};

    err= write_to_i2c_bus(PM_SLAVE_ADDR, transmit_buf, sizeof(transmit_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    err= read_from_i2c_bus(PM_SLAVE_ADDR, receive_buf, sizeof(receive_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
    
    return (uint16_t)((receive_buf[0] << 8) | receive_buf[1]);
}

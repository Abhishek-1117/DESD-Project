
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "aht20_slave.h"
#include "i2c_master.h"

void aht20_init(void)
{
    uint8_t status_reg, cal_bit;
    uint8_t init_cmds_buf[]= { AHT20_INIT_CMD,
                               AHT20_INIT_CMD_PARA1,
                               AHT20_INIT_CMD_PARA2};
    

    /* Checking calibration enable bit */
    status_reg= read_aht20_status_reg();
    cal_bit= (status_reg >> 3) & 0x01;
    ESP_LOGI(TAG, "Calibration Enable bit is: %d", cal_bit);


    /* Initialize AHT20 sensor */
    /* If calibration enable bit is not set (0), then the suggested parameters
     * has to be sent after initialization command */
    if(cal_bit) {
        err= write_to_i2c_bus(AHT20_SLAVE_ADDR, init_cmds_buf, 1);
    }
    else {
        err= write_to_i2c_bus(AHT20_SLAVE_ADDR, init_cmds_buf, sizeof(init_cmds_buf));
    }
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);        // minimum suggested delay/wait after initialization
}

void aht20_measure(float *temperature_p, float *humidity_p)
{
    uint8_t status_reg, busy_indi_bit= 1;
    uint32_t raw_data;
    uint8_t measure_buf[]= { AHT20_MEASURE_CMD,
                             AHT20_MEASURE_CMD_PARA1,
                             AHT20_MEASURE_CMD_PARA2};
    uint8_t rawData_buf[7]= {0};


    /* Sending measurement command to AHT20 */
    err= write_to_i2c_bus(AHT20_SLAVE_ADDR, measure_buf, sizeof(measure_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    vTaskDelay(80 / portTICK_PERIOD_MS);                // minimux delay/wait for measurements to be completed

    /* Checking for Busy indication bit if the measurements are completed or not */
    /* Busy indication bit resetted (0) indicates measurement has been completed */
    while(busy_indi_bit) {
        status_reg= read_aht20_status_reg();
        busy_indi_bit= (status_reg >> 7) & 0x01;
    }

    /* Reading the 7 bytes from aht20 for temperature and humidity readings */
    err= read_from_i2c_bus(AHT20_SLAVE_ADDR, rawData_buf, sizeof(rawData_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    /* Appling given formulas on raw data to obtain measurements */
    raw_data = ((uint32_t)rawData_buf[1] << 12) | ((uint32_t)rawData_buf[2] << 4) | (rawData_buf[3] >> 4);
    *humidity_p = (float)raw_data * 100 / 0x100000;

    raw_data = ((uint32_t)(rawData_buf[3] & 0x0f) << 16) | ((uint32_t)rawData_buf[4] << 8) | rawData_buf[5];
    *temperature_p = (float)raw_data * 200 / 0x100000 - 50;
}

uint8_t read_aht20_status_reg (void)
{
    uint8_t transmit_buf[]= {AHT20_STATUS_REG};
    uint8_t receive_buf[1]= {0};

    err= write_to_i2c_bus(AHT20_SLAVE_ADDR, transmit_buf, sizeof(transmit_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
    
    err= read_from_i2c_bus(AHT20_SLAVE_ADDR, receive_buf, sizeof(receive_buf));
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    return receive_buf[0];
}

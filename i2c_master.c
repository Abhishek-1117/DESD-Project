
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"

#include "i2c_master.h"

void i2c_master_init(void)
{
    i2c_config_t master_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,        // Internal pull-up enable
        .scl_pullup_en = GPIO_PULLUP_ENABLE,        // Internal pull-up enable
        .master.clk_speed= I2C_MASTER_FREQ_HZ,
        .clk_flags= 0
    };

    err = i2c_param_config(I2C_MASTER_CONTROLLER, &master_config);
    if (err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
    
    err= i2c_driver_install(I2C_MASTER_CONTROLLER,
                            master_config.mode,
                            I2C_MASTER_RXBUF_DISABLE,
                            I2C_MASTER_TXBUF_DISABLE,
                            INTR_HANDLER_DISABLE);
    if (err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
}


esp_err_t write_to_i2c_bus (const uint8_t slave_add, const uint8_t *data_p, uint8_t len)
{
    esp_err_t ret;
    i2c_cmd_handle_t cmd;

    cmd= i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_add << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE);
    i2c_master_write(cmd, data_p, len, ACK_CHECK_ENABLE);
    i2c_master_stop(cmd);
    ret= i2c_master_cmd_begin(I2C_MASTER_CONTROLLER, cmd, 1000/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    
    return ret;
}

esp_err_t read_from_i2c_bus (const uint8_t slave_add, uint8_t *data_p, uint8_t len)
{
    esp_err_t ret;
    i2c_cmd_handle_t cmd;

    cmd= i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_add << 1) | I2C_MASTER_READ, ACK_CHECK_ENABLE);
    if(len > 1) {
        i2c_master_read(cmd, data_p, len-1, ACK_VALUE);
    }
    i2c_master_read_byte(cmd, data_p + len-1, NACK_VALUE);
    i2c_master_stop(cmd);
    ret= i2c_master_cmd_begin(I2C_MASTER_CONTROLLER, cmd, 1000/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    return ret;
}

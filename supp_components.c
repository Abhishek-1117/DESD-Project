
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "supp_components.h"

/* @brief For error handling */
void stop_at_error (esp_err_t err_code, uint32_t line, const char *file)
{
    if(err_code != ESP_OK) {
        ESP_LOGE(TAG, "ERROR OCCURRED!");
        ESP_LOGE(TAG, "Error name: %s,", esp_err_to_name(err_code));
        ESP_LOGE(TAG, "in file: %s,", file);
        ESP_LOGE(TAG, "at line: %d", (int)line);
        for (uint8_t i= 5; i>0; i--) {
            ESP_LOGE(TAG, "Rebooting in %ds\n", i);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGE(TAG, "REBOOTING NOW...\n");
        abort();
    }
}
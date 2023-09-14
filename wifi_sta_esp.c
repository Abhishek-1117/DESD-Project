
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "wifi_sta_esp.h"

static uint8_t WIFI_CONNECTED_ONCE;                 // to check whether esp has connected to wifi even once
static uint8_t num_of_retry;                        // number of retries to get connect to AP before resetting


void wifi_event_handler (void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT) {
        switch (event_id)
        {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "Initiating to establish a wifi connection in the station mode");
                stop_at_error(esp_wifi_connect(), __LINE__, __FILE__);
                break;

            case WIFI_EVENT_STA_CONNECTED:
                xEventGroupClearBits(connection_event_group, WIFI_DISCONNECTED);
                xEventGroupSetBits(connection_event_group, WIFI_CONNECTED);
                ESP_LOGI(TAG, "Connection established in station mode with AP");
                WIFI_CONNECTED_ONCE= 1;
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                xEventGroupClearBits(connection_event_group, WIFI_CONNECTED);
                if (WIFI_CONNECTED_ONCE) {
                    xEventGroupSetBits(connection_event_group, WIFI_DISCONNECTED);
                }
                if(num_of_retry < MAXIMUM_WIFI_RETRIES) {
                    if (!WIFI_CONNECTED_ONCE) {
                        ESP_LOGI(TAG, "Failed to connect, retrying after a second...");
                    }
                    else {
                        ESP_LOGI(TAG, "Disconnected, retrying after a second...");
                    }
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    stop_at_error(esp_wifi_connect(), __LINE__, __FILE__);
                    num_of_retry++;
                }
                else {
                    xEventGroupSetBits(connection_event_group, WIFI_DISCONNECTED);
                    ESP_LOGE(TAG, "Unable to connect to the AP with SSID: %s & password: ITS A SECRET", AP_SSID);
                    ESP_LOGW(TAG, "Check whether your AP is turned on");
                }
                break;

            default:
                ESP_LOGW(TAG, "Uncared Wifi event occured with event ID: %d", (int)event_id);
        }
    }

    else if(event_base == IP_EVENT) {
        switch (event_id)
        {
            case IP_EVENT_STA_GOT_IP:
                ip_event_got_ip_t *event= (ip_event_got_ip_t*)event_data;
                ESP_LOGI(TAG, "Acquired the ip address: " IPSTR, IP2STR(&event->ip_info.ip));
                ESP_LOGI(TAG, "Connected to the AP with SSID: %s & password: ITS A SECRET", AP_SSID);
                num_of_retry= 0;
                break;

            default:
                ESP_LOGW(TAG, "Uncared IP event occured with event ID: %d", (int)event_id);
        }
    }
}

void wifi_sta_init(void)
{
    /* Creating event loop for wifi */
    stop_at_error(esp_event_loop_create_default(), __LINE__, __FILE__);

    /* Registering wifi event callback funciton */
    esp_event_handler_instance_register(WIFI_EVENT,             // event_base
                                        ESP_EVENT_ANY_ID,       // event_id
                                        &wifi_event_handler,    // handler_callback
                                        NULL,                   // handler_arguments
                                        NULL);                  // event_instance
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    /* Initializing required hardware and settings */
    stop_at_error(esp_netif_init(), __LINE__, __FILE__);        // Initializing lwIP and creating lwIP task
    esp_netif_create_default_wifi_sta();                        // Default Initialization of wifi station mode

    /* Configuring wifi stack and Initializing wifi task */
    wifi_init_config_t wifi_init_config= WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    /* Setting wifi in station mode */
    wifi_config_t wifi_sta_config= {
        .sta= {
            .ssid= AP_SSID,
            .password= AP_PASS,
            .threshold.authmode= WIFI_AUTH_WPA2_WPA3_PSK,
            .sae_pwe_h2e= WPA3_SAE_PWE_BOTH,
        },
    };

    stop_at_error(esp_wifi_set_mode(WIFI_MODE_STA), __LINE__, __FILE__);
    stop_at_error(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config), __LINE__, __FILE__);
    stop_at_error(esp_wifi_start(), __LINE__, __FILE__);
    ESP_LOGI(TAG, "wifi configuration completed successfully");
    ESP_LOGI(TAG, "Starting WiFi...");
}

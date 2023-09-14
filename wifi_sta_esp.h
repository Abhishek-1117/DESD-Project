/* Wifi station mode header file */
#ifndef WIFI_STA_ESP_H
#define WIFI_STA_ESP_H

#include "supp_components.h"
#include "esp_event.h"

#define AP_SSID                         CONFIG_AP_SSID
#define AP_PASS                         CONFIG_AP_PASS
#define MAXIMUM_WIFI_RETRIES            CONFIG_MAXIMUM_WIFI_RETRIES

#define WIFI_CONNECTED      BIT0        // this event bit is set if wifi is connected
#define WIFI_DISCONNECTED   BIT1        // this event bit is set if wifi is disconnected

void wifi_event_handler (void*, esp_event_base_t, int32_t, void*);
void wifi_sta_init(void);

#endif

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"      // delays and tasks
#include "freertos/task.h"          // tasks
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"  // freertos event group
#include "esp_system.h"
#include "esp_event.h"              // Event group
#include "esp_log.h"                // Logging
#include "esp_wifi.h"               // wifi

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "mqtt_publish.h"

esp_mqtt_client_handle_t client= NULL;


void mqtt_init (void)
{
    esp_mqtt_client_config_t mqttConfig = {
        .broker.address= {
            .hostname= BROKER_IP,
            .transport= MQTT_TRANSPORT_OVER_TCP,
        },
    };
    
    /* Configuring mqtt for esp32 */
    client = esp_mqtt_client_init(&mqttConfig);

    /* Registering mqtt handler funciton for mqtt events */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, &mqtt_event_handler, client);
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }

    /* Initializing mqtt for esp32 */
    err= esp_mqtt_client_start(client);
    if(err != ESP_OK) {
        stop_at_error(err, __LINE__, __FILE__);
    }
}

void mqtt_event_handler (void *args, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event= event_data;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT connected");
            xEventGroupClearBits(connection_event_group, MQTT_DISCONNECTED);
            xEventGroupSetBits(connection_event_group, MQTT_CONNECTED);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT disconnected");
            xEventGroupClearBits(connection_event_group, MQTT_CONNECTED);
            xEventGroupSetBits(connection_event_group, MQTT_DISCONNECTED);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT published, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT error occurred");
            break;

        default:
            ESP_LOGW(TAG, "Uncared mqtt event occurred with event id: %d", event->event_id);
    }
}

/* MQTT Publish header file */
#ifndef MQTT_PUBLISH_H
#define MQTT_PUBLISH_H

#include "esp_event.h"
#include "mqtt_client.h"

#include "supp_components.h"

#define BROKER_IP           CONFIG_BROKER_IP

#define MQTT_CONNECTED      BIT2        // this event bit is set if MQTT is connected
#define MQTT_DISCONNECTED   BIT3        // this event bit is set if MQTT is disconnected

extern esp_mqtt_client_handle_t client;

void mqtt_init (void);
void mqtt_event_handler (void*, esp_event_base_t, int32_t, void*);

#endif              // MQTT Publish headre file ends
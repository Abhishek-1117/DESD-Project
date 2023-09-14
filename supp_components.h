/* Project3 header file */
#ifndef SUPP_COMPONENTS_H
#define SUPP_COMPONENTS_H

#include "esp_err.h"
#include "freertos/event_groups.h"

#include "project3_config.h"

extern esp_err_t err;
extern const char *TAG;
extern EventGroupHandle_t connection_event_group;

void stop_at_error (esp_err_t ret, uint32_t, const char*);

#endif              // Project3 header file ends
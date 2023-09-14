idf_component_register (SRCS "wifi_sta_esp.c"
                        INCLUDE_DIRS "."
                        REQUIRES i2c_master esp_event esp_wifi)
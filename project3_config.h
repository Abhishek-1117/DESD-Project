/* Project3 configuration header */
#ifndef PROJECT3_CONFIG_H
#define PROJECT3_CONFIG_H

/* Configuration for wifi station mode */
#define CONFIG_AP_SSID                  "V"       // configure your wifi's ssid
#define CONFIG_AP_PASS                  "123456789"            // configure your wifi's password
#define CONFIG_MAXIMUM_WIFI_RETRIES     05                      // configure no. of retries

/* Configuration for MQTT protocol */
#define CONFIG_BROKER_IP                "192.168.58.32"        // configure broker ip here

/* Configuration for project3 main file */
#define CONFIG_ESP_NAME                         "PROJECT3_ESP32"    // ESP Node name
#define CONFIG_SENSOR_FREQ_MEASURE              5000                // configure frequency of sensor measurement
#define CONFIG_MQTT_PUBLISH_BUFF_SIZE           512                 // configure mqtt publish buffer size
#define CONFIG_SIZE_OF_TASK_QUEUE               32                  // configure size of task queue to publisher task

#endif          // Project3 configuration header ends
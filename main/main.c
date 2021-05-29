#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "station/station.h"

void app_main(void)
{
    // connect to Wi-Fi
    app_station();
}
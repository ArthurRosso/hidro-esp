#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"


#include <esp_http_server.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <unistd.h>
#include "esp_timer.h"
#include "esp_sleep.h"

#include "esp_http_client.h"

#include "station.h"

static const char *TAG = "HTTP_CLIENT";

void custom_sleep(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

float get_medicao()
{
    return esp_timer_get_time()/3.14;
}

esp_err_t post_data (){

    esp_err_t err;
    char response[200];
    wifi_config_t stationConfig;
    char strmac[17]="";

    esp_http_client_config_t config = {
        .url = "https://hidro-esp-srv.herokuapp.com/insert.php",
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_wifi_get_config(ESP_IF_WIFI_STA, &stationConfig);
    for(int j = 0; j< 6; j++){
      char str[3];
      sprintf(str, "%02x", (int)stationConfig.sta.bssid[j]);
      strcat(strmac, str);

      if(j<5){
        strcat(strmac, ":");
      }
    }

    snprintf(response, sizeof(response), "{\"Mac\": \"%s\", \"Litros\": %f}", strmac, get_medicao());

    //const char *post_data = "{\"field1\":\"value1\"}";
    
    esp_http_client_set_url(client, "https://hidro-esp-srv.herokuapp.com/insert.php");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, response, strlen(response));
    err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    return(err);
}

void app_main(void)
{
    // connect to internet
    ESP_ERROR_CHECK(app_station());

    while (1){
       ESP_ERROR_CHECK(post_data());
       custom_sleep(2000); // 2 segundos
    }
}
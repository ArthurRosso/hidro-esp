#include "routes.h"

static float get_medicao()
{
    return esp_timer_get_time()/3.14;
}

esp_err_t get_status_handler(httpd_req_t *req)
{
    char response[200];
    wifi_config_t stationConfig;
    char strmac[17]="";
    
    esp_wifi_get_config(ESP_IF_WIFI_STA, &stationConfig);
    for(int j = 0; j< 6; j++){
      char str[3];
      sprintf(str, "%02x", (int)stationConfig.sta.bssid[j]);
      strcat(strmac, str);

      if(j<5){
        strcat(strmac, ":");
      }
    }


    httpd_resp_set_hdr(req, "Content-Type", "application/json");

    snprintf(response, sizeof(response), "{\"Mac\": %s \"Litros\": %f}", strmac, get_medicao());

    return httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
}
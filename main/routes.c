#include "routes.h"
#include "esp_wifi.h"
#include <esp_wifi_types.h>
/*
static char *convert(uint8_t *a)
{
  char* buffer2;
  int i;

  buffer2 = malloc(9);
  if (!buffer2)
    return NULL;

  buffer2[8] = 0;
  for (i = 0; i <= 7; i++)
    buffer2[7 - i] = (((*a) >> i) & (0x01)) + '0';

  return buffer2;
}*/

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

    snprintf(response, sizeof(response), "{\"Mac\": %s \"Litros\": %f}", strmac, 27.45);

    return httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
}
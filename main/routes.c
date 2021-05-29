#include "routes.h"

esp_err_t get_status_handler(httpd_req_t *req)
{
    char response[200];

    httpd_resp_set_hdr(req, "Content-Type", "application/json");

    snprintf(response, sizeof(response), "{ \"Litros\": %f}", 27.45);

    return httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
}
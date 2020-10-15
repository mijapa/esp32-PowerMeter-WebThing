#include <ArduinoOTA.h>
#include "esp_log.h"

static const char *TAG = "ArduinoOTA";

void handleOTA() {
    ArduinoOTA.handle();
}

void setupOTA() {
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname("esp32-PowerMeter-WebThing");

    // No authentication by default
    ArduinoOTA.setPassword((const char *) "123456");

    ArduinoOTA.onStart([]() {
        ESP_LOGI(TAG, "Start");
    });
    ArduinoOTA.onEnd([]() {
        ESP_LOGI(TAG, "\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        ESP_LOGI(TAG, "Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        ESP_LOGE(TAG, "Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) ESP_LOGE(TAG, "Auth Failed");
        else if (error == OTA_BEGIN_ERROR) ESP_LOGE(TAG, "Begin Failed");
        else if (error == OTA_CONNECT_ERROR) ESP_LOGE(TAG, "Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) ESP_LOGE(TAG, "Receive Failed");
        else if (error == OTA_END_ERROR) ESP_LOGE(TAG, "End Failed");
        ESP.restart();
    });

    ArduinoOTA.begin();
    ESP_LOGI(TAG, "OTA ready");
    ESP_LOGI(TAG, "IP address: %s", WiFi.localIP());
}
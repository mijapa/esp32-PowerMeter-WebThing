#include <ESPAsyncWiFiManager.h>
#include <Ticker.h>
#include <ESPmDNS.h>
#include "wifiManager.h"
#include "esp_log.h"

static const char *TAG = "WiFiManager";

Ticker ledTicker;

void blink() {
    //toggle state
    int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
    digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

void configModeCallback(AsyncWiFiManager *myWiFiManager) {
    ESP_LOGI(TAG, "Entered config mode %s", WiFi.softAPIP());
    //if you used auto generated SSID, print it
    ESP_LOGI(TAG, "SSID: %s", myWiFiManager->getConfigPortalSSID());
    //entered config mode, make led toggle faster
    ledTicker.attach_ms(100, blink);
}

void setupWiFi() {
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    AsyncWebServer server(80);
    DNSServer dns;
    AsyncWiFiManager wifiManager(&server, &dns);
    //reset settings - for testing
    //wifiManager.resetSettings();

    //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wifiManager.setAPCallback(configModeCallback);

    wifiManager.setConfigPortalTimeout(180);

    //fetches ssid and pass and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    if (!wifiManager.autoConnect("Power Meter", "Stove@5")) {
        ledTicker.detach();
        ESP_LOGW(TAG, "failed to connect and hit timeout");
        return;
//        //reset and try again, or maybe put it to deep sleep
//        ESP.restart();
//        delay(1000);
    }

    ESP_LOGI(TAG, "Connected to %s", WiFi.BSSIDstr());
    ESP_LOGI(TAG, "IP address: %s", WiFi.localIP());
    ledTicker.detach();
}

void tryConnectWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        ESP_LOGI(TAG, "tryConnectWiFi");
        //WiFiManager
        //Local intialization. Once its business is done, there is no need to keep it around
        AsyncWebServer server(80);
        DNSServer dns;
        AsyncWiFiManager wifiManager(&server, &dns);
        wifiManager.setConfigPortalTimeout(1);
        wifiManager.autoConnect("Power Meter", "Stove@5");
    }
}
#include <Arduino.h>
#include <Ticker.h>
#include "wifiManager.h"
#include "webthings.h"
#include "ota.h"
#include "esp_log.h"
#include "power.h"

static const char *TAG = "Main";

Ticker printTicker;
bool timeToPrint = false;

void printReady() {
    timeToPrint = true;
}

void loopPrint() {
    if (timeToPrint) {
        timeToPrint = false;
        displayPower();
    }
}


void setup() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);


    setupWiFi();
    setupOTA();
    setupPower();
    setupWebThing();


    printTicker.attach(1, printReady);
}

void loop() {
    handleOTA();
    loopPrint();
    loopWebThing();

    tryConnectWiFi();
}

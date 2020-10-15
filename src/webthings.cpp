#define SMALL_JSON_DOCUMENT_SIZE 1024
#define LARGE_JSON_DOCUMENT_SIZE 9216
#define ARDUINOJSON_USE_LONG_LONG 1

#include <WebThingAdapter.h>
#include <Ticker.h>
#include "webthings.h"
#include "esp_log.h"

static const char *TAG = "WebThings";

WebThingAdapter *adapter;

const char *meterSensorTypes[] = {"EnergyMonitor", "MultiLevelSensor", "TextDisplay", "Sensor", nullptr};
ThingDevice meter("powerMeter", "Energy Monitor", meterSensorTypes);

String uptimeString = "";
ThingProperty textProperty("uptext", "Uptime text", STRING, nullptr);
ThingProperty levelProperty("uplevel", "Uptime level in seconds", NUMBER, "LevelProperty");
ThingProperty currentProperty("Current", "The RMS level of current", NUMBER, "CurrentProperty");
ThingProperty powerProperty("Power", "Instant Power of AC", NUMBER, "InstantaneousPowerProperty");

bool webThingReady = false;
bool timeToResetup = true;
Ticker updateWebThingTicker;
Ticker adapterPresentTicker;

void timeToCheckPresence() {
    timeToResetup = true;
}

int isAdapterPresent() {
    if (!adapter) {
        ESP_LOGW(TAG, "No WebThing adapter");
        if (timeToResetup) {
            timeToResetup = false;
            adapterPresentTicker.once(5, timeToCheckPresence);
            ESP_LOGW(TAG, "Resetup");
            setupWebThing();
        }
        return 0;
    }
    return 1;
}

void updateTextWebThing() {
    if (!isAdapterPresent()) {
        return;
    }
    long secsUp = millis() / 1000;
    long seconds = secsUp % 60;
    long minutes = (secsUp / 60) % 60;
    long hours = (secsUp / (60 * 60)) % 24;
    long days = secsUp / (60 * 60 * 24);

    uptimeString = "";
    uptimeString += days;
    uptimeString += " dni, ";
    uptimeString += hours;
    uptimeString += " godzin, ";
    uptimeString += minutes;
    uptimeString += " minut, ";
    uptimeString += seconds;
    uptimeString += " sekund";

    ThingPropertyValue value;
    value.string = &uptimeString;
    textProperty.setValue(value);

    value.number = secsUp;
    levelProperty.setValue(value);
}

void timeToUpdateWebThing() {
    webThingReady = true;
}

void setupWebThing() {
    if (WiFi.localIP()) {
        adapter = new WebThingAdapter("ESP32-PowerMeter", WiFi.localIP());

        textProperty.readOnly = true;
        textProperty.title = "Uptime";
        meter.addProperty(&textProperty);
        levelProperty.readOnly = true;
        levelProperty.title = "Uptime";
        levelProperty.unit = "s";
        meter.addProperty(&levelProperty);
        meter.addProperty(&currentProperty);
        meter.addProperty(&powerProperty);

        adapter->addDevice(&meter);

        adapter->begin();

        ESP_LOGI(TAG, "WebThing HTTP server started");
        ESP_LOGI(TAG, "http://%s", WiFi.localIP());
    } else {
        ESP_LOGW(TAG, "No local IP");
    }
    updateWebThingTicker.attach_ms(1000, timeToUpdateWebThing);
}

void loopWebThing() {
    if (webThingReady) {
        if (!isAdapterPresent()) {
            return;
        }
        webThingReady = false;

        updateTextWebThing();
        adapter->update();
    }
}

void updatePowerWebThing(double amps, double watts) {
    if (!isAdapterPresent()) {
        return;
    }
    ThingPropertyValue value;
    value.number = amps;
    currentProperty.setValue(value);
    value.number = watts;
    powerProperty.setValue(value);
}
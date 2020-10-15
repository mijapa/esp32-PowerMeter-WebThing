#include "power.h"
#include "EmonLib.h"
#include "esp_log.h"
#include "webthings.h"

#define ADC_INPUT 34
#define HOME_VOLTAGE 230
#define WATT_THRESHOLD 30

static const char *TAG = "Power";

EnergyMonitor emon1;

void setupPower(){
    emon1.current(ADC_INPUT, 40);
}

void displayPower(){
    double amps = emon1.calcIrms(1480); // Calculate Irms only (number of samples)
    double watts = amps * HOME_VOLTAGE;
    if (watts < WATT_THRESHOLD){
        watts = 0;
    }
    ESP_LOGI(TAG, "%fA, %fW", amps, watts);
    ESP_LOGI(TAG, "analog read %d", analogRead(ADC_INPUT));
    updatePowerWebThing(amps, watts);
}
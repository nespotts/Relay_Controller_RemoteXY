long currenttime = 0;

int relay1_pin = 16;
int relay2_pin = 14;
int relay3_pin = 12; 
int relay4_pin = 13; 

int relay1_state = 0;
int relay2_state = 0;
int relay3_state = 0;
int relay4_state = 0;

float solar1_temp = 0;
float solar2_temp = 0;
float solar3_temp = 0;
float prev_max_solar_temp = 0;
float max_solar_temp = 0;

float prev_electrical_cabinet_temp = 0;
float electrical_cabinet_temp = 0;


// ****************************** JLED *************************************
#include <jled.h>
// breathe LED for 5 times, LED is connected to pin 9 (PWM capable) gpio
auto led_on = JLed(LED_BUILTIN).Breathe(3000).Forever();
auto led_off = JLed(LED_BUILTIN).LowActive().Off();
// *************************************************************************


// ****************************** ADS1115 **********************************
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
int adc_reading_count = 0;
float inverter_amp_sum = 0;
float inverter_amps = 0;
float dc_dc_amp_sum = 0;
float dc_dc_amps = 0;
// *************************************************************************


// ***************************** Thermistor ********************************
#include <Thermistor.h>
#include <NTC_Thermistor.h>
// sensor goes between A0 and Gnd, pullup goes between A0 and 3.3V
#define SENSOR_PIN             A0
#define REFERENCE_RESISTANCE   4610
#define NOMINAL_RESISTANCE     10000
#define NOMINAL_TEMPERATURE    25
#define B_VALUE                3455

Thermistor* thermistor;
float thermistor_sum = 0;
int thermistor_count = 0;
float thermistor_f = 0;
// *************************************************************************


#include <ESP8266WiFi.h>
#include "network.h"
#include "RemoteXY_functions.h"
#include "RemoteXY_callbacks.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include "OTA.h"

#include "functions.h"


void setup()
{
  // Debug console
  Serial.begin(115200);
  led_off.Update();

  pinMode(LED_BUILTIN, OUTPUT);
  setupRelays();
  ConnectWifi();

  WiFi.mode(WIFI_STA);

  OTA_Functions();

  setupADS1115();
  setupThermistor();
}


void loop()
{
  manageBlynkConnection2();
  led_on.Update();
  ArduinoOTA.handle();
  currenttime = millis();

  // read all data every 250ms
  get_all_data();

  // send inverter data every 500ms
  sendInverterData();

  // read sensors every 10ms
  readSensors();
}



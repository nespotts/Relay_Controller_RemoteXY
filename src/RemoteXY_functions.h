
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>



String mainURL ="http://192.168.8.146:8000";
JsonDocument doc;


long get_data_timer = 0;
long get_data_interval = 500; // ms


// convert float to char array
String floatToCharArray(float val, int decimal_places=3) {
  char temp[32];
  dtostrf(val, 0, decimal_places, temp);
  return String(temp);
}



void set_value(String key, float value) {
  WiFiClient client;
  HTTPClient http;

  // Serial.print("[HTTP] begin...");

  if (http.begin(client, mainURL + "/set_value/" + key + "/" + value)) {  // HTTP
    // Serial.print("[HTTP] GET...");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      // Serial.printf("[HTTP] GET code: %d", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        // deserializeJson(doc, payload);
        // Serial.println(payload);
      }
    } else {
      // Serial.printf("[HTTP] GET failed, error: %s", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    // Serial.printf("[HTTP} Unable to connect");
  }
}


void get_all_data() {
  if ((currenttime - get_data_timer) < get_data_interval) {
    return;
  }
  get_data_timer = currenttime;

  WiFiClient client;
  HTTPClient http;

  // Serial.print("[HTTP] begin...");

  if (http.begin(client, mainURL + "/get_all")) {  // HTTP
    // Serial.print("[HTTP] GET...");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET code: %d", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();
        deserializeJson(doc, payload);

        relay1_state = doc["water_heater"];
        relay2_state = doc["inverter"];
        relay3_state = doc["solar_fan"];
        relay4_state = doc["exhaust_fan"];

        solar1_temp = doc["solar1_contoller_temp"];
        solar2_temp = doc["solar2_contoller_temp"];
        solar3_temp = doc["solar3_contoller_temp"];
        // get max of the solar temps
        max_solar_temp = max(solar1_temp, max(solar2_temp, solar3_temp));

        // automate solar fans
        if (max_solar_temp >= 115 && prev_max_solar_temp < 115) {
          // turn on fans
          set_value("solar_fan", 1);
        } else if (max_solar_temp < 105 && prev_max_solar_temp >= 105) {
          // turn off fans
          set_value("solar_fan", 0);
        }
        prev_max_solar_temp = max_solar_temp;


        // automate exhaust fan
        if (electrical_cabinet_temp >= 95 && prev_electrical_cabinet_temp < 95) {
          // turn on fans
          set_value("exhaust_fan", 1);
        } else if (electrical_cabinet_temp < 93 && prev_electrical_cabinet_temp >= 93) {
          // turn off fans
          set_value("exhaust_fan", 0);
        }
        prev_electrical_cabinet_temp = electrical_cabinet_temp;


        // set relays
        if (relay1_state == 1) {
          digitalWrite(relay1_pin, HIGH);
        } else {
          digitalWrite(relay1_pin, LOW);
        }

        if (relay2_state == 1) {
          digitalWrite(relay2_pin, HIGH);
        } else {
          digitalWrite(relay2_pin, LOW);
        }

        if (relay3_state == 1) {
          digitalWrite(relay3_pin, HIGH);
        } else {
          digitalWrite(relay3_pin, LOW);
        }

        if (relay4_state == 1) {
          digitalWrite(relay4_pin, HIGH);
        } else {
          digitalWrite(relay4_pin, LOW);
        }

      }
    } else {
      Serial.printf("[HTTP] GET failed, error: %s", http.errorToString(httpCode).c_str());
    }

    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect");
  }
}





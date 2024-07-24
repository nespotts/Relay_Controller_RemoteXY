
// apply changes for App Outputs - only need to do this for controls
void RemoteXY_callbacks() {
  // if (Sensors.set_temp != RemoteXY.set_temp) {
  //   Serial.println("setting temperature value");
  //   set_value("set_temp_display", String(RemoteXY.set_temp));
  //   set_value("set_temp", String(RemoteXY.set_temp));
  //   // set_value("Temp", String(RemoteXY.Temp));
  //   // RemoteXY.Temp = Sensors.Temp;
  //   Sensors = RemoteXY;
  // }


}


// force pins to sync on a regular basis
// void syncPins() {
//   // Serial.println("Syncing virtual pins");
//   Blynk.syncVirtual(V2, V73, V74, V75, V76);
// }

// BLYNK_WRITE(V73) {
// 	relay1_state = param.asInt();
// 	digitalWrite(relay1_pin, relay1_state);
// }
// BLYNK_WRITE(V74) {
// 	relay2_state = param.asInt();
// 	digitalWrite(relay2_pin, relay2_state);
// }
// BLYNK_WRITE(V75) {
// 	relay3_state = param.asInt();
// 	digitalWrite(relay3_pin, relay3_state);
// }
// BLYNK_WRITE(V76) {
// 	relay4_state = param.asInt();
// 	digitalWrite(relay4_pin, relay4_state);
// }


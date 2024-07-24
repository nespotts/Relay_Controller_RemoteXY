void setupRelays() {
  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);
  digitalWrite(relay1_pin, LOW);
  digitalWrite(relay2_pin, LOW);
  digitalWrite(relay3_pin, LOW);
  digitalWrite(relay4_pin, LOW);
}


void setupThermistor() {
  thermistor = new NTC_Thermistor(
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE
  );
}


void setupADS1115() {
  ads.setGain(GAIN_SIXTEEN);    // 16x gain +/- 0.256V 1 bit = 0.125mV 0.0078125mV
  if (!ads.begin(0x48)) {
    Serial.println("Failed to initialize ADS.");
    // force reboot
    while (1);
  }
}


void readADC() {
  int16_t results, results2;

  results = ads.readADC_Differential_0_1();
  // Serial.println(results);
  // 400 amps = 75/256 * 32768bits = 9600bits
  inverter_amp_sum += (float)results * (400.0 / ((75.0/256.0)*32768.0)); // 75mV / 400A shunt

  results2 = ads.readADC_Differential_2_3();
  dc_dc_amp_sum += (float)results2 * (100.0 / ((100.0/256.0)*32768.0)); // 75mV / 400A shunt

  adc_reading_count += 1;
  // Serial.println(amps);
}


void readThermistor() {
	// const double celsius = thermistor->readCelsius();
	// const double kelvin = thermistor->readKelvin();
	thermistor_sum += thermistor->readFahrenheit();
	thermistor_count += 1;	

	// Output of information
	// Serial.print("Temperature: ");
	// Serial.print(celsius);
	// Serial.print(" C, ");
	// Serial.print(kelvin);
	// Serial.print(" K, ");
	// Serial.print(fahrenheit);
	// Serial.println(" F");
}

void readSensors() {
	readADC();
	readThermistor();
}

long send_data_timer = 0;
long send_data_interval = 500; // ms


void sendInverterData() {
  if (currenttime - send_data_timer < send_data_interval) {
    return;
  }
  send_data_timer = currenttime;

	thermistor_f = thermistor_sum / (float)thermistor_count;
	thermistor_sum = 0;
	thermistor_count = 0;

	inverter_amps = inverter_amp_sum / (float)adc_reading_count;
	inverter_amp_sum = 0;

  dc_dc_amps = dc_dc_amp_sum / (float)adc_reading_count;
  dc_dc_amp_sum = 0;

	adc_reading_count = 0;

	Serial.println(thermistor_f);
	Serial.println(inverter_amps);
  Serial.println(dc_dc_amps);


  set_value("inverter_temp", thermistor_f);
  set_value("inverter_current", inverter_amps);
  set_value("dc_dc_current", dc_dc_amps);
}


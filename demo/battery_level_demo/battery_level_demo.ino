#define BATTERY_ADC_PIN 35

#define ADC_MAX 4095.0 // based on 12bit ADC
#define V_REF 3.3  // ADC reference voltage

// Calibrated ADC voltage range
#define V_ADC_FULL 3.13   // Observed at 4.2V (values calculated based on observed values and max is extrapolated)
#define V_ADC_EMPTY 2.13  // Estimated at 3.0V (values calcualted based on observed values and min is extrapolated)

void setup() {
  Serial.begin(115200);
}

void loop() {
  int adc = analogRead(BATTERY_ADC_PIN);
  float v_adc = (adc / ADC_MAX) * V_REF;

  // Linear mapping of voltage to percentage (reduced down to 3.3V)
  float percent = (v_adc - V_ADC_EMPTY) / (V_ADC_FULL - V_ADC_EMPTY) * 100.0;
  percent = constrain(percent, 0, 100);

  // Output all key values
  Serial.print("ADC: ");
  Serial.print(adc);
  Serial.print(" | V_ADC: ");
  Serial.print(v_adc, 3); // this is reduced to 3.3V
  Serial.print(" V | Battery: ");
  Serial.print(percent, 1);
  Serial.println(" %");

  delay(1000);
}

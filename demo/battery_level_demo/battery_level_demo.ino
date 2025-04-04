#define BATTERY_ADC_PIN 35

#define ADC_MAX 4095.0 // based on 12bit ADC
#define V_REF 3.3  // ADC reference voltage

// Battery voltage limits after voltage divider
#define V_ADC_FULL 3.3     // Corresponds to 4.2V battery
#define V_ADC_EMPTY 2.91   // Corresponds to 3.7V battery

void setup() {
  Serial.begin(115200);
}

void loop() {
  int adc = analogRead(BATTERY_ADC_PIN);
  float v_adc = (adc / ADC_MAX) * V_REF;

  // Linear mapping of voltage to percentage
  float percent = (v_adc - V_ADC_EMPTY) / (V_ADC_FULL - V_ADC_EMPTY) * 100.0;
  percent = constrain(percent, 0, 100);

  Serial.print("V_ADC: ");
  Serial.print(v_adc, 3);
  Serial.print(" V | Battery: ");
  Serial.print(percent, 1);
  Serial.println(" %");

  delay(1000);
}
// HEATER
#define mosfet_pin PA6
#define led PC15

// NTC
#define ntc_pin PA0                // Pin,to which the voltage divider is connected
#define vd_power_pin PA4           // 5V for the voltage divider
#define nominal_resistance 150000  // Nominal resistance at 25⁰C
#define nominal_temeprature 25     // temperature for nominal resistance (almost always 25⁰ C)
#define samplingrate 5             // Number of samples
#define beta 3950                  // The beta coefficient or the B value of the thermistor (usually 3000-4000) check the datasheet for the accurate value.
#define Rref 100520                // Value of  resistor used for the voltage divider
int samples = 10;                  // array to store the samples

void setup(void) {
  pinMode(mosfet_pin, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(ntc_pin, INPUT);
  pinMode(vd_power_pin, OUTPUT);
  // Serial.setRx(PA_10);
  // Serial.setTx(PA_9);
  Serial.begin(115200);  // initialize serial communication at a baud rate of 9600
  analogReadResolution(12);
  analogWriteFrequency(5);
}

#define error_ADC 0

int target_temp = 55;
#define KP 30
int time1 = 0;

void loop(void) {
  // int incomingByte = 0;  // for incoming serial data
  // if (Serial.available() > 0) {
  //   // read the incoming byte:
  //   incomingByte = Serial.read();
  //   target_temp = incomingByte;
  //   // say what you got:
  //   Serial.print("Temp setted to: ");
  //   Serial.println(incomingByte, DEC);
  // }


  if (millis() - time1 >= 500) {
    int real_temp = getTemperature(samples);
    Serial.println(real_temp);
    if (target_temp > real_temp) {
      digitalWrite(led, 1);
      int e = target_temp - real_temp;
      e = e * KP;
      if (e > 255) e = 255;
      analogWrite(mosfet_pin, e);
    } else {
      analogWrite(mosfet_pin, 0);
    }
    time1 = millis();
  }
}


float getTemperature(uint8_t readings) {
  uint8_t i;
  float average;
  samples = 0;
  // take voltage readings from the voltage divider
  digitalWrite(vd_power_pin, HIGH);
  for (i = 0; i < samplingrate; i++) {
    samples += analogRead(ntc_pin) + error_ADC;
    delay(10);
  }
  digitalWrite(vd_power_pin, LOW);
  average = 0;
  average = samples / samplingrate;

  // Calculate NTC resistance
  average = 4096 / average - 1;
  average = Rref / average;
  float temperature;
  temperature = average / nominal_resistance;           // (R/Ro)
  temperature = log(temperature);                       // ln(R/Ro)
  temperature /= beta;                                  // 1/B * ln(R/Ro)
  temperature += 1.0 / (nominal_temeprature + 273.15);  // + (1/To)
  temperature = 1.0 / temperature;                      // Invert
  temperature -= 273.15;                                // convert absolute temp to C
  return temperature - 13;
}

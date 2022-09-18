#define ThermistorPin A0
int Vo;
float R1 = 10000;
float R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
   Serial.begin(9600);
}

void loop() {

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  T = (1.0 / (c1 + (c2 * log(R2)) + (c3 * log(R2) * log(R2) * log(R2)))) - 273.15;

  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.println(" °C");

  delay(500);
}


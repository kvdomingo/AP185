float Vin = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  Vin = analogRead(A0);
  Serial.print(millis());
  Serial.print(",");
  Serial.print(Vin);
  Serial.println();
}

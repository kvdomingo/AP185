int Vin = 0;
float Vs = 0;

void setup() {
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
}
 
void loop() {
  Vin = analogRead(A0);
  Vs = Vin/1024. * 3.3;
  Serial.println(Vin);
}

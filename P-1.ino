int ledPin=5;
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  }

void loop() {
  digitalWrite(ledPin,HIGH);
  Serial.print("LED is ON");
  delay(1000);
  digitalWrite(ledPin,HIGH);
  Serial.print("LED is OFF");
  delay(1000);
}

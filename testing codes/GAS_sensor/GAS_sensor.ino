#define MQ135_DO_PIN 18

void setup() {
  Serial.begin(115200);
  pinMode(MQ135_DO_PIN, INPUT);
  Serial.println("MQ2 warming up!");
  delay(20000);
}

void loop() {
  int gasState = digitalRead(MQ135_DO_PIN);

  if (gasState == HIGH)
    Serial.println("The gas is NOT present");
  else
    Serial.println("The gas is present");
  delay(1000);
}
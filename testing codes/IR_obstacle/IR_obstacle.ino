#define IR 33

bool obstacle = false;

void setup() {
  Serial.begin(115200);
  pinMode(IR, INPUT);
}

void loop() {
  obstacle = !digitalRead(IR);
  if (obstacle)
    Serial.println("There is an obstacle");
  else
    Serial.println("There isn't an obstacle");
  delay(1000);
}

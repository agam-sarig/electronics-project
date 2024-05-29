#define ULTRASONIC_TRIG_PIN 14
#define ULTRASONIC_ECHO_PIN 12

#define SOUND_SPEED 0.034

long duration;
float distance;

void setup() {
  Serial.begin(115200);
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  distance = duration * SOUND_SPEED/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(1000);
}


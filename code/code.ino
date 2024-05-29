#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <ESP32Time.h>

const char* ssid = "0rphan";
const char* password = "mooncake";

#define SERVO_PIN 13

#define TRIG_PIN 14
#define ECHO_PIN 12
#define IR_SPOT1_PIN 33
#define IR_SPOT2_PIN 32
#define MQ2_DO_PIN 18

// RGB LEDs for each parking spot
#define RED_SPOT1_PIN 25
#define GREEN_SPOT1_PIN 26
#define BLUE_SPOT1_PIN 27

#define RED_SPOT2_PIN 15
#define GREEN_SPOT2_PIN 2
#define BLUE_SPOT2_PIN 4

Servo servo;
bool spot1Occupied = false, spot2Occupied = false;
bool isOpen;
int carCount = 0; // Counter for cars in the parking lot

ESP32Time rtc(3600);
AsyncWebServer server(80);

void setup() 
{
  Serial.begin(115200);
  servo.attach(SERVO_PIN);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_SPOT1_PIN, INPUT);
  pinMode(IR_SPOT2_PIN, INPUT);
  pinMode(MQ2_DO_PIN, INPUT);

  // Setup RGB LEDs for each spot
  pinMode(RED_SPOT1_PIN, OUTPUT);
  pinMode(GREEN_SPOT1_PIN, OUTPUT);
  pinMode(BLUE_SPOT1_PIN, OUTPUT);
  pinMode(RED_SPOT2_PIN, OUTPUT);
  pinMode(GREEN_SPOT2_PIN, OUTPUT);
  pinMode(BLUE_SPOT2_PIN, OUTPUT);

  resetLED(RED_SPOT1_PIN, GREEN_SPOT1_PIN, BLUE_SPOT1_PIN);
  resetLED(RED_SPOT2_PIN, GREEN_SPOT2_PIN, BLUE_SPOT2_PIN);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    Serial.println("WiFi connected");
    Serial.print("IP address: "); 
    Serial.println(WiFi.localIP());
  }
  
  rtc.setTime(30, 24, 9, 19, 5, 2024);  

  servo.attach(SERVO_PIN);
  closeGate();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String page = "<!DOCTYPE html><html><head><title>Parking System</title></head><body>";
    page += "<h1>Parking System</h1>";
    page += "<p>Gate: ";
    if (isOpen) {
      page += "Open</p>";
    } else {
      page += "Closed</p>";
    }
    page += "<p>Spot 1: ";
    if (spot1Occupied) {
      page += "Occupied</p>";
    } else {
      page += "Vacant</p>";
    }
    page += "<p>Spot 2: ";
    if (spot2Occupied) {
      page += "Occupied</p>";
    } else {
      page += "Vacant</p>";
    }
    page += "<p>Current Time: ";
    page += rtc.getTime("%A, %B %d %Y %H:%M:%S");
    page += "</p>";
    page += "<form action=\"/open\" method=\"get\"><button type=\"submit\">Open Gate</button></form>";
    page += "<form action=\"/close\" method=\"get\"><button type=\"submit\">Close Gate</button></form>";
    page += "</body></html>";
    request->send(200, "text/html", page);
  });

  server.on("/open", HTTP_GET, [](AsyncWebServerRequest *request){
    openGate(); // Open gate
    request->send(200, "text/plain", "Opened");
  });

  server.on("/close", HTTP_GET, [](AsyncWebServerRequest *request){
    closeGate(); // Close gate
    request->send(200, "text/plain", "Closed");
  });

  server.begin();
}

void loop() 
{
  checkGate();
  checkParkingSpot(IR_SPOT1_PIN, spot1Occupied, RED_SPOT1_PIN, GREEN_SPOT1_PIN, BLUE_SPOT1_PIN);
  checkParkingSpot(IR_SPOT2_PIN, spot2Occupied, RED_SPOT2_PIN, GREEN_SPOT2_PIN, BLUE_SPOT2_PIN);
  delay(100);
  Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));  
}

void checkGate() 
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  double duration = pulseIn(ECHO_PIN, HIGH);
  
  int distance = duration * 0.034 / 2;
  Serial.println(distance);
  if (distance < 20 && carCount < 2)
  {
    openGate();
  }
  else
  {
    closeGate();
  }
}

void checkParkingSpot(int irPin, bool &spotOccupied, int redPin, int greenPin, int bluePin) 
{
  bool carPresent = digitalRead(irPin) == HIGH;
  if (!carPresent && !spotOccupied) 
  {
    spotOccupied = true;
    carCount++;
    updateLED(redPin, greenPin, bluePin, true); // Occupied
  }
  else if (carPresent && spotOccupied) 
  {
    spotOccupied = false;
    carCount--;
    updateLED(redPin, greenPin, bluePin, false); // Not occupied
  }
}

void openGate() 
{
  if(!isOpen)
  {
    servo.write(0);
    delay(120);
    servo.write(90);
    isOpen = true;
    Serial.println("open");
  }
}

void closeGate() 
{
  if(isOpen)
  {
    servo.write(180);
    delay(120);
    servo.write(90);
    isOpen = false;
    Serial.println("closed");
  }
}

void resetLED(int redPin, int greenPin, int bluePin)
{
  digitalWrite(redPin, LOW); 
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void updateLED(int redPin, int greenPin, int bluePin, bool occupied) {
  if (occupied) // Red: Occupied
  {
    digitalWrite(redPin, HIGH); 
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  } 
  else  // Green: Vacant
  {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  }

}


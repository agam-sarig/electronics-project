#include <WiFi.h>
#include <ESPAsyncWebServer.h>
const char* ssid = "0rphan";
const char* password = "mooncake";
AsyncWebServer server(80);
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("ESP32 Web Server: New request received:");
    Serial.println("GET /");
    request->send(200, "text/html", "<html><body><h1>Hello, ESP32!</h1></body></html>");});

  server.begin();
}

void loop() {
  // Your code here
}


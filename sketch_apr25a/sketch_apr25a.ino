#include <WiFi.h>
//#include <HTTPClient.h>
//#include <ArduinoJson.h>
//#include <FS.h>
//#include <SPIFFS.h>

const char* ssid = "Hoi Thuy";
const char* password = "26051981";
//const char* serverUrl = "http://192.168.165.221:5000/data";  // IP của server Flask

const int rainSensorPin = 34;  // Cảm biến mưa (chân analog)
const int lightSensorPin = 35; // Cảm biến ánh sáng (chân analog)

// Cấu hình chân điều khiển motor
const int motorIN1 = 26;    // IN1 -> GPIO27
const int motorIN2 = 27;    // IN2 -> GPIO26
const int motorENA = 25;    // ENA -> GPIO25

bool isRoofOpen = false; // Trạng thái mái che, false = đang đóng, true = đang mở

void setup() {
  Serial.begin(115200);

  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
  pinMode(motorENA, OUTPUT);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  //sendDataToServer(true, false, "open");  // Chưa cần gửi
}

/*
// Hàm gửi dữ liệu lên server Flask (Tạm thời chưa dùng)
void sendDataToServer(bool rain, bool light, String roofStatus) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    doc["rain"] = analogRead(rainSensorPin);
    doc["light"] = analogRead(lightSensorPin);
    doc["roof"] = roofStatus;
    doc["timestamp"] = millis();

    String jsonData;
    serializeJson(doc, jsonData);

    int httpResponseCode = http.POST(jsonData);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
}
*/

// Hàm mở mái che
void openRoof() {
  digitalWrite(motorENA, HIGH);    // Bật motor
  digitalWrite(motorIN1, HIGH);    // Quay xuôi
  digitalWrite(motorIN2, LOW);
  delay(5000);                     // Quay 5s

  // Dừng motor
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorENA, LOW);

  //sendDataToServer(true, false, "open");  // Chưa cần gửi
}

// Hàm đóng mái che
void closeRoof() {
  digitalWrite(motorENA, HIGH);    // Bật motor
  digitalWrite(motorIN1, LOW);     // Quay ngược
  digitalWrite(motorIN2, HIGH);
  delay(5000);                     // Quay 5s

  // Dừng motor
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
  digitalWrite(motorENA, LOW);

  //sendDataToServer(false, false, "closed"); // Chưa cần gửi
}

void loop() {
  int rainValue = analogRead(rainSensorPin);
  int lightValue = analogRead(lightSensorPin);

  bool isRaining = rainValue < 3000;     // Ngưỡng mưa
  bool isLight = lightValue > 1500;      // Ngưỡng sáng

  // In thông tin cảm biến ra Serial Monitor
  Serial.println("----------- Sensor Readings -----------");
  Serial.print("Rain Sensor Value  : ");
  Serial.println(rainValue);
  Serial.print("Light Sensor Value : ");
  Serial.println(lightValue);
  Serial.print("Roof Status        : ");
  Serial.println(isRoofOpen ? "OPEN" : "CLOSED");
  Serial.println("---------------------------------------");

  if ((isRaining || isLight) && !isRoofOpen) {
    openRoof();
    isRoofOpen = true;
    Serial.println(">>> Roof opened!");
  } 
  else if ((!isRaining && !isLight) && isRoofOpen) {
    closeRoof();
    isRoofOpen = false;
    Serial.println(">>> Roof closed!");
  }

  delay(10000); // Check mỗi 10 giây
}

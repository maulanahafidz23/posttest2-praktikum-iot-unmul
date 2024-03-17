#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "Xiaomi 12T";    // Nama SSID AP/Hotspot
const char* password = "sebentar";  // Password Wifi

const int BUZZER_PIN = D2;
const int LED_PIN = D3;
const int btnUP = D6;
const int btnDOWN = D7;
const int btnONOFF = D5;

bool on = true;
int LED = 0;
int BUZZER = 100;
int btnUPState = 0;
int btnDOWNState = 0;
int btnONOFFState = 0;

ESP8266WebServer server(80);  // Port untuk web server

void setup() {
  Serial.begin(9600);
  delay(1000);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnDOWN, INPUT_PULLUP);
  pinMode(btnONOFF, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println("Server dijalankan");
}

void loop() {
  server.handleClient();  // Menangani permintaan dari klien
  btnUPState = digitalRead(btnUP);
  btnDOWNState = digitalRead(btnDOWN);
  btnONOFFState = digitalRead(btnONOFF);
  if (btnUPState == LOW) {
    if (LED < 249) {
      LED += 3;
    } 
    if (BUZZER < 2500) {
      BUZZER += 10;
    }
    delay(100);
  }
  if (btnDOWNState == LOW) {
    if (LED > 0){
      LED -= 3;
    }
    if (BUZZER > 100){
      BUZZER -= 10;
    }
    delay(100);
  }
  if (btnONOFFState == LOW) {
    on = !on;
    delay(300);
  }

  if (on){
    analogWrite(LED_PIN, LED);
    tone(BUZZER_PIN, BUZZER);
  } else {
    analogWrite(LED_PIN, 0);
    noTone(BUZZER_PIN);
  }

}

void handleRoot() {
  String content = "<h1> Monitor Value Komponen Kelompok 3 IOT B </h1> <br> <p>Tingkat Kecerahan LED =  " + String(LED) + "</p><br>" + "Tingkat Tone Frekuensi Buzzer = " + String(BUZZER);
  server.send(200, "text/html", content);
}
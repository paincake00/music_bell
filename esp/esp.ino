// #include "SoundData.h"      // Твой массив sample[]
// #include "driver/dac.h"

// #define BOOT_PIN 0          // Кнопка BOOT на ESP32
// #define SAMPLE_RATE_HZ 8000
// #define SAMPLE_DELAY_US (1000000 / SAMPLE_RATE_HZ)

// void setup() {
//   pinMode(BOOT_PIN, INPUT_PULLUP);        // BOOT подтянут к питанию
//   dac_output_enable(DAC_CHANNEL_1);       // GPIO25 (DAC1)
// }

// void loop() {
//   // Если кнопка нажата (LOW), проигрываем звук
//   if (digitalRead(BOOT_PIN) == LOW) {
//     for (size_t i = 0; i < sizeof(sample); i++) {
//       // Прерываем проигрывание, если кнопку отпустили
//       if (digitalRead(BOOT_PIN) == HIGH) break;

//       dac_output_voltage(DAC_CHANNEL_1, sample[i]);
//       delayMicroseconds(SAMPLE_DELAY_US);
//     }
//   }

//   delay(10); // Маленькая задержка для стабильности
// }



// #include <WiFi.h>
// #include <WebServer.h>

// #define SPEAKER_PIN 25

// const char* ssid = "Redmi 9";
// const char* password = "24de4697848d";

// // Массив для мелодии
// int melody[] = {
//   523, 466, 440, 349,
//   392, 392, 294, 466,
//   440, 440, 523, 466, 440,
//   392, 392, 466, 440, 466, 440, 466,
//   392, 392, 466, 440, 466, 440, 466,
//   392, 392, 294, 261, 233, 440, 440, 440,
//   523, 466, 440
// };

// int noteDuration = 180;  // ms

// WebServer server(80);

// void playMelody() {
//   for (int i=0; i<sizeof(melody)/sizeof(int); i++){
//     tone(SPEAKER_PIN, melody[i], noteDuration);
//     delay(noteDuration*1.3);
//     noTone(SPEAKER_PIN);
//   }
//   noTone(SPEAKER_PIN);
//   pinMode(SPEAKER_PIN, OUTPUT);
//   digitalWrite(SPEAKER_PIN, LOW);
// }

// void handleRoot() {
//   String html = "<html><body>";
//   html += "<h1>ESP32 Web Server</h1>";
//   html += "<p>IP-адрес устройства: <b>" + WiFi.localIP().toString() + "</b></p>";
//   html += "<p>MAC-адрес: " + WiFi.macAddress() + "</p>";
//   html += "<p>SSID: " + String(ssid) + "</p>";
//   html += "<p>Уровень сигнала: " + String(WiFi.RSSI()) + " dBm</p>";
//   html += "<button onclick=\"fetch('/ring', {method: 'POST'}).then(() => alert('Звонок отправлен!'));\">Позвонить</button>";
//   html += "</body></html>";
//   server.send(200, "text/html", html);
// }

// void handleRingPost() {
//   server.send(200, "text/plain", "Ringing...");
//   playMelody();
// }

// void handleIP() {
//   server.send(200, "text/plain", WiFi.localIP().toString());
// }

// void printNetworkInfo() {
//   Serial.println("\n--------- Информация о сети ---------");
//   Serial.println("IP-адрес: " + WiFi.localIP().toString());
//   Serial.println("Маска подсети: " + WiFi.subnetMask().toString());
//   Serial.println("Шлюз: " + WiFi.gatewayIP().toString());
//   Serial.println("MAC-адрес: " + WiFi.macAddress());
//   Serial.println("SSID: " + String(ssid));
//   Serial.println("Уровень сигнала: " + String(WiFi.RSSI()) + " dBm");
//   Serial.println("------------------------------------\n");
// }

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Запуск ESP32...");
  
//   noTone(SPEAKER_PIN);
//   pinMode(SPEAKER_PIN, OUTPUT);
//   digitalWrite(SPEAKER_PIN, LOW);

//   Serial.print("Подключение к WiFi");
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
  
//   printNetworkInfo();

//   server.on("/", HTTP_GET, handleRoot);
//   server.on("/ring", HTTP_POST, handleRingPost);
//   server.on("/ip", HTTP_GET, handleIP);
//   server.begin();
  
//   Serial.println("Веб-сервер запущен");
// }

// unsigned long previousMillis = 0;
// const long interval = 60000;  // 1 минута

// void loop() {
//   server.handleClient();
  
//   // Периодически выводим информацию об IP
//   unsigned long currentMillis = millis();
//   if (currentMillis - previousMillis >= interval) {
//     previousMillis = currentMillis;
//     printNetworkInfo();
//   }
// }


#include <WiFi.h>
#include <WebServer.h>

#define SPEAKER_PIN 25

const char* ssid = "your_ssid";
const char* password = "your_password";

int melody[] = {
  523, 466, 440, 349,
  392, 392, 294, 466,
  440, 440, 523, 466, 440,
  392, 392, 466, 440, 466, 440, 466,
  392, 392, 466, 440, 466, 440, 466,
  392, 392, 294, 261, 233, 440, 440, 440,
  523, 466, 440
};

int noteDuration = 180;  // ms

WebServer server(80);

void playMelody() {
  for (int i=0; i<sizeof(melody)/sizeof(int); i++){
    tone(SPEAKER_PIN, melody[i], noteDuration);
    delay(noteDuration * 1.3);
    noTone(SPEAKER_PIN);
  }
  noTone(SPEAKER_PIN);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);
}

void handleRingPost() {
  server.send(200, "text/plain", "Ringing...");
  playMelody();
}

void scanNetworks() {
  Serial.println("Scanning available networks...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No networks found.");
  } else {
    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s (RSSI: %d)\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }
  }
  Serial.println("---------------------------");
}

void setup() {
  Serial.begin(115200);
  noTone(SPEAKER_PIN);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);

  scanNetworks();  // <-- Сканирование и вывод сетей

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected. IP = " + WiFi.localIP().toString());

  server.on("/ring", HTTP_POST, handleRingPost);  // POST /ring
  server.begin();
}

void loop() {
  server.handleClient();
}

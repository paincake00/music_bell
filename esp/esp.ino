#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define SPEAKER_PIN 25

const char* ssid = "LAPTOP-5ORAQVBT";
const char* password = "w3R1644#";

// Определяем несколько мелодий как двумерный массив
// Мелодия 1 - оригинальная
// Мелодия 2 - "В лесу родилась ёлочка" (начало)
// Мелодия 3 - "Имперский марш" (фрагмент)
const int MELODIES_COUNT = 3;

const int MELODY_1_SIZE = 35;
const int melody1[MELODY_1_SIZE] = {
  523, 466, 440, 349,
  392, 392, 294, 466,
  440, 440, 523, 466, 440,
  392, 392, 466, 440, 466, 440, 466,
  392, 392, 466, 440, 466, 440, 466,
  392, 392, 294, 261, 233, 440, 440, 440
};

const int MELODY_2_SIZE = 8;
const int melody2[MELODY_2_SIZE] = {
  262, 294, 330, 262, 262, 294, 330, 262
};

const int MELODY_3_SIZE = 9;
const int melody3[MELODY_3_SIZE] = {
  392, 392, 392, 311, 466, 392, 311, 466, 392
};

// Указатели на мелодии
const int* melodies[MELODIES_COUNT] = {
  melody1, melody2, melody3
};

// Размеры мелодий
const int melodySizes[MELODIES_COUNT] = {
  MELODY_1_SIZE, MELODY_2_SIZE, MELODY_3_SIZE
};

// Различные длительности для разных мелодий
const int noteDurations[MELODIES_COUNT] = {
  180, 200, 250
};

// Индекс текущей мелодии
int currentMelodyIndex = 0;

WebServer server(80);

void playMelody() {
  // Получаем текущую мелодию
  const int* currentMelody = melodies[currentMelodyIndex];
  int size = melodySizes[currentMelodyIndex];
  int duration = noteDurations[currentMelodyIndex];

  // Проигрываем выбранную мелодию
  for (int i = 0; i < size; i++) {
    tone(SPEAKER_PIN, currentMelody[i], duration);
    delay(duration * 1.3);
    noTone(SPEAKER_PIN);
  }

  noTone(SPEAKER_PIN);
  pinMode(SPEAKER_PIN, OUTPUT);
  digitalWrite(SPEAKER_PIN, LOW);
}

void handleNextMelody() {
  // Переключаем на следующую мелодию без воспроизведения
  currentMelodyIndex = (currentMelodyIndex + 1) % MELODIES_COUNT;

  StaticJsonDocument<64> doc;
  doc["status"] = "switched";
  doc["currentMelody"] = currentMelodyIndex + 1;  // номер мелодии начиная с 1
  String jsonString;
  serializeJson(doc, jsonString);
  server.send(200, "application/json", jsonString);
}

void handleRingPost() {
  int size = melodySizes[currentMelodyIndex];
  int duration = noteDurations[currentMelodyIndex];
  int totalDuration = size * duration * 1.3;
  
  StaticJsonDocument<64> doc;
  doc["status"] = "ok";
  doc["duration"] = totalDuration;
  doc["melodyIndex"] = currentMelodyIndex;
  
  String jsonString;
  serializeJson(doc, jsonString);
  server.send(200, "application/json", jsonString);
  
  playMelody(); // Воспроизведение после ответа
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

  server.on("/ring", HTTP_POST, handleRingPost);
  server.on("/nextMelody", HTTP_POST, handleNextMelody);

  server.begin();
}

void loop() {
  server.handleClient();
}

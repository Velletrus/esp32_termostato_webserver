#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "FranzWifi";          // Inserisci il nome della tua rete WiFi
const char* password = "Velletri82.";  // Inserisci la password della tua rete WiFi

void setup() {
  Serial.begin(115200);

  // Connessione WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configura OTA
  ArduinoOTA.begin();

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_SPIFFS
      type = "filesystem";
    }
    // Aggiorna: cancella dati vecchi
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  pinMode(15, OUTPUT);
  pinMode(T2, OUTPUT);
}
unsigned int timed = 5;
void loop() {
  timed++;
  ArduinoOTA.handle();
  digitalWrite(T2, HIGH);
  digitalWrite(15, HIGH);
  delay(timed);
  digitalWrite(T2, LOW);
  digitalWrite(15, LOW);
  delay(timed);
    // Controlla costantemente se c'è un nuovo aggiornamento
  if (timed > 1000)
      timed= 5; 
}


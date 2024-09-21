//// 08/09/2024

#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <LCD_I2C.h>
#include <WiFi.h>

#include <WebServer.h>
#include <DHT22.h>

//define pin data
#define pinDATA 2 // SDA, or almost any other I/O pin
#define caldaia 16
#define EEPROM_SIZE 20

DHT22 dht22(pinDATA); 
LCD_I2C lcd(0x27, 16, 2);
unsigned long prevMillis, actMillis;
bool outRisc;
bool manual;
float setpoint = 25.0;
float hyst = 1.0;
float sym = 0.0;
bool updn = false;
bool flag1 , flag2;
bool enRisc = false;
int limit = 0;
IPAddress dns(8,8,8,8);
float t,h;
bool en = true; //// da mettere a fale
#include "funz.h"
IPAddress ip(192,168,1,82);
IPAddress sub(255,255,255,0);
IPAddress gw(192,168,1,254);

const char* ssid =  "FranzWifi";
const char* pw =    "Velletri82.";

WebServer server(80);
uint8_t gradi[8] =
{
    0b01000,
    0b10100,
    0b01000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
};
void setup() {
  //// setup wifi
  
  EEPROM.begin(EEPROM_SIZE);
  lcd.begin(); 
  lcd.backlight();
  lcd.createChar(0, gradi);
  Serial.begin(9600); //1bit=10µs
  WiFi.config(ip, dns, gw, sub);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);
  Serial.print("Con WIFI");
  lcd.setCursor(0, 0);
  lcd.print("Wifi");
  if (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    lcd.print(".");
    delay(200);
  }
  lcd.clear();
  Serial.println("");
  Serial.print("IP = "); Serial.println(WiFi.localIP());
  lcd.setCursor(0,0); 
  lcd.print(WiFi.localIP());
  pinMode(23, OUTPUT);
  pinMode(caldaia, OUTPUT); /// rele caldaia
  delay(1000);
  lcd.clear();
  server.on("/", HTTP_GET,  getdata);
  server.on("/accendi",accendi);
  server.on("/spegni", spegni);
  server.on("/setpoint", HTTP_GET, impsetpoint);
  server.begin();
  setpoint = EEPROM.readFloat(1);
  hyst = EEPROM.readFloat(5);
  lcd.clear();
  Serial.print(setpoint); Serial.print(" "); Serial.println(hyst);
  lcd.print(setpoint); lcd.print(" "); lcd.println(hyst);
  delay(1000);
}

void loop() {
  //Serial.println(dht22.debug()); //optionnal
  actMillis = millis();
  server.handleClient();
  delay(2);
  if (actMillis -prevMillis >= 1998){
        t = dht22.getTemperature();
        h = dht22.getHumidity();
        t = t + sym;
        if (dht22.getLastError() != dht22.OK) {
          Serial.print("last error :");
          Serial.println(dht22.getLastError());
          en = false;
        }
        outRisc = riscaldamento(setpoint, t, hyst, en);
        
      //  Serial.println(WiFi.status());
        prevMillis = actMillis;
         if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connessione Wi-Fi persa. Riconnessione...");
       connectToWiFi();
      }
//// simulazione
    lcdWrite();
  }
}
void getdata(){
      /// copongo la stringa
      server.send(200, "application/json", generaJson());
      
  ;}

void accendi()
  {
     server.send(200, "application/json", generaJson());
      en = true;
   }

void spegni(){ 
      
      server.send(200, "application/json", generaJson());
      en = false;}

void impsetpoint(){ 
      bool savemem = false;
      if(server.hasArg("setpoint")){
        String _arg = server.arg("setpoint");
        Serial.println(_arg);
        setpoint = _arg.toFloat();
        EEPROM.writeFloat(1, setpoint);
        //EEPROM.commit();
        savemem = true;
// Read a Float Variable From EEPROM @ Specific Address

      };
      if(server.hasArg("hyst")){
        String _arg = server.arg("hyst");
        Serial.println(_arg);
        hyst = _arg.toFloat();
        EEPROM.writeFloat(5, hyst);
      //  EEPROM.commit();
        savemem = true;
      };
     
      if (savemem)
      {
         EEPROM.commit();
         Serial.println("salvato in EEprom");
      }
      server.send(200, "application/json", generaJson());
      
      }
String generaJson(){
  JsonDocument doc;
  String _jsonObj;
      doc["temp"]= t;
      doc["hum"] = h;
      doc["status"] = en;
      doc["caldaia"] = outRisc;
      doc["setpoint"] = setpoint;
      doc["secOn"] = limit;
      serializeJson(doc, _jsonObj);
      Serial.println(_jsonObj);
      
      return _jsonObj;
}

void lcdWrite(){
       
        digitalWrite(23, !digitalRead(23));
       // delay(1000);
        lcd.setCursor(0, 0); //Collecting period should be : >1.7 second
        lcd.print("t= "); lcd.print(t,1); lcd.write(0); lcd.print("h= "); lcd.print(h, 1); lcd.print("%");
        lcd.setCursor(0, 1);
        lcd.print("Risc = "); 
        if (outRisc)
        {
          lcd.print("ACCESO"); 
        }
        else
        {
          lcd.print("SPENTO"); 
        } 

}
void connectToWiFi() {
  Serial.print("Connessione alla rete Wi-Fi...");
  
  // Avvio della connessione
  WiFi.begin(ssid, pw);
  
  // Attendi finché non viene stabilita la connessione
  unsigned long startTime = millis();
  unsigned long timeout = 10000;  // Timeout di 10 secondi
  
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime >= timeout) {
      Serial.println("Timeout di connessione!");
      return;  // Esci se il timeout è stato superato
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connesso!");
}
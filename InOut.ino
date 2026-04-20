// InOut.ino — Arduino GPIO manager with OTA, mDNS, WiFi, captive portal, and web UI
// All HTML served from html.h

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <esp_task_wdt.h>
#include <Secrets.h>   // defines MYSSID, MYPSK

// ─── Debug ────────────────────────────────────────────────────────────────────
#define DEBUG 1
#if DEBUG
  #define DBG(x)   Serial.print(x)
  #define DBGLN(x) Serial.println(x)
#else
  #define DBG(x)
  #define DBGLN(x)
#endif

// ─── Config ───────────────────────────────────────────────────────────────────
#define HOSTNAME  "InOut3"
#define TX_POWER  11       // dBm

// ESP32-C3 safe user GPIOs (0-10 = general I/O, 18/19 = USB, 20/21 = UART0)
// GPIO 11-17 are reserved for internal flash/SPI — do not use.
const uint8_t PINS[] = {0,1,2,3,4,5,6,7,8,9,10,20,21};
const uint8_t PIN_COUNT = sizeof(PINS) / sizeof(PINS[0]);

// ─── Globals ──────────────────────────────────────────────────────────────────
WebServer   server(80);
DNSServer   dns;
Preferences prefs;

bool captiveMode = false;
char savedSSID[64];
char savedPSK[64];

// Pin state: direction (0=IN,1=OUT), value (0/1), monitored
struct PinState { uint8_t dir; uint8_t val; bool monitor; };
PinState pinState[PIN_COUNT];   // FIX: was sizeof(PINS) — now uses PIN_COUNT

// ─── Forward declarations ─────────────────────────────────────────────────────
void startWifi();
void startAP();
void setupOTA();
void setupMDNS();
void setupRoutes();
String pinJSON();

#include "html.h"   // HTML strings: PAGE_MAIN, PAGE_PORTAL

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  DBGLN("[InOut] boot");

  // Load saved credentials
  prefs.begin("inout", false);
  strlcpy(savedSSID, prefs.getString("ssid", MYSSIDIOT).c_str(), 64);
  strlcpy(savedPSK,  prefs.getString("psk",  MYPSKIOT).c_str(), 64);

  // Default all pins to INPUT
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    pinState[i] = {0, 0, false};
    pinMode(PINS[i], INPUT);
  }

  startWifi();
  setupOTA();
  setupMDNS();
  setupRoutes();
  server.begin();
  DBGLN("[InOut] ready");
}

// ─── Loop ─────────────────────────────────────────────────────────────────────
void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  if (captiveMode) dns.processNextRequest();

  // Re-read INPUT pin values
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    if (pinState[i].dir == 0) {
      pinState[i].val = digitalRead(PINS[i]);
    }
  }

  // Attempt reconnect if WiFi lost (station mode only)
  static uint32_t lastCheck = 0;
  if (!captiveMode && WiFi.status() != WL_CONNECTED && millis() - lastCheck > 15000) {
    lastCheck = millis();
    DBGLN("[WiFi] reconnecting...");
    WiFi.reconnect();
  }
}

// ─── WiFi ─────────────────────────────────────────────────────────────────────
void startWifi() {
  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_15dBm);
  WiFi.begin(savedSSID, savedPSK);

  DBG("[WiFi] connecting to "); DBGLN(savedSSID);
  uint32_t t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    esp_task_wdt_reset();   // FIX: feed WDT during blocking wait
    delay(250);
  }

  if (WiFi.status() == WL_CONNECTED) {
    DBG("[WiFi] IP: "); DBGLN(WiFi.localIP());
  } else {
    DBGLN("[WiFi] failed - starting captive portal");
    startAP();
  }
}

void startAP() {
  captiveMode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP(HOSTNAME);
  dns.start(53, "*", WiFi.softAPIP());
  DBG("[AP] IP: "); DBGLN(WiFi.softAPIP());
}

// ─── OTA ──────────────────────────────────────────────────────────────────────
void setupOTA() {
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.onStart([]()  { DBGLN("[OTA] start");   });
  ArduinoOTA.onEnd([]()    { DBGLN("[OTA] done");    });
  ArduinoOTA.onError([](ota_error_t e) { DBG("[OTA] err "); DBGLN(e); });
  ArduinoOTA.begin();
}

// ─── mDNS ─────────────────────────────────────────────────────────────────────
void setupMDNS() {
  if (MDNS.begin(HOSTNAME)) { DBGLN("[mDNS] InOut.local"); }
}

// ─── HTTP Routes ──────────────────────────────────────────────────────────────
void setupRoutes() {
  // Serve main UI or captive portal
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", captiveMode ? PAGE_PORTAL : PAGE_MAIN);
  });

  // Captive portal redirect (iOS/Android probes)
  server.onNotFound([]() {
    if (captiveMode) {
      server.sendHeader("Location", "http://192.168.4.1/");
      server.send(302);
    } else {
      server.send(404, "text/plain", "Not found");
    }
  });

  // Save WiFi credentials from captive portal
  server.on("/save", HTTP_POST, []() {
    String ssid = server.arg("ssid");
    String psk  = server.arg("psk");
    if (ssid.length()) {
      prefs.putString("ssid", ssid);
      prefs.putString("psk",  psk);
      server.send(200, "text/html", "<h2>Saved! Rebooting...</h2>");
      delay(1500);
      ESP.restart();
    } else {
      server.send(400, "text/plain", "ssid required");
    }
  });

  // GET /pins  -> JSON array of pin states
  server.on("/pins", HTTP_GET, []() {
    server.send(200, "application/json", pinJSON());
  });

  // GET /wifi -> RSSI and SSID
  server.on("/wifi", HTTP_GET, []() {
    int rssi = WiFi.RSSI();
    String j = "{\"rssi\":" + String(rssi) + ",\"ssid\":\"" + String(savedSSID) + "\"}";
    server.send(200, "application/json", j);
  });

  // POST /pin?id=N&dir=0|1&val=0|1&mon=0|1
  server.on("/pin", HTTP_POST, []() {
    int id = server.arg("id").toInt();
    if (id < 0 || id >= PIN_COUNT) { server.send(400); return; }

    if (server.hasArg("dir")) {
      pinState[id].dir = server.arg("dir").toInt();
      if (pinState[id].dir == 1) {
        pinMode(PINS[id], OUTPUT);
      } else {
        pinMode(PINS[id], INPUT);
      }
    }
    if (server.hasArg("val") && pinState[id].dir == 1) {
      pinState[id].val = server.arg("val").toInt();
      digitalWrite(PINS[id], pinState[id].val);
    }
    if (server.hasArg("mon")) {
      pinState[id].monitor = server.arg("mon").toInt();
    }
    server.send(200, "application/json", pinJSON());
  });
}

// ─── JSON helper ──────────────────────────────────────────────────────────────
String pinJSON() {
  String j = "[";
  for (uint8_t i = 0; i < PIN_COUNT; i++) {
    if (i) j += ",";
    j += "{\"id\":" + String(i)
      +  ",\"pin\":"  + String(PINS[i])
      +  ",\"dir\":"  + String(pinState[i].dir)
      +  ",\"val\":"  + String(pinState[i].val)
      +  ",\"mon\":"  + String(pinState[i].monitor ? 1 : 0)
      +  "}";
  }
  j += "]";
  return j;
}

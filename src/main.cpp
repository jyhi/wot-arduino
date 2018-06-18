#include <Arduino.h>
#include "webthing.h"
#include "utils.h"
#include "http-headers.h"

// Network
#ifdef ARDUINO_AVR_UNO
#include <Ethernet.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif

// Server instance
#ifdef ARDUINO_AVR_UNO
EthernetServer server(SERVER_HTTP_PORT);
#endif
#if defined(ESP8266) || defined(ESP32)
WiFiServer server(SERVER_HTTP_PORT);
#endif

// Buffer sizes
#define BUFSIZE_PAYLOAD  256
#define BUFSIZE_RESPONSE 384

// Connection timeout (in second), used when waiting for actual contents
// after connection is established
#define CONNECTION_TIMEOUT 4

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif

  // Internal LED is used to indicate setup status
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

#ifdef ARDUINO_AVR_UNO
#ifdef DEBUG
  Serial.println(F("** Connecting Ethernet"));
#endif

#ifdef ARDUINO_ETHERNET_NO_DHCP
  Ethernet.begin((uint8_t *)mac, (uint8_t *)ip);
#else
  Ethernet.begin((uint8_t *)mac);
#endif

  server.begin();

#ifdef DEBUG
#ifdef ARDUINO_ETHERNET_NO_DHCP
  Serial.print(F("** IP: "));
  Serial.println(ip);
#else
  Serial.print(F("** DHCP: "));
  Serial.println(Ethernet.localIP());
#endif
#endif
#endif // #ifdef ARDUINO_AVR_UNO

#if defined(ESP8266) || defined(ESP32)
#ifdef DEBUG
  Serial.println(F("** Connecting WiFi: " WIFI_SSID));
#endif

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
  // LED should be low on success
  digitalWrite(LED_BUILTIN, LOW);

  server.begin();
  server.setNoDelay(true);

#ifdef DEBUG
  Serial.print(F("** DHCP: "));
  Serial.println(WiFi.localIP());
#endif
#endif // #if defined(ESP8266) || defined(ESP32)
}

void loop()
{
  // Check connection availability
#ifdef ARDUINO_AVR_UNO
  network_maintain();
  EthernetClient client = server.available();
#endif
#if defined(ESP8266) || defined(ESP32)
  WiFiClient client = server.available();
#endif

  if (!client)
    return;

  static char response[BUFSIZE_RESPONSE] = {0};
  static char payload[BUFSIZE_PAYLOAD]   = {0};

  // Wait for actual bytes coming
  // Especially on ESP8266 / ESP32 contents are slow to reach
  {
    time_t start_time = time(NULL);
    while (!client.available())
      if (time(NULL) - start_time > CONNECTION_TIMEOUT)
        goto end;
  }

#ifdef DEBUG
  Serial.print  (F("** New connection, incoming "));
  Serial.print  (client.available());
  Serial.println(F(" bytes"));
#endif

  // If a request is too large then it might be truncated
  // XXX: It is actually okay to proceed if we only need the first HTTP line
  if (client.available() > BUFSIZE_PAYLOAD) {
    Serial.println(F("** Content too large, 413"));
    client.print(http_413);
    client.print(http_header_without_content);
    goto end;
  }

  for (size_t i = 0; client.connected() && client.available() && i < BUFSIZE_PAYLOAD; i++) {
    payload[i] = client.read();
  }

#ifdef DEBUG
  Serial.println(F("** ===== Payload:"));
  Serial.println(payload);
  Serial.println(F("** ====="));
#endif

  webthing_handle_request(response, payload);

#ifdef DEBUG
  Serial.println(F("** ===== Response:"));
  Serial.println(response);
  Serial.println(F("** ====="));
#endif

  client.println(response);

end:
#ifdef DEBUG
  Serial.println(F("** Closing connection"));
#endif

  client.stop();

  // Clear buffer to prevent problems
  memset(response, 0, BUFSIZE_RESPONSE);
  memset(payload,  0, BUFSIZE_PAYLOAD);
}

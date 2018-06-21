#include <Arduino.h>
#include <time.h>
#include "webthing.h"
#include "utils.h"
#include "http-headers.h"

// Use Ethernet shield on Arduino UNO by default
#if defined(ARDUINO_AVR_UNO) && !defined(ARDUINO_AVR_UNO_USE_WIFI)
#define ARDUINO_AVR_UNO_USE_ETHERNET
#endif

// Network
#if defined(ARDUINO_AVR_UNO)
#if defined(ARDUINO_AVR_UNO_USE_ETHERNET)
#include <Ethernet.h>
#elif defined(ARDUINO_AVR_UNO_USE_WIFI)
#include <WiFi.h>
#endif
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

// Server instance
#if defined(ARDUINO_AVR_UNO)
#if defined(ARDUINO_AVR_UNO_USE_ETHERNET)
EthernetServer server(SERVER_HTTP_PORT);
#elif defined(ARDUINO_AVR_UNO_USE_WIFI)
WiFiServer server(SERVER_HTTP_PORT);
#endif
#elif defined(ESP8266) || defined(ESP32)
WiFiServer server(SERVER_HTTP_PORT);
#endif

// Buffer sizes
#define BUFSIZE_PAYLOAD  256
#define BUFSIZE_RESPONSE 384

// Connection timeout (in second), used when waiting for actual contents
// after connection is established
#define CONNECTION_TIMEOUT 4

#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif

void setup()
{
#if defined(DEBUG)
  Serial.begin(115200);
#endif

  // Internal LED is used to indicate setup status
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

#if defined(DEBUG)
#if defined(ARDUINO_AVR_UNO_USE_ETHERNET)
  Serial.println(F("** Connecting Ethernet"));
#elif defined(ARDUINO_AVR_UNO_USE_WIFI) || defined(ESP8266) || defined(ESP32)
  Serial.println(F("** Connecting WiFi"));
#endif
#endif

#if defined(ARDUINO_AVR_UNO)
#if defined(ARDUINO_AVR_UNO_USE_ETHERNET)
#if defined(ARDUINO_ETHERNET_NO_DHCP)
  Ethernet.begin((uint8_t *)mac, (uint8_t *)ip);
#else
  Ethernet.begin((uint8_t *)mac);
#endif
#endif

  server.begin();

#endif

#if (defined (ARDUINO_AVR_UNO) && defined(ARDUINO_AVR_UNO_USE_WIFI)) || \
     defined(ESP8266) || defined(ESP32)

  WiFi.mode(WIFI_STA); // Otherwise AP+STA is used, exposing an unnecessary AP
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
  }
  // LED should be low on success
#if defined(ESP8266)
  digitalWrite(LED_BUILTIN, HIGH); // LED_BUILTIN on ESP8266 is low active
#else
  digitalWrite(LED_BUILTIN, LOW);
#endif

  server.begin();

#if defined(DEBUG)
#if defined(ARDUINO_AVR_UNO) && defined(ARDUINO_AVR_UNO_USE_ETHERNET)
#if defined(ARDUINO_ETHERNET_NO_DHCP)
  Serial.print  (F("** IP: "));
  Serial.print  (ip[0]);
  Serial.print  (F("."));
  Serial.print  (ip[1]);
  Serial.print  (F("."));
  Serial.print  (ip[2]);
  Serial.print  (F("."));
  Serial.println(ip[3]);
#else
  Serial.print(F("** DHCP: "));
  Serial.println(Ethernet.localIP());
#endif
#else
  Serial.print(F("** DHCP: "));
  Serial.println(WiFi.localIP());
#endif
#endif
#endif
}

void loop()
{
  // Check connection availability
#if defined(ARDUINO_AVR_UNO)
#if defined(ARDUINO_AVR_UNO_USE_ETHERNET)
  network_maintain();
  EthernetClient client = server.available();
#elif defined(ARDUINO_AVR_UNO_USE_WIFI)
  WiFiClient client = server.available();
#endif
#elif defined(ESP8266) || defined(ESP32)
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

#if defined(DEBUG)
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

#if defined(DEBUG)
  Serial.println(F("** ===== Payload:"));
  Serial.println(payload);
  Serial.println(F("** ====="));
#endif

  webthing_handle_request(response, payload);

#if defined(DEBUG)
  Serial.println(F("** ===== Response:"));
  Serial.println(response);
  Serial.println(F("** ====="));
#endif

  client.println(response);

end:
#if defined(DEBUG)
  Serial.println(F("** Closing connection"));
#endif

  client.stop();

  // Clear buffer to prevent problems
  memset(response, 0, BUFSIZE_RESPONSE);
  memset(payload,  0, BUFSIZE_PAYLOAD);
}

#ifndef WEBTHING_DEFINES_H
#define WEBTHING_DEFINES_H

#include <stdint.h>

#ifndef WEBTHING_NAME
#define WEBTHING_NAME "led"
#endif

#ifndef WEBTHING_TYPE
#define WEBTHING_TYPE "onOffLight"
#endif

#ifndef WEBTHING_DESC
#define WEBTHING_DESC "A web connected LED"
#endif

#ifndef SERVER_HTTP_PORT
#define SERVER_HTTP_PORT 80
#endif

#ifndef SERVER_HTTPS_PORT
#define SERVER_HTTPS_PORT 443
#endif

#ifdef ARDUINO_AVR_UNO
static const uint8_t mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
#ifdef ARDUINO_ETHERNET_NO_DHCP
static const uint8_t ip[] = {192, 168, 1, 141};
#endif
#endif

#if defined(ESP8266) || defined(ESP32)
#ifndef WIFI_SSID
#define WIFI_SSID "Project Xatonif"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "finotaX0"
#endif
#endif

#ifdef USE_MDNS
#define WEBTHING_MDNS_SERVICE_NAME WEBTHING_DESC "._http"
#endif

#endif

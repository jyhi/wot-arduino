#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include <Arduino.h>

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h> // PROGMEM
#endif

#ifdef HTTP_HEADERS_WITH_DESCRIPTION

static const char http_100[] PROGMEM =
  "HTTP/1.1 100 Continue\r\n";

static const char http_200[] PROGMEM =
  "HTTP/1.1 200 OK\r\n";

static const char http_201[] PROGMEM =
  "HTTP/1.1 201 Created\r\n";

static const char http_204[] PROGMEM =
  "HTTP/1.1 204 No Content\r\n";

static const char http_400[] PROGMEM =
  "HTTP/1.1 400 Bad Request\r\n";

static const char http_404[] PROGMEM =
  "HTTP/1.1 404 Not Found\r\n";

static const char http_405[] PROGMEM =
  "HTTP/1.1 405 Method Not Allowed\r\n";

static const char http_413[] PROGMEM =
  "HTTP/1.1 413 Payload Too Large\r\n";

static const char http_415[] PROGMEM =
  "HTTP/1.1 415 Unsupported Media Type\r\n";

static const char http_500[] PROGMEM =
  "HTTP/1.1 500 Internal Server Error\r\n";

#else

static const char http_100[] PROGMEM =
  "HTTP/1.1 100\r\n";

static const char http_200[] PROGMEM =
  "HTTP/1.1 200\r\n";

static const char http_201[] PROGMEM =
  "HTTP/1.1 201\r\n";

static const char http_204[] PROGMEM =
  "HTTP/1.1 204\r\n";

static const char http_400[] PROGMEM =
  "HTTP/1.1 400\r\n";

static const char http_404[] PROGMEM =
  "HTTP/1.1 404\r\n";

static const char http_405[] PROGMEM =
  "HTTP/1.1 405\r\n";

static const char http_413[] PROGMEM =
  "HTTP/1.1 413\r\n";

static const char http_415[] PROGMEM =
  "HTTP/1.1 415\r\n";

static const char http_500[] PROGMEM =
  "HTTP/1.1 500\r\n";

#endif // #ifdef HTTP_HEADERS_WITH_DESCRIPTION

static const char http_header_with_content_json[] PROGMEM =
  "Content-Type: application/json; charset=UTF-8\r\n"
  "Connection: close\r\n"
  "\r\n";

static const char http_header_without_content[] PROGMEM =
  "Connection: close\r\n"
  "\r\n";

#endif // #ifndef HTTP_HEADERS_H

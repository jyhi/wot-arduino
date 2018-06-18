#include <Arduino.h>
#include <ArduinoJson.h>
#include "http-headers.h"
#include "webthing-properties.h"

#define LED_PIN LED_BUILTIN

#define BUFSIZE_JSON 64

static void webthing_handle_thing_properties_on(char *response, bool is_ctype_json, const char *method, const char *message)
{
  StaticJsonBuffer<BUFSIZE_JSON> json_buffer;

  if (strcmp_P(method, PSTR("GET")) == 0) {

    JsonObject & j_on = json_buffer.parseObject(WEBTHING_RESOURCE_PROPERTY_ON, 1);
    if (!j_on.success()) {
      strcpy_P(response, http_500);
      strcpy_P(response += strlen_P(http_500), http_header_without_content);
      return;
    }

    // Read current properties in
    j_on["on"] = (bool)digitalRead(LED_PIN);

    // Respond
    strcpy_P(response, http_200);
    strcpy_P(response += strlen_P(http_200), http_header_with_content_json);
    j_on.printTo(response + strlen_P(http_header_with_content_json), 32);

  } else if (strcmp_P(method, PSTR("PUT")) == 0) {

    JsonObject & j_on = json_buffer.parseObject(message, 1);
    if (!j_on.success()) {
      strcpy_P(response, http_400);
      strcpy_P(response += strlen_P(http_400), http_header_without_content);
      return;
    }

    // Require content type be JSON
    if (!is_ctype_json) {
      strcpy_P(response, http_415);
      strcpy_P(response += strlen_P(http_415), http_header_without_content);
      return;
    }

    // Check if the JSON message actually contains the correct key
    if (!j_on.containsKey(F("on"))) {
      strcpy_P(response, http_400);
      strcpy_P(response += strlen_P(http_400), http_header_without_content);
      return;
    }

    // Action
    digitalWrite(LED_PIN, j_on["on"] ? HIGH : LOW);

    // Read it back
    // NOTE: While Mozilla proposes a 200 OK return, W3C (who first developed
    // Web Thing specifiication) proposes a 204 No Content return. To comply
    // with W3C specification, this read is unnecessary and do not return a
    // JSON.
    j_on["on"] = (bool)digitalRead(LED_PIN);

    // Response with status now
    strcpy_P(response, http_200);
    strcpy_P(response += strlen_P(http_200), http_header_with_content_json);
    j_on.printTo(response + strlen_P(http_header_with_content_json), BUFSIZE_JSON);

  } else {

    // Unsupported method
    strcpy_P(response, http_405);
    strcpy_P(response += strlen_P(http_405), http_header_without_content);

  }
}

static void webthing_handle_thing_properties_all(char *response, const char *method)
{
  if (strcmp_P(method, PSTR("GET")) == 0) {

    strcat_P(response, http_200);
    strcat_P(response, http_header_with_content_json);
    strcat_P(response, PSTR("["));
    strcat_P(response, WEBTHING_RESOURCE_PROPERTY_ON);
    strcat_P(response, PSTR("]"));

  } else {

    // Unsupported method
    strcpy_P(response, http_405);
    strcpy_P(response += strlen_P(http_405), http_header_without_content);

  }
}

extern "C" {

void webthing_handle_thing_properties(char *response, bool is_ctype_json, const char *method, const char *path, const char *message)
{
  if (!response || !method || !path || !message)
    return;

  // URL dispatcher
  if (strcasecmp_P(path, PSTR("")) == 0 || strcasecmp_P(path, PSTR("/")) == 0) {
    webthing_handle_thing_properties_all(response, method);
  } else if (strcasecmp_P(path, PSTR("/on")) == 0) {
    webthing_handle_thing_properties_on(response, is_ctype_json, method, message);
  } else {
    strcpy_P(response, http_404);
    strcpy_P(response + strlen_P(http_404), http_header_without_content);
  }
}

} // extern "C"

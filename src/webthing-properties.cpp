#include <Arduino.h>
#include <ArduinoJson.h>
#include "http-headers.h"
#include "webthing-properties.h"

#define LED_PIN LED_BUILTIN
#define BUFSIZE_JSON 64

static void webthing_handle_thing_properties_on(char *response, bool is_ctype_json, const char *method, const char *message)
{
  StaticJsonBuffer<BUFSIZE_JSON> json_buffer;
  String json;

  if (strcmp_P(method, PSTR("GET")) == 0) {

    JsonObject & j_on = json_buffer.parseObject(WEBTHING_RESOURCE_PROPERTY_ON, 1);
    if (!j_on.success()) {
      strcat_P(response, http_500);
      strcat_P(response, http_header_without_content);
      return;
    }

    // Read current properties in
    j_on["on"] = (bool)digitalRead(LED_PIN);
    j_on.printTo(json);

    // Respond
    strcat_P(response, http_200);
    strcat_P(response, http_header_with_content_json);
    strcat  (response, json.c_str());

  } else if (strcmp_P(method, PSTR("PUT")) == 0) {

    JsonObject & j_on = json_buffer.parseObject(message, 1);
    if (!j_on.success()) {
      strcat_P(response, http_400);
      strcat_P(response, http_header_without_content);
      return;
    }

    // Require content type be JSON
    if (!is_ctype_json) {
      strcat_P(response, http_415);
      strcat_P(response, http_header_without_content);
      return;
    }

    // Check if the JSON message actually contains the correct key
    if (!j_on.containsKey(F("on"))) {
      strcat_P(response, http_400);
      strcat_P(response, http_header_without_content);
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
    j_on.printTo(json);

    // Response with status now
    strcat_P(response, http_200);
    strcat_P(response, http_header_with_content_json);
    strcat  (response, json.c_str());

  } else {

    // Unsupported method
    strcat_P(response, http_405);
    strcat_P(response, http_header_without_content);

  }
}

static void webthing_handle_thing_properties_all(char *response, const char *method)
{
  StaticJsonBuffer<BUFSIZE_JSON> json_buffer;
  String json;

  if (strcmp_P(method, PSTR("GET")) == 0) {

    JsonObject & j_on = json_buffer.parseObject(WEBTHING_RESOURCE_PROPERTY_ON, 1);
    if (!j_on.success()) {
      strcat_P(response, http_500);
      strcat_P(response, http_header_without_content);
      return;
    }

    // Read current value in
    j_on["on"] = (bool)digitalRead(LED_PIN);
    j_on.printTo(json);

    strcat_P(response, http_200);
    strcat_P(response, http_header_with_content_json);
    strcat_P(response, PSTR("["));
    strcat(response, json.c_str());
    strcat_P(response, PSTR("]"));

  } else {

    // Unsupported method
    strcat_P(response, http_405);
    strcat_P(response, http_header_without_content);

  }
}

extern "C" void webthing_handle_thing_properties(char *response, bool is_ctype_json, const char *method, const char *path, const char *message)
{
  if (!response || !method || !path || !message)
    return;

  // URL dispatcher
  if (strcasecmp_P(path, PSTR("")) == 0 || strcasecmp_P(path, PSTR("/")) == 0) {
    webthing_handle_thing_properties_all(response, method);
  } else if (strcasecmp_P(path, PSTR("/on")) == 0 ||
             strcasecmp_P(path, PSTR("/on/")) == 0) {
    webthing_handle_thing_properties_on(response, is_ctype_json, method, message);
  } else {
    strcat_P(response, http_404);
    strcat_P(response, http_header_without_content);
  }
}

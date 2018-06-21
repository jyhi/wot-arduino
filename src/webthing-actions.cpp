#include <Arduino.h>
#include <ArduinoJson.h>
#include "http-headers.h"

#define BUFSIZE_JSON 64

void webthing_handle_thing_actions_reboot(char *response, bool is_ctype_json, const char *method) {
  wdt_enable(WDTO_1S);
  for (;;) {}
}

void webthing_handle_thing_actions_all(char *response, bool is_ctype_json, const char *method, const char *message) {
  StaticJsonBuffer<BUFSIZE_JSON> json_buffer;

  if (strcasecmp_P(method, PSTR("GET")) == 0) {

    // Action list
    // Unimplemented
    strcpy_P(response, http_204);
    strcpy_P(response, http_header_without_content);

  } else if (strcasecmp_P(method, PSTR("POST")) == 0) {

    // Action request
    if (!is_ctype_json) {
      strcpy_P(response, http_415);
      strcpy_P(response, http_header_without_content);
      return;
    }

    JsonObject & j_action = json_buffer.parse(message, 1);
    if (!j_action.success()) {
      strcpy_P(response, http_400);
      strcpy_P(response, http_header_without_content);
      return;
    }

    if (j_action.containsKey("reboot")) {
      webthing_handle_thing_actions_reboot(response, );
    } else {
      strcpy_P(response, http_400);
      strcpy_P(response, http_header_without_content);
      return;
    }

  } else {

    // Unsupported method
    strcat_P(response, http_405);
    strcat_P(response, http_header_without_content);

  }
}

extern "C" void webthing_handle_thing_actions(char *response, bool is_ctype_json, const char *method, const char *path, const char *message)
{
  if (!response || !method || !path)
    return;

  // URL dispatcher
  if (strcasecmp_P(path, PSTR("")) == 0 || strcasecmp_P(path, PSTR("/")) == 0) {
    webthing_handle_thing_actions_all(response, is_ctype_json, method, message);
  } else if (strcasecmp_P(path, PSTR("/reboot")) == 0 ||
             strcasecmp_P(path, PSTR("/reboot/")) == 0) {
    webthing_handle_thing_actions_reboot(response, is_ctype_json, method);
  } else {
    //
  }
}

#include <Arduino.h>

void webthing_handle_thing_actions_reboot(char *response, bool is_ctype_json, const char *method) {
  wdt_enable(WDTO_1S);
  for (;;) {}
}

void webthing_handle_thing_actions_all(char *response, bool is_ctype_json, const char *method) {
  if (strcasecmp_P(method, PSTR("POST")) == 0) {
    //
  } else {
    //
  }
}

extern "C" void webthing_handle_thing_actions(char *response, bool is_ctype_json, const char *method, const char *path)
{
  if (!response || !method || !path)
    return;

  // URL dispatcher
  if (strcasecmp_P(path, PSTR("")) == 0 || strcasecmp_P(path, PSTR("/")) == 0) {
    webthing_handle_thing_actions_all(response, is_ctype_json, method);
  } else if (strcasecmp_P(path, PSTR("/reboot")) == 0 ||
             strcasecmp_P(path, PSTR("/reboot/")) == 0) {
    webthing_handle_thing_actions_reboot(response, is_ctype_json, method);
  } else {
    //
  }
}

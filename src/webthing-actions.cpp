#include <Arduino.h>

void webthing_handle_thing_actions_reboot(char *response, bool is_ctype_json, const char *method) {
  wdt_enable(WDTO_1S);
  for (;;) {}
}

extern "C" void webthing_handle_thing_actions(char *response, bool is_ctype_json, const char *method, const char *path)
{
  if (!response || !method || !path)
    return;

  // TODO
}

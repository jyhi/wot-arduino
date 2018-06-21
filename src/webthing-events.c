#include <Arduino.h>
#include "http-headers.h"

void webthing_handle_thing_events(char *response, const char *method, const char *path)
{
  // Unimplemented
  strcat_P(response, http_404);
  strcat_P(response, http_header_without_content);
}

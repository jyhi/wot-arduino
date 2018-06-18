#include <Arduino.h>
#include "http-headers.h"
#include "webthing-description.h"

void webthing_handle_thing_description(char *response, const char *method)
{
  // Unexpected NULL pointer, which is a bug
  if (!response || !method)
    return;

  if (strcmp_P(method, PSTR("GET")) == 0) {

    // Respond with the Web Thing description JSON
    strcpy_P(response, http_200);
    strcpy_P(response += strlen_P(http_200), http_header_with_content_json);
    strcpy_P(response += strlen_P(http_header_with_content_json), webthing_description);

  } else {

    // Unsupported method
    strcpy_P(response, http_405);
    strcpy_P(response += strlen_P(http_405), http_header_without_content);

  }
}

void webthing_handle_things_description(char *response, const char *method)
{
  // Unexpected NULL pointer, which is a bug
  if (!response || !method)
    return;

  if (strcmp_P(method, PSTR("GET")) == 0) {

    // Respond with the Web Thing description JSON, but in an array
    strcpy_P(response, http_200);
    strcpy_P(response += strlen_P(http_200), http_header_with_content_json);
    strcpy_P(response += strlen_P(http_header_with_content_json), PSTR("["));
    strcpy_P(response += 1, webthing_description);
    strcpy_P(response += strlen_P(webthing_description), PSTR("]"));

  } else {

    // Unsupported method
    strcpy_P(response, http_405);
    strcpy_P(response += strlen_P(http_405), http_header_without_content);

  }
}

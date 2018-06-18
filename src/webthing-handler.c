#include <Arduino.h>
#include <stdio.h>
#include "http-headers.h"
#include "webthing-defines.h"
#include "webthing-description.h"
#include "webthing-properties.h"
#include "webthing-actions.h"
#include "webthing-events.h"

#define BUFSIZE_METHOD 9
#define BUFSIZE_PATH   65

#define BUFSIZE_METHOD_SCANF "8"
#define BUFSIZE_PATH_SCANF   "64"

void webthing_handle_request(char *response, const char *payload)
{
  if (!response)
    return;

  static char http_method[BUFSIZE_METHOD] = {0};
  static char http_path[BUFSIZE_PATH]     = {0};
  bool is_ctype_json = false;

  // xtensa sdks does not have sscanf_P impl
#if defined(ESP8266) || defined(ESP32)
  if (sscanf(payload,
             "%" BUFSIZE_METHOD_SCANF "s %" BUFSIZE_PATH_SCANF "s",
             http_method, http_path) != 2)
#else
  if (sscanf_P(payload,
               PSTR("%" BUFSIZE_METHOD_SCANF "s %" BUFSIZE_PATH_SCANF "s"),
               http_method, http_path) != 2)
#endif
  {
    // Malformed request
    strcpy_P(response, http_400);
    strcat_P(response, http_header_without_content);
    return;
  }

  // Scan HTTP headers for Content-Type for handlers
  // Anyway we are returning application/json so not checking Accept
  if (strstr_P(payload, PSTR("Content-Type: application/json")) ||
      strstr_P(payload, PSTR("content-type: application/json"))) // HTTP/2 (?)
    is_ctype_json = true;

  // URL dispatcher
  if (strcasecmp_P(http_path, PSTR("/")) == 0) {
    webthing_handle_thing_description(response, http_method);
  } else if (strcasecmp_P(http_path, PSTR("/things")) == 0 ||
           strcasecmp_P(http_path, PSTR("/things/")) == 0) {
    webthing_handle_things_description(response, http_method);
  } else if (strcasecmp_P(http_path, PSTR("/things/" WEBTHING_NAME)) == 0 ||
           strcasecmp_P(http_path, PSTR("/things/" WEBTHING_NAME "/")) == 0) {
    webthing_handle_thing_description(response, http_method);
  } else if (strncasecmp_P(http_path,
                         PSTR("/things/" WEBTHING_NAME "/properties"),
                         strlen_P(PSTR("/things/" WEBTHING_NAME "/properties"))) == 0) {
    webthing_handle_thing_properties(response,
                                     is_ctype_json,
                                     http_method,
                                     http_path + strlen_P(PSTR("/things/" WEBTHING_NAME "/properties")),
                                     strstr_P(payload, PSTR("\r\n\r\n")) + 4);
  } else if (strncasecmp_P(http_path,
                         PSTR("/things/" WEBTHING_NAME "/actions"),
                         strlen_P(PSTR("/things/" WEBTHING_NAME "/actions"))) == 0) {
    webthing_handle_thing_actions(response,
                                  is_ctype_json,
                                  http_method,
                                  http_path + strlen_P(PSTR("/things/" WEBTHING_NAME "/actions")));
  } else if (strncasecmp_P(http_path,
                         PSTR("/things/" WEBTHING_NAME "/events"),
                         strlen_P(PSTR("/things/" WEBTHING_NAME "/events"))) == 0) {
    webthing_handle_thing_events(response,
                                 http_method,
                                 http_path + strlen_P(PSTR("/things/" WEBTHING_NAME "/events")));
  } else {
    strcpy_P(response, http_404);
    strcpy_P(response + strlen_P(http_404), http_header_without_content);
  }
}

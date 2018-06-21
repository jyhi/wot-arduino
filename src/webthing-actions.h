#ifndef WEBTHING_ACTIONS_H
#define WEBTHING_ACTIONS_H

#include "webthing-defines.h"

#define ACTION(name, desc) \
  "\"" name "\":{" \
    "\"description\":\"" desc "\"," \
    "\"href\":\"/things/" WEBTHING_NAME "/actions/" name "\"" \
  "}"

#define WEBTHING_DESCRIPTION_ACTIONS \
  "\"actions\":{" \
    ACTION("reboot", "Restart the device") \
  "}"

#ifdef __cplusplus
extern "C" {
#endif

void webthing_handle_thing_actions(char *response, bool is_ctype_json, const char *method, const char *path, const char *message);

#ifdef __cplusplus
}
#endif

#endif

#ifndef WEBTHING_EVENTS_H
#define WEBTHING_EVENTS_H

#include "webthing-defines.h"

#define EVENT(name, type, desc) \
  "\"" name "\":{" \
    "\"type\":\"" type "\"," \
    "\"description\":\"" desc "\"" \
  "}"

#define WEBTHING_DESCRIPTION_EVENTS \
  "\"events\":{" \
  "}"

#ifdef __cplusplus
extern "C" {
#endif

void webthing_handle_thing_events(char *response, const char *method, const char *path);

#ifdef __cplusplus
}
#endif

#endif

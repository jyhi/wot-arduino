#ifndef WEBTHING_DESCRIPTION_H
#define WEBTHING_DESCRIPTION_H

#include "webthing-defines.h"
#include "webthing-properties.h"
#include "webthing-actions.h"
#include "webthing-events.h"

static const char webthing_description[] PROGMEM =
  "{"
    "\"name\":\"" WEBTHING_NAME "\","
    "\"type\":\"" WEBTHING_TYPE "\","
    "\"description\":\"" WEBTHING_DESC "\","
    WEBTHING_DESCRIPTION_PROPERTIES ","
    WEBTHING_DESCRIPTION_ACTIONS ","
    WEBTHING_DESCRIPTION_EVENTS
  "}";

#ifdef __cplusplus
extern "C" {
#endif

void webthing_handle_thing_description(char *response, const char *method);
void webthing_handle_things_description(char *response, const char *method);

#ifdef __cplusplus
}
#endif

#endif

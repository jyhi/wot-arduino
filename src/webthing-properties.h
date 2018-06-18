#ifndef WEBTHING_PROPERTIES_H
#define WEBTHING_PROPERTIES_H

#include "webthing-defines.h"
#include "utils.h"

#define PROPERTY_DESCRIPTION(name, type, desc) \
  "\"" name "\":{" \
    "\"type\":\"" type "\"," \
    "\"description\":\"" desc "\"," \
    "\"href\":\"/things/" WEBTHING_NAME "/properties/" name "\"" \
  "}"

#define PROPERTY_RESOURCE(name, defval) \
  "{" \
    "\"" name "\":" STR(defval) \
  "}"

#define WEBTHING_DESCRIPTION_PROPERTIES \
  "\"properties\":{" \
    PROPERTY_DESCRIPTION("on", "boolean", "Switch LED on / off") \
  "}"

#define WEBTHING_RESOURCE_PROPERTY_ON PROPERTY_RESOURCE("on", false)

#ifdef __cplusplus
extern "C" {
#endif

void webthing_handle_thing_properties(char *response, bool is_ctype_json, const char *method, const char *path, const char *message);

#ifdef __cplusplus
}
#endif

#endif

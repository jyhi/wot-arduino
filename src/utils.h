#ifndef WEBTHING_UTILS_H
#define WEBTHING_UTILS_H

#define _STR(s) #s
#define STR(s) _STR(s)

#ifdef ARDUINO_AVR_UNO

#ifdef __cplusplus
extern "C" {
#endif

void network_maintain(void);

#ifdef __cplusplus
}
#endif

#endif // #ifdef ARDUINO_AVR_UNO

#endif // #ifndef WEBTHING_UTILS_H

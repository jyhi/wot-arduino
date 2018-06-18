#ifndef WEBTHING_UTILS_H
#define WEBTHING_UTILS_H

#define _STR(s) #s
#define STR(s) _STR(s)

#ifdef ARDUINO_AVR_UNO
void network_maintain(void);
#endif

#endif

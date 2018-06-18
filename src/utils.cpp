#ifdef ARDUINO_AVR_UNO

#include <Arduino.h>
#include <Ethernet.h>

extern "C" {

void network_maintain(void)
{
  switch(Ethernet.maintain()) {
    case 1:
      // renew failed
      Serial.println(F("E| DHCP IP renewal failed!"));
      break;
    case 2:
      // renew success
      Serial.println(F("I| DHCP IP renewal success"));
      Serial.print(F("I| "));
      Serial.println(Ethernet.localIP());
      break;
    case 3:
      // rebind fail
      Serial.println(F("E| DHCP IP rebind failed!"));
      break;
    case 4:
      // rebind success
      Serial.println(F("I| DHCP IP rebind success"));
      Serial.print(F("I| "));
      Serial.println(Ethernet.localIP());
      break;
    case 0: // fall through
    default:
      // nothing happened
      break;
  }
}

} // extern "C"

#endif

#pragma once

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

struct RadioPacket {
    int32_t latitude;
    int32_t longitude;
    uint16_t altitude;
};

String createRadioPacket(RadioPacket& packet, String& callsign, uint8_t version);

RadioPacket parseRadioPacket(const String& packetString);

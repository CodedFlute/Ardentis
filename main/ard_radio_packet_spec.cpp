#include "ard_radio_packet_spec.h"

#include "data_struct.h"

#include <Arduino.h>
#include <stdint.h>

byte* createRadioPacket(ArdDataStruct& packet, char* callsign, uint8_t version) {
    byte* packetBytes = new byte[14]();
    packetBytes[0] = callsign[0];
    packetBytes[1] = callsign[1];
    packetBytes[2] = callsign[2];
    packetBytes[3] = callsign[3];
    packetBytes[4] = callsign[4];
    packetBytes[5] = version;
    packetBytes[6] = (packet.latitude >> 24) & 0xFF; // Most significant byte
    packetBytes[7] = (packet.latitude >> 16) & 0xFF;
    packetBytes[8] = (packet.latitude >> 8) & 0xFF;
    packetBytes[9] = (packet.longitude >> 24) & 0xFF;
    packetBytes[10] = (packet.longitude >> 16) & 0xFF;
    packetBytes[11] = (packet.longitude >> 8) & 0xFF;
    packetBytes[12] = (packet.altitude >> 8) & 0xFF;
    packetBytes[13] = packet.altitude & 0xFF;
    return packetBytes; // Caller must delete the allocated memory
}

RadioPacket parseRadioPacket(const String& packetString) {
    RadioPacket packet;
    int firstDelim = packetString.indexOf('>');
    int secondDelim = packetString.indexOf('-', firstDelim + 1);
    if (secondDelim != -1) {
        String latString = packetString.substring(firstDelim + 1, secondDelim);
        for (size_t i = 0; i < 3; ++i) {
            String byteStr = latString.substring(i * 3, i * 3 + 2);
            //packet.latitude[i] = (uint8_t)strtol(byteStr.c_str(), nullptr, 16);
        }
    }

    if (firstDelim == -1 || secondDelim == -1) {
        // Handle invalid packet format
        //return packet;
    }

    String callsign = packetString.substring(0, firstDelim);
    //String version = packetString.substring(firstDelim + 1, packetString.indexOf(byteArrayToString(packet.latitude, 3)));

    String latString = packetString.substring(firstDelim + 1, secondDelim);
    // Continue parsing longitude and altitude similarly
    //return packet;
}
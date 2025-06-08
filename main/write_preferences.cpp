#ifdef PREFERENCES
#include <Arduino.h>

#include <Preferences.h>

#include <preference_shorthand.h>

#include <preference_values.h>

void setup() {
    // put your main code here, to run repeatedly:
    Serial.begin(115200);
    Serial.println("Hello World!");
    delay(1000);

    Preferences preferences;
    preferences.begin(WIFINAMESPACE, false);
    preferences.putString(WIFIPASSWORDID, WiFiPassword);
    preferences.putString(WIFISSIDID, WiFiSSID);

    preferences.end();

    preferences.begin(RADIONAMESPACEID, false);
    preferences.putChar(RADIOFREQID, FREQ);
    preferences.putChar(RADIOBANDWIDTHID, BANDWIDTH);
    preferences.putChar(RADIOSFID, 7);
    preferences.putChar(RADIOCRID, 5);

    preferences.putString(RADIOCALLSIGNID, CALLSIGN);

}

#endif
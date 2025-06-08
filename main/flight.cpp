#ifdef FLIGHT
#include "pins.hpp"
#include "fire_modes.hpp"

#include "RadioParameters.h"

#include "ard_radio_packet_spec.h"
#include "data_struct.cpp"

#include <Arduino.h>
#include <Wire.h>

#include <FS.h>
#include <LittleFS.h>

#include <ICM42670P.h>

#include <SPI.h>
#include <SD.h>


#include <RadioLib.h>

//#include <SparkFun_LIS331.h>
#include <MS5611.h>

#include <Preferences.h>

#include <SparkFun_u-blox_GNSS_v3.h>

HardwareSerial gpsUART(ARD_GPS_UART);

SFE_UBLOX_GNSS_SERIAL gps;

File flightLog;

SX1262 radio = new Module(ARD_RADIO_NSS, ARD_RADIO_BUSY, ARD_RADIO_NRST, ARD_RADIO_DIO1);

MS5611 barometer(0x77);

bool fire_signal_1 = false;
bool fire_signal_2 = false;
bool fire_signal_3 = false;
bool fire_signal_4 = false;

bool continuity_1 = false;
bool continuity_2 = false;
bool continuity_3 = false;
bool continuity_4 = false;

bool apogee = false; // Set to true when apogee is detected
uint16_t time_since_apogee = 0; // Set to true when time since apogee is detected
uint16_t altitude = 0;
bool descending = false; // Set to true when descending is detected
bool ascending = false; // Set to true when ascending is detected
uint16_t time_since_liftoff = 0;

uint16_t ref_pressure = 101325U; // Sea level pressure in Pa

uint8_t fire_1_mode = ARD_FIRE_INACTIVE;
uint8_t fire_1_param = 0; // Parameter for fire_1_mode. Meaning depends on mode selected.
uint8_t fire_2_mode = ARD_FIRE_INACTIVE;
uint8_t fire_2_param = 0; // Parameter for fire_2_mode. Meaning depends on mode selected.
uint8_t fire_3_mode = ARD_FIRE_INACTIVE;
uint8_t fire_3_param = 0; // Parameter for fire_3_mode. Meaning depends on mode selected.
uint8_t fire_4_mode = ARD_FIRE_INACTIVE;
uint8_t fire_4_param = 0; // Parameter for fire_4_mode. Meaning depends on mode selected.

uint8_t fire_1_altitude = 0; // in meters
uint8_t fire_2_altitude = 0; // in meters
uint8_t fire_3_altitude = 0; // in meters
uint8_t fire_4_altitude = 0; // in meters


void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  gpsUART.begin(ARD_GPS_BAUD, SERIAL_8N1, ARD_GPS_RX, ARD_GPS_TX); // RX, TX

  gps.begin(gpsUART);

  gps.setNavigationFrequency(25);

  Wire.begin(ARD_SDA, ARD_SCL);
  if (barometer.begin() == true)
  {
    Serial.print("[BARO] MS5611 found at: ");
    Serial.println(barometer.getAddress());
  }
  else
  {
    Serial.println("[ERROR] MS5611 not found. halt.");
  }



  // initialize radio
  Serial.print(F("[RADIO] Initializing..."));
  uint16_t state = radio.begin(FREQ, BANDWIDTH, SF, CR, LoRa_SYNCWORD, RADIO_TX_PWR);
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println(F("[RADIO] Initialization successful!"));
  } else {
    Serial.print(F("[ERROR] RADIO failed to initialize. Error code: "));
    Serial.println(state);
  }


  if (!SD.begin(4)) {
    Serial.println("[ERROR] SD initialization failed!");
    while (1);
  }
  Serial.println("[SD] initialization done.");

  flightLog = SD.open("flight.log", FILE_WRITE);
  if (flightLog) {
    Serial.print("[SD] Writing to flight.log...");
    flightLog.println("testing 1, 2, 3.");
    // close the file:
    flightLog.close();
    Serial.println("[SD] Done writing to flight.log");
  } else {
    // if the file didn't open, print an error:
    Serial.println("[ERROR] Error opening flight.log");

  }

  // Initialize Fire Output GPIOs
  pinMode(ARD_FIRE1, OUTPUT);
  digitalWrite(ARD_FIRE1, HIGH);
  pinMode(ARD_FIRE2, OUTPUT);
  digitalWrite(ARD_FIRE2, HIGH);
  pinMode(ARD_FIRE3, OUTPUT);
  digitalWrite(ARD_FIRE3, HIGH);
  pinMode(ARD_FIRE4, OUTPUT);
  digitalWrite(ARD_FIRE4, HIGH);

  pinMode(ARD_CONT1, INPUT_PULLUP);
  pinMode(ARD_CONT2, INPUT_PULLUP);
  pinMode(ARD_CONT3, INPUT_PULLUP);
  pinMode(ARD_CONT4, INPUT_PULLUP);

  Serial.println("[OUTPUT] Output GPIOs initialized.");

}

void loop() {

  ArdDataStruct data;

  // Check Continuity
  continuity_1 = digitalRead(ARD_CONT1);
  continuity_2 = digitalRead(ARD_CONT2);
  continuity_3 = digitalRead(ARD_CONT3);
  continuity_4 = digitalRead(ARD_CONT4);

  if(gps.getPVT() == true) {
    Serial.print("Latitude= "); Serial.print(gps.getLatitude() * 10e7, 6); Serial.print(" Longitude= "); Serial.println(gps.getLongitude() * 10e7, 6);
    Serial.print("Altitude= "); Serial.print(gps.getAltitude() * 1000, 6); Serial.println(" m");
    Serial.print("Horizontal Accuracy= "); Serial.println(gps.getHorizontalAccuracy() * 1000, 6); Serial.println(" m");
    Serial.print("Vertical Accuracy= "); Serial.println(gps.getVerticalAccuracy() * 1000, 6); Serial.println(" m");
    Serial.print("Date= "); Serial.print(gps.getDay()); Serial.print("/"); Serial.print(gps.getMonth()); Serial.print("/"); Serial.println(gps.getYear());
    Serial.print("Time= "); Serial.print(gps.getHour()); Serial.print(":"); Serial.print(gps.getMinute()); Serial.print(":"); Serial.println(gps.getSecond());
  }

  barometer.read();
  data.pressure = barometer.getPressure() * 100;
  data.temperature = barometer.getTemperature();
  data.latitude = gps.getLatitude() * 10e7; // Convert latitude to degrees
  data.longitude = gps.getLongitude() * 10e7; // Convert longitude to degrees


  for (int i = 1; i <= 4; i++) {
    uint8_t fire_mode = 0;
    uint16_t fire_param = 0;
    bool* fire_signal = nullptr;

    switch (i) {
      case 1:
        fire_mode = fire_1_mode;
        fire_param = fire_1_param;
        fire_signal = &fire_signal_1;
        break;
      case 2:
        fire_mode = fire_2_mode;
        fire_param = fire_2_param;
        fire_signal = &fire_signal_2;
        break;
      case 3:
        fire_mode = fire_3_mode;
        fire_param = fire_3_param;
        fire_signal = &fire_signal_3;
        break;
      case 4:
        fire_mode = fire_4_mode;
        fire_param = fire_4_param;
        fire_signal = &fire_signal_4;
        break;
    }

    if (fire_mode == 1) { // Fire at apogee
      *fire_signal = (apogee == true) && (time_since_apogee > fire_param);
    } else if (fire_mode == 2) { // Fire at or below set altitude on descent
      *fire_signal = (altitude <= fire_param) && (descending == true);
    } else if (fire_mode == 3) { // Fire at or above set altitude on ascent
      *fire_signal = (altitude >= fire_param) && (ascending == true);
    } else if (fire_mode == 4) { // Fire at set time after liftoff
      *fire_signal = time_since_apogee > fire_param;
    } else {
      *fire_signal = false;
    }
  }
  

  RadioPacket packet;
  uint16_t altitude = static_cast<uint32_t>(gps.getAltitude() * 1000); // Convert altitude to meters
  packet.altitude[0] = static_cast<uint8_t>((data.getAltitude(101325) >> 8) & 0xFF);
  packet.altitude[1] = static_cast<uint8_t>(data.getAltitude(101325) & 0xFF);
  uint32_t latitude = static_cast<uint32_t>(gps.getLatitude() * 10e7);
  packet.latitude[0] = static_cast<uint8_t>((latitude >> 16) & 0xFF);
  packet.latitude[1] = static_cast<uint8_t>((latitude >> 8) & 0xFF);
  packet.latitude[2] = static_cast<uint8_t>(latitude & 0xFF);
  uint32_t longitude = static_cast<uint32_t>(gps.getLongitude() * 10e7);
  packet.longitude[0] = static_cast<uint8_t>((longitude >> 16) & 0xFF);
  packet.longitude[1] = static_cast<uint8_t>((longitude >> 8) & 0xFF);
  packet.longitude[2] = static_cast<uint8_t>(longitude & 0xFF);


  createRadioPacket(packet, CALLSIGN, 'a');

  Serial.println(F("[RADIO] Sending packet"));
  uint8_t serializedPacket[sizeof(packet)];
  memcpy(serializedPacket, &packet, sizeof(packet));
  uint16_t state = radio.transmit(serializedPacket, sizeof(serializedPacket));
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("[RADIO] Packet sent successfully!"));
  } else {
    Serial.print(F("[ERROR] Failed to send packet. Error code: "));
    Serial.println(state);
  }

  if (fire_signal_1) {
    Serial.println("[OUTPUT] Firing Output 1.");
    digitalWrite(ARD_FIRE1, LOW);
    delay(1000);
    digitalWrite(ARD_FIRE1, HIGH);
  }

}
#endif
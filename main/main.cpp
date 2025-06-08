/** 
 * @file main.cpp
 * @author Roland Neill
 * @brief Main file for the Ardentis flight computer code.
 * @version 0.1-alpha
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025
 * 
 * @page main
 */
#include "pins.h"
#include "fire_modes.hpp"

#include "RadioParameters.h"

#include "ard_radio.h"

ard_RadioConfig_t radioConfig = {
  	ARD_RADIO_FREQ,
  	ARD_LORA_BW,
  	ARD_LORA_SF,
	ARD_LORA_CR,
  	ARD_LORA_SYNCWORD,
  	ARD_RADIO_TX_PWR
};
ard_RadioPins_t radioPins = {
  	ARD_RADIO_NSS,
  	ARD_RADIO_BUSY,
  	ARD_RADIO_NRST,
  	ARD_RADIO_DIO1,
  	ARD_RADIO_SPI_MISO,
  	ARD_RADIO_SPI_MOSI,
  	ARD_RADIO_SPI_SCK
};

ardRadio radio(radioPins);

#include "data_struct.h"

#include "ard_deployment.h"


#include "ard_log.h"

const char* main_TAG = "Main";

ardLog main_log(main_TAG);

void txCallback(void) {
  	// Callback function for when the radio is done transmitting
  	radio.doneTX();
}

#include <Arduino.h>
#include <Wire.h>

#include <FS.h>
#include <LittleFS.h>

#include <SD.h>

#include <Preferences.h>

#include "ard_sensor.h"

ardSensors sensors(ARD_SDA, ARD_SCL, 1, ARD_GPS_UART, ARD_GPS_TX, ARD_GPS_RX);


File flightLog;

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

// TODO: Change these parameters to read from preferences when implemented
int ref_pressure = 101325; // Sea level pressure in Pa

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

	radio.begin(radioConfig);
	radio.setTxInt(txCallback);

  	if (sensors.initSensors(0x18, 0x68, 0x77, ARD_GPS_BAUD) != ERROR_NONE) {
		main_log.error("Sensor initialization failed.");
  	} else {
		main_log.info("Sensor initialization successful.");
  	}


  	// SD Card Setup
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
  
  	sensors.update();



  	// Check Continuity
  	data.cont1 = digitalRead(ARD_CONT1);
  	data.cont2 = digitalRead(ARD_CONT2);
  	data.cont3 = digitalRead(ARD_CONT3);
  	data.cont4 = digitalRead(ARD_CONT4);


  	for (int i = 1; i <= 4; i++) {
		uint8_t fire_mode = 0;
		uint16_t fire_param = 0;
		bool* fire_signal = nullptr;
		bool* data_signal = nullptr;

		switch (i) {
	  	case 1:
			fire_mode = fire_1_mode;
			fire_param = fire_1_param;
			fire_signal = &fire_signal_1;
			data_signal = &data.fired_1;
			break;
	  	case 2:
			fire_mode = fire_2_mode;
			fire_param = fire_2_param;
			fire_signal = &fire_signal_2;
			data_signal = &data.fired_2;
			break;
		case 3:
			fire_mode = fire_3_mode;
			fire_param = fire_3_param;
			fire_signal = &fire_signal_3;
			data_signal = &data.fired_3;
			break;
		case 4:
			fire_mode = fire_4_mode;
			fire_param = fire_4_param;
			fire_signal = &fire_signal_4;
			data_signal = &data.fired_4;
			break;
		}
		// Check if the fire mode is active
		if (fire_mode == 1) { // Fire at apogee
			if (apogee == true && time_since_apogee > fire_param) {
				*fire_signal = true;
				*data_signal = true;
			} else {
				*fire_signal = false;
			}
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

	if (!radio.busy) {
		RadioPacket radioPacket;
		strncpy(radioPacket.callsign, "VE3FSZ", sizeof(radioPacket.callsign) - 1);
		radioPacket.callsign[sizeof(radioPacket.callsign) - 1] = '\0';
		radioPacket.latitude = data.latitude;
		radioPacket.longitude = data.longitude;
		radioPacket.altitude = data.altitude;

		radio.sendData(radioPacket);


  		Serial.println(F("[RADIO] Sending packet"));

		if (fire_signal_1) {
			Serial.println("[OUTPUT] Firing Output 1.");
			digitalWrite(ARD_FIRE1, LOW);
			delay(1000);
			digitalWrite(ARD_FIRE1, HIGH);
		}
	}
}

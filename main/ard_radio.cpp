/**
 * @file ard_radio.cpp
 * @author Roland Neill
 * @brief Implementation code for the radio module for the Ardentis flight computer.
 * @version 0.1-alpha
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ard_radio.h"

#include "ard_log.h"

#include "driver/gpio.h"

const char* radio_TAG = "Radio";

ardLog radio_log(radio_TAG);

#include <Arduino.h>

#include <SPI.h>

byte* encodePacket(RadioPacket packet) {
	byte* encodedData = new byte[16];
	encodedData[0] = packet.callsign[0];
	encodedData[1] = packet.callsign[1];
	encodedData[2] = packet.callsign[2];
	encodedData[3] = packet.callsign[3];
	encodedData[4] = packet.callsign[4];
	encodedData[5] = uint8_t(0); // Packet Version number
	encodedData[6] = uint8_t(packet.latitude >> 24);
	encodedData[7] = uint8_t(packet.latitude >> 16);
	encodedData[8] = uint8_t(packet.latitude >> 8);
	encodedData[9] = uint8_t(packet.latitude);
	encodedData[10] = uint8_t(packet.longitude >> 24);
	encodedData[11] = uint8_t(packet.longitude >> 16);
	encodedData[12] = uint8_t(packet.longitude >> 8);
	encodedData[13] = uint8_t(packet.longitude);
	encodedData[14] = uint8_t(packet.altitude >> 8);
	encodedData[15] = uint8_t(packet.altitude);
	return encodedData;
}


ardRadio::ardRadio(ard_RadioPins_t pins) {
	this->pins = pins;
	radio = new Module(pins.nss, pins.busy, pins.nrst, pins.dio1, spi, RADIOLIB_DEFAULT_SPI_SETTINGS);
}

ard_errorCode_t ardRadio::begin(ard_RadioConfig_t config) {
	config = config;
	spi.begin(pins.sck, pins.miso, pins.mosi);
	radio_log.info("Initializing radio module...");
	int state = radio.begin(config.frequency, config.bandwidth, config.spreadFactor, config.codingRate, config.syncWord, config.txPower);
	if (state == RADIOLIB_ERR_NONE) {
		radio_log.info("Radio module initialized successfully.");
		return ard_errorCode_t::ERROR_NONE;
	} else {
		radio_log.error("Radio module initialization failed.");
		return ard_errorCode_t::ERROR_GENERIC;
	}
}

void ardRadio::setTxInt(void (*callback)(void)) {
	radio.setPacketSentAction(callback);
}

void ardRadio::setRxInt(void (*callback)(void)) {
	radio.setPacketReceivedAction(callback);
}

ard_errorCode_t ardRadio::sendData(RadioPacket data) {
	byte* encodedData = encodePacket(data);
	int state = radio.startTransmit(encodedData, 16);
	if (state == RADIOLIB_ERR_NONE) {
		radio_log.info("Radio packet sent.");
		return ard_errorCode_t::ERROR_NONE;
	} else {
		radio_log.error("Radio failed to send data.");
		return ard_errorCode_t::ERROR_GENERIC;
	}
}

RadioPacket ardRadio::receiveData(uint8_t* buff, size_t length) {
	byte buffer[16];
	int state = radio.readData(buffer, length);
	if (state == RADIOLIB_ERR_NONE) {
		radio_log.info("Data received successfully.");
		return decodePacket(buffer, 16);
	} else {
		radio_log.error("Failed to receive data.");
		return RadioPacket(); // Return an empty packet on error
	}
}

void ardRadio::doneTX() {
	radio.finishTransmit();
	busy = false;
}
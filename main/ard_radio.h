#pragma once
/**
 * @file ard_radio.h
 * @author Roland Neill
 * @brief 
 * @version 0.1-alpha
 * @date 2025-04-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ard_log.h"

#include <RadioLib.h>

#include <stdint.h>

#include <SPI.h>
#include <Arduino.h>


struct RadioPacket {
	char callsign[7];
	int32_t latitude;
	int32_t longitude;
	uint16_t altitude;
};


/**
 * @struct ard_RadioConfig_t
 * 
 * @brief Structure to hold the LoRa configuration parameters.
 * 
 * 
 * @param frequency The frequency (in MHz) to use.
 * @param bandwidth The bandwidth to use.
 * @param spreadFactor The LoRa spread factor to use.
 * @param codingRate The LoRa coding rate to use.
 * @param syncWord The LoRa sync word to use.
 * @param txPower The transmission power level (in dBm) to use.
 */
struct ard_RadioConfig_t {
	int frequency;
	int bandwidth;
	int spreadFactor;
	int codingRate;
	int syncWord;
	int txPower;
};

/**
 * @brief Structure to hold the radio pin configuration parameters.
 * 
 * @param nss Radio NSS (chip select) pin.
 * @param busy Radio busy pin.
 * @param nrst Radio reset pin.
 * @param dio1 Radio DIO1 pin.
 * @param miso SPI MISO pin.
 * @param mosi SPI MOSI pin.
 * @param sck SPI serial clock.
 */
struct ard_RadioPins_t {
	gpio_num_t nss;
	gpio_num_t busy;
	gpio_num_t nrst;
	gpio_num_t dio1;
	gpio_num_t miso;
	gpio_num_t mosi;
	gpio_num_t sck;
};


byte* encodePacket(RadioPacket packet);

RadioPacket decodePacket(byte data[18], int length);


class ardRadio {
public:
  	ardRadio(ard_RadioPins_t pins);
	ard_errorCode_t begin(ard_RadioConfig_t config);
	void setTxInt(void (*callback)(void));
	void setRxInt(void (*callback)(void));
	RadioPacket receiveData(uint8_t *data, size_t length);
	ard_errorCode_t sendData(RadioPacket data);
	volatile bool busy;
	void doneTX(void);
	void doneRX(void);
private:
	SX1262 radio = NULL;
	ard_RadioPins_t pins;
	ard_RadioConfig_t config;
	SPIClass spi = SPIClass(HSPI);
};

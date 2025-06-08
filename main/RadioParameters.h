//#include <SX126x-Arduino.h>
#include "pins.h"

#define ARD_RADIO_FREQ 915 // MHz
#define ARD_LORA_BW 125 // kHz
#define ARD_LORA_SF 7
#define ARD_LORA_CR 4 // 4/5

#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0   // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 3000
#define TX_TIMEOUT_VALUE 3000

#define ARD_LORA_SYNCWORD 0x12U

#define ARD_RADIO_TX_PWR 22

#define CALLSIGN "VE3FSZ"

#define BUFFER_SIZE 64 // Define the payload size here

//static RadioEvents_t RadioEvents;
//hw_config ardRadioConfig;

// Define the HW configuration between MCU and SX126x
/*void setupRadioConfig() {
	ardRadioConfig.CHIP_TYPE = SX1262_CHIP;         // Example uses an eByte E22 module with an SX1262
	ardRadioConfig.PIN_LORA_RESET = ARD_RADIO_NRST; // LORA RESET
	ardRadioConfig.PIN_LORA_NSS = ARD_RADIO_NSS;     // LORA SPI CS
	ardRadioConfig.PIN_LORA_SCLK = ARD_RADIO_SPI_SCK;   // LORA SPI CLK
	ardRadioConfig.PIN_LORA_MISO = ARD_RADIO_SPI_MISO;   // LORA SPI MISO
	ardRadioConfig.PIN_LORA_DIO_1 = ARD_RADIO_DIO1; // LORA DIO_1
	ardRadioConfig.PIN_LORA_BUSY = ARD_RADIO_BUSY;   // LORA SPI BUSY
	ardRadioConfig.PIN_LORA_MOSI = ARD_RADIO_SPI_MOSI;   // LORA SPI MOSI
	ardRadioConfig.USE_DIO3_ANT_SWITCH = false;     // Only Insight ISP4520 module uses DIO3 as antenna control
	ardRadioConfig.USE_LDO = false;                 // Set to true if SX126x uses LDO instead of DCDC converter
	ardRadioConfig.USE_RXEN_ANT_PWR = false;        // Antenna power is not controlled by a GPIO
}*/
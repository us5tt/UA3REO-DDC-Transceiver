#include "agc.h"
#include "codec.h"
#include "hardware.h"
#include "i2c.h"
#include "lcd.h"
#include "trx_manager.h"
#include "usbd_audio_if.h"

#ifdef HRDW_AUDIO_CODEC_WM8731

// send I2C command to codec
static uint8_t WM8731_SendI2CCommand(uint8_t reg, uint8_t value) {
	uint8_t st = 2;
	uint8_t repeats = 0;
	while (st != 0 && repeats < 3) {
		i2c_beginTransmission_u8(&I2C_CODEC, B8(0011010)); // I2C_ADDRESS_WM8731 00110100
		i2c_write_u8(&I2C_CODEC, reg);                     // MSB
		i2c_write_u8(&I2C_CODEC, value);                   // MSB
		st = i2c_endTransmission(&I2C_CODEC);
		if (st != 0) {
			repeats++;
		}
		HAL_Delay(1);
	}
	return st;
}

// switch to mixed RX-TX mode (for LOOP)
void CODEC_TXRX_mode(void) // loopback
{
	WM8731_SendI2CCommand(B8(00000100), B8(01111110)); // R2 Left Headphone Out
	WM8731_SendI2CCommand(B8(00000110), B8(01111110)); // R3 Right Headphone Out
	WM8731_SendI2CCommand(B8(00001010), B8(00010110)); // R5 Digital Audio Path Control De-emphasis

	if (getInputType() == TRX_INPUT_LINE) // line
	{
		WM8731_SendI2CCommand(B8(00000000), B8(00010111)); // R0 Left Line In
		WM8731_SendI2CCommand(B8(00000010), B8(00010111)); // R1 Right Line In
		WM8731_SendI2CCommand(B8(00001000), B8(00010010)); // R4 Analogue Audio Path Control
		WM8731_SendI2CCommand(B8(00001100), B8(01100010)); // R6 Power Down Control, internal crystal
	}

	if (getInputType() == TRX_INPUT_MIC) // mic
	{
		WM8731_SendI2CCommand(B8(00000001), B8(10000000)); // R0 Left Line In
		WM8731_SendI2CCommand(B8(00000011), B8(10000000)); // R1 Right Line In
		if (TRX.MIC_Boost) {
			WM8731_SendI2CCommand(B8(00001000), B8(00010101)); // R4 Analogue Audio Path Control
		} else {
			WM8731_SendI2CCommand(B8(00001000), B8(00010100)); // R4 Analogue Audio Path Control
		}
		WM8731_SendI2CCommand(B8(00001100), B8(01100001)); // R6 Power Down Control, internal crystal
	}
}

// initialize the audio codec over I2C
void CODEC_Init(void) {
	if (WM8731_SendI2CCommand(B8(00011110), B8(00000000)) != 0) // R15 Reset Chip
	{
		println("[ERR] Audio codec not found");
		LCD_showError("Audio codec init error", true);
		CODEC_test_result = false;
	}
	WM8731_SendI2CCommand(B8(00000101), B8(10000000)); // R2 Left Headphone Out Mute
	WM8731_SendI2CCommand(B8(00000111), B8(10000000)); // R3 Right Headphone Out Mute
	WM8731_SendI2CCommand(B8(00001110),
	                      B8(00001110));               // R7 Digital Audio Interface Format, Codec Slave, 32bits, I2S Format, MSB-First left-1 justified
	WM8731_SendI2CCommand(B8(00010000), B8(00000000)); // R8 Sampling Control normal mode, 256fs, SR=0 (MCLK@12.288Mhz, fs=48kHz))
	WM8731_SendI2CCommand(B8(00010010), B8(00000001)); // R9 reactivate digital audio interface
	WM8731_SendI2CCommand(B8(00000000), B8(10000000)); // R0 Left Line In
	WM8731_SendI2CCommand(B8(00000010), B8(10000000)); // R1 Right Line In
	WM8731_SendI2CCommand(B8(00001000), B8(00010110)); // R4 Analogue Audio Path Control
	WM8731_SendI2CCommand(B8(00001010), B8(00010000)); // R5 Digital Audio Path Control
	WM8731_SendI2CCommand(B8(00001100), B8(01100111)); // R6 Power Down Control
	CODEC_UnMute();
}

#endif

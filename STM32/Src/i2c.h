#ifndef WIRE_h
#define WIRE_h

#include "hardware.h"
#include "main.h"
#include "touchpad.h"
#include <stdbool.h>

#define WIRE_BUFSIZ 201

/* return codes from endTransmission() */
#define SUCCESS 0   /* transmission was successful */
#define EDATA 1     /* too much data */
#define ENACKADDR 2 /* received nack on transmit of address */
#define ENACKTRNS 3 /* received nack on transmit of data */
#define EOTHER 4    /* other error */

#define I2C_WRITE 0
#define I2C_READ 1
#define I2C_DELAY                                   \
	for (uint32_t wait_i = 0; wait_i < 400; wait_i++) \
		__asm("nop");
#define I2C_OUTPUT_MODE GPIO_MODE_OUTPUT_OD
#define I2C_INPUT_MODE GPIO_MODE_INPUT
#define I2C_PULLS GPIO_PULLUP
#define I2C_GPIO_SPEED GPIO_SPEED_FREQ_LOW

typedef struct {
	GPIO_TypeDef *SDA_PORT;
	uint16_t SDA_PIN;
	GPIO_TypeDef *SCK_PORT;
	uint16_t SCK_PIN;
	uint8_t i2c_tx_addr;             /* address transmitting to */
	uint8_t i2c_tx_buf[WIRE_BUFSIZ]; /* transmit buffer */
	uint8_t i2c_tx_buf_idx;          /* next idx available in tx_buf, -1 overflow */
	bool i2c_tx_buf_overflow;
} I2C_DEVICE;

extern I2C_DEVICE I2C_CODEC;
#ifdef HAS_TOUCHPAD
extern I2C_DEVICE I2C_TOUCHPAD;
#endif

#define SDA_SET HAL_GPIO_WritePin(dev->SDA_PORT, dev->SDA_PIN, GPIO_PIN_SET)
#define SCK_SET HAL_GPIO_WritePin(dev->SCK_PORT, dev->SCK_PIN, GPIO_PIN_SET)
#define SDA_CLR HAL_GPIO_WritePin(dev->SDA_PORT, dev->SDA_PIN, GPIO_PIN_RESET)
#define SCK_CLR HAL_GPIO_WritePin(dev->SCK_PORT, dev->SCK_PIN, GPIO_PIN_RESET)

extern void i2c_begin(I2C_DEVICE *dev);
extern void i2c_beginTransmission_u8(I2C_DEVICE *dev, uint8_t slave_address);
extern void i2c_write_u8(I2C_DEVICE *dev, uint8_t value);
extern void i2c_shift_out(I2C_DEVICE *dev, uint8_t val);
extern void i2c_start(I2C_DEVICE *dev);
extern void i2c_stop(I2C_DEVICE *dev);
extern bool i2c_get_ack(I2C_DEVICE *dev);
extern bool i2c_beginReceive_u8(I2C_DEVICE *dev, uint8_t slave_address);
extern uint8_t i2c_Read_Byte(I2C_DEVICE *dev, uint8_t ack);
extern uint16_t i2c_Read_Word(I2C_DEVICE *dev); // Tisho
extern uint8_t i2c_endTransmission(I2C_DEVICE *dev);

#endif

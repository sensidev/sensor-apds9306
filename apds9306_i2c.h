#ifndef SENSOR_APDS9306_I2C_H
#define SENSOR_APDS9306_I2C_H

#include "stdint.h"

#define APDS9306_I2C_MAX_BUFF_SIZE 10

int8_t apds9306_i2c_read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t count);

int8_t apds9306_i2c_write(uint8_t address, uint8_t reg, const uint8_t *data, uint16_t count);

void apds9306_i2c_delay_ms(uint32_t delay);

#endif
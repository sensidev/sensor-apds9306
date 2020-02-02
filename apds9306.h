#ifndef SENSOR_APDS9306_H
#define SENSOR_APDS9306_H

#include "stdbool.h"
#include "stdint.h"

// Registers
#define APDS9306_CMD_MAIN_CTRL  0x00
#define APDS9306_CMD_ALS_MEAS_RATE  0x04
#define APDS9306_CMD_ALS_GAIN 0x05
#define APDS9306_CMD_PART_ID 0x06
#define APDS9306_CMD_MAIN_STATUS 0x07
#define APDS9306_CMD_CLEAR_DATA_0 0x0A
#define APDS9306_CMD_CLEAR_DATA_1 0x0B
#define APDS9306_CMD_CLEAR_DATA_2 0x0C
#define APDS9306_CMD_ALS_DATA_0 0x0D
#define APDS9306_CMD_ALS_DATA_1 0x0E
#define APDS9306_CMD_ALS_DATA_2 0x0F
#define APDS9306_CMD_INT_CFG 0x19
#define APDS9306_CMD_INT_PERSISTENCE 0x1A
#define APDS9306_CMD_ALS_THRES_UP_0 0x21
#define APDS9306_CMD_ALS_THRES_UP_1 0x22
#define APDS9306_CMD_ALS_THRES_UP_2 0x23
#define APDS9306_CMD_ALS_THRES_LOW_0 0x24
#define APDS9306_CMD_ALS_THRES_LOW_1 0x25
#define APDS9306_CMD_ALS_THRES_LOW_2 0x26
#define APDS9306_CMD_ALS_THRES_VAR 0x27

// MAIN_CTRL register
#define APDS9306_MAIN_CTRL_SW_RESET 0x10u
#define APDS9306_MAIN_CTRL_ALS_ENABLE 0x02u
#define APDS9306_MAIN_CTRL_ALS_STANDBY 0x00u

// ALS_MEAS_RES register - Resolution/Bit Width
#define APDS9306_ALS_MEAS_RES_20BIT_400MS 0x00u
#define APDS9306_ALS_MEAS_RES_19BIT_200MS 0x10u
#define APDS9306_ALS_MEAS_RES_18BIT_100MS 0x20u // default
#define APDS9306_ALS_MEAS_RES_17BIT_50MS 0x30u
#define APDS9306_ALS_MEAS_RES_16BIT_25MS 0x40u
#define APDS9306_ALS_MEAS_RES_13BIT_3MS 0x50u

// ALS_MEAS_RES register - Measurement Rate
#define APDS9306_ALS_MEAS_RATE_25MS 0x00u
#define APDS9306_ALS_MEAS_RATE_50MS 0x01u
#define APDS9306_ALS_MEAS_RATE_100MS 0x02u // default
#define APDS9306_ALS_MEAS_RATE_200MS 0x03u
#define APDS9306_ALS_MEAS_RATE_500MS 0x04u
#define APDS9306_ALS_MEAS_RATE_1000MS 0x05u
#define APDS9306_ALS_MEAS_RATE_2000MS 0x06u

// ALS_GAIN register
#define APDS9306_ALS_GAIN_1 0x00u
#define APDS9306_ALS_GAIN_3 0x01u // default
#define APDS9306_ALS_GAIN_6 0x02u
#define APDS9306_ALS_GAIN_9 0x03u
#define APDS9306_ALS_GAIN_18 0x04u

// PART_ID register
#define APDS9306_PART_ID_APDS9306 0xB1u
#define APDS9306_PART_ID_APDS9306065 0xB3u

// MAIN_STATUS register
#define APDS9306_MAIN_STATUS_POWER_ON 0x20u
#define APDS9306_MAIN_STATUS_ALS_INT 0x10u
#define APDS9306_MAIN_STATUS_ALS_DATA 0x08u

#define APDS9306_READ_WAIT_FOR_REG_ATTEMPTS 20

#ifndef I2C_APDS9306_ADDRESS
#define I2C_APDS9306_ADDRESS 0x52u  // use default address, if not defined.
#endif

int16_t apds9306_probe();

int16_t apds9306_reset();

int16_t apds9306_set_als_meas_rate(uint8_t value);

int16_t apds9306_set_als_gain(uint8_t value);

bool apds9306_is_data_ready();

int8_t apds9306_get_raw_data_wait(uint32_t *data);

int8_t apds9306_read_wait(float *luminosity);

float apds9306_get_resolution_time_for(uint8_t value);

float apds9306_get_gain_for(uint8_t value);

int16_t apds9306_als_enable();

int16_t apds9306_als_standby();

#endif
#include "apds9306.h"
#include "apds9306_i2c.h"
#include "apds9306_errors.h"

static uint8_t apds9306_als_meas_rate_value = APDS9306_ALS_MEAS_RES_18BIT_100MS | APDS9306_ALS_MEAS_RATE_100MS;
static uint8_t apds9306_als_gain_value = APDS9306_ALS_GAIN_3;

int16_t apds9306_probe() {
    uint8_t buff[1];
    apds9306_i2c_read(I2C_APDS9306_ADDRESS, APDS9306_CMD_PART_ID, buff, 1);

    bool is_valid_part_id = buff[0] == APDS9306_PART_ID_APDS9306 || buff[0] == APDS9306_PART_ID_APDS9306065;

    if (!is_valid_part_id) {
        return APDS9306_PART_ID_ERROR;
    }

    return APDS9306_OK;
}

int16_t apds9306_reset() {
    uint8_t buff[1];
    buff[0] = APDS9306_MAIN_CTRL_SW_RESET;
    return apds9306_i2c_write(I2C_APDS9306_ADDRESS, APDS9306_CMD_MAIN_CTRL, buff, 1);
}

int16_t apds9306_set_als_meas_rate(uint8_t value) {
    uint8_t buff[1];
    buff[0] = value;
    apds9306_als_meas_rate_value = value;
    return apds9306_i2c_write(I2C_APDS9306_ADDRESS, APDS9306_CMD_ALS_MEAS_RATE, buff, 1);
}


int16_t apds9306_set_als_gain(uint8_t value) {
    uint8_t buff[1];
    buff[0] = value;
    apds9306_als_gain_value = value;
    return apds9306_i2c_write(I2C_APDS9306_ADDRESS, APDS9306_CMD_ALS_GAIN, buff, 1);
}

int16_t apds9306_als_enable() {
    uint8_t buff[1];
    buff[0] = APDS9306_MAIN_CTRL_ALS_ENABLE;
    return apds9306_i2c_write(I2C_APDS9306_ADDRESS, APDS9306_CMD_MAIN_CTRL, buff, 1);
}

int16_t apds9306_als_standby() {
    uint8_t buff[1];
    buff[0] = APDS9306_MAIN_CTRL_ALS_STANDBY;
    return apds9306_i2c_write(I2C_APDS9306_ADDRESS, APDS9306_CMD_MAIN_CTRL, buff, 1);
}

bool apds9306_is_data_ready() {
    uint8_t buff[1] = {0x00};
    apds9306_i2c_read(I2C_APDS9306_ADDRESS, APDS9306_CMD_MAIN_STATUS, buff, 1);
    return buff[0] & APDS9306_MAIN_STATUS_ALS_DATA;
}

float apds9306_get_resolution_time_for(uint8_t value) {
    switch (value & 0xF0u) {
        case APDS9306_ALS_MEAS_RES_20BIT_400MS :
            return 400.0f;
        case APDS9306_ALS_MEAS_RES_19BIT_200MS :
            return 200.0f;
        case APDS9306_ALS_MEAS_RES_18BIT_100MS :
            return 100.0f;
        case APDS9306_ALS_MEAS_RES_17BIT_50MS :
            return 50.0f;
        case APDS9306_ALS_MEAS_RES_16BIT_25MS :
            return 25.0f;
        case APDS9306_ALS_MEAS_RES_13BIT_3MS :
            return 3.125f;
        default:
            return 100.0f;
    }
}

float apds9306_get_gain_for(uint8_t value) {
    switch (value) {
        case APDS9306_ALS_GAIN_1 :
            return 1.0f;
        case APDS9306_ALS_GAIN_3 :
            return 3.0f;
        case APDS9306_ALS_GAIN_6 :
            return 6.0f;
        case APDS9306_ALS_GAIN_9 :
            return 9.0f;
        case APDS9306_ALS_GAIN_18 :
            return 18.0f;
        default:
            return 3.0f;
    }
}

int8_t apds9306_get_raw_data_wait(uint32_t *data) {
    uint8_t buff[3] = {0x00, 0x00, 0x00};
    int8_t ret;
    uint8_t attempts = 0;

    apds9306_als_enable();

    // Wait to make the measurement
    uint16_t delay = apds9306_get_resolution_time_for(apds9306_als_meas_rate_value);
    apds9306_i2c_delay_ms(delay);

    while (!apds9306_is_data_ready() && attempts < APDS9306_READ_WAIT_FOR_REG_ATTEMPTS) {
        apds9306_i2c_delay_ms(20);
        attempts++;
    }
    apds9306_als_standby();

    ret = apds9306_i2c_read(I2C_APDS9306_ADDRESS, APDS9306_CMD_ALS_DATA_0, buff, 3);

    if (ret != APDS9306_OK) {
        return ret;
    }

    *data = (uint8_t) buff[0];
    *data |= (uint8_t) (buff[1] << 8u);
    *data |= (uint8_t) (buff[2] << 16u);

    return APDS9306_OK;
}

int8_t apds9306_read_wait(float *luminosity) {
    uint32_t data;
    int8_t ret = apds9306_get_raw_data_wait(&data);

    float rate = apds9306_get_resolution_time_for(apds9306_als_meas_rate_value);
    float gain = apds9306_get_gain_for(apds9306_als_gain_value);

    *luminosity = (data / gain) * (100.0f / rate);

    return ret;
}
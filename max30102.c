#include "max30102.h"
#include <zephyr/drivers/i2c.h>

#define MAX30102_I2C_ADDR  0x57
#define MAX30102_REG_MODE_CONFIG 0x09
#define MAX30102_REG_FIFO_DATA 0x07
#define MAX30102_FIFO_DATA_SIZE  6

static int max30102_read_register(struct device *i2c_dev, uint8_t reg, uint8_t *data, size_t len) {
    return i2c_burst_read(i2c_dev, MAX30102_I2C_ADDR, reg, data, len);
}

static int max30102_write_register(struct device *i2c_dev, uint8_t reg, uint8_t data) {
    return i2c_reg_write_byte(i2c_dev, MAX30102_I2C_ADDR, reg, data);
}

int max30102_init(struct device *i2c_dev) {
    // Reset the sensor to a known state
    int ret = max30102_write_register(i2c_dev, MAX30102_REG_MODE_CONFIG, 0x40);
    if (ret) {
        return ret;
    }

    // Wait for the reset to complete
    k_sleep(K_MSEC(100));

    // Configure the sensor: you can set appropriate configurations here
    // For example, set the sample rate, LED current, and other settings

    // Put the sensor in a suitable mode (e.g., SpO2 mode)
    ret = max30102_write_register(i2c_dev, MAX30102_REG_MODE_CONFIG, 0x03);
    if (ret) {
        return ret;
    }

    return 0;
}

int max30102_read_sensor_data(struct device *i2c_dev, uint32_t *ir_value, uint32_t *red_value) {
    uint8_t data[MAX30102_FIFO_DATA_SIZE];

    int ret = max30102_read_register(i2c_dev, MAX30102_REG_FIFO_DATA, data, MAX30102_FIFO_DATA_SIZE);
    if (ret) {
        return ret;
    }

    // Combine the bytes to obtain the IR and RED values
    *ir_value = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
    *red_value = ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5];

    return 0;
}


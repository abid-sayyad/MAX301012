#ifndef MAX30102_H
#define MAX30102_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>

// Function to initialize the MAX30102 sensor
int max30102_init(struct device *i2c_dev);

// Function to read the sensor data (e.g., IR and RED values)
int max30102_read_sensor_data(struct device *i2c_dev, uint32_t *ir_value, uint32_t *red_value);

#endif /* MAX30102_H */

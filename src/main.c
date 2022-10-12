#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c-example";

#define I2C_SLAVE_ADDR 0b11000000
#define TIMEOUT_MS 1000
#define DELAY_MS 100
#define ACK_CHECK_EN 0x1

static esp_err_t i2c_master_init()
{

    int i2c_master_port = I2C_NUM_0;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, // Fast Mode 400Kbit/s mcp4725
        // .master.clk_speed = 3400000, // High Speed Mode 3.4Mbit/s mcp4725
    };
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

void app_main(void)
{

    uint8_t data[2];
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C Initialized succesfully");
}

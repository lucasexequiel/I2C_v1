#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_SLAVE_ADDR 0x62 // 0xC1
#define TIMEOUT_MS 1000
#define DELAY_MS 100
#define ACK_CHECK_EN 0

static void i2c_master_init()
{

    // i2c_port_t i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 22,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, // Fast Mode 400Kbit/s mcp4725 // High Speed Mode 3.4Mbit/s mcp4725
    };

    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    return;
}


static void i2c_valor(){

    uint8_t rx[2];
    rx[1] = 0b11111111;
    rx[0] = 0b00000111;

    while(1){
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (I2C_SLAVE_ADDR << 1) | I2C_MASTER_WRITE, 0);
    i2c_master_write_byte(cmd, rx[0], ACK_CHECK_EN);
    i2c_master_write_byte(cmd, rx[1], ACK_CHECK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    printf("Se envio de valor I2C con valor 3.3V\n\n");
    vTaskDelay(100);
    return;
    }
}

void app_main(void)
{
    // Seteo de GPIO
    i2c_master_init();

    for (size_t i = 0; i < 4; i++)
    {
        vTaskDelay(500);
        printf("a la [%d]\n\n", i);
    }

    i2c_valor();

}

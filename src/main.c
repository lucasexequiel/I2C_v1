#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c-example";

#define I2C_SLAVE_ADDR 0x60//0xC1
#define TIMEOUT_MS 1000
#define DELAY_MS 100
#define ACK_CHECK_EN 0x1
#define I2C_MASTER_PORT I2C_NUM_1

static esp_err_t i2c_master_init()
{

    // i2c_port_t i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = GPIO_NUM_22,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, // Fast Mode 400Kbit/s mcp4725
        // .master.clk_speed = 3400000, // High Speed Mode 3.4Mbit/s mcp4725
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_PORT, &conf);
    if (err != ESP_OK)
    {
        return err;
    }
    return i2c_driver_install(I2C_MASTER_PORT, conf.mode, 0, 0, 0);
}

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C Initialized succesfully");

    // uint8_t rx_data [2];
    // rx_data[1]  = 0b00000000;
    // rx_data[0]  = 0b00000000;

    esp_err_t i2c_ret = ESP_OK;

    while (1)
    {
        // Command link created
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        // Start condition
        i2c_master_start(cmd);
        // Address + Write bit 
        /*----------------------SE HACE ESTE CORRIMIENTO HACIA LA IZQUIERDA PORQUE EL TAMAÑO DEL CANAL DE COMUNICACION DEL I2C---------------------------*/
        ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (I2C_SLAVE_ADDR << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN)); // habla al sensor dac
        i2c_master_write_byte(cmd, 0x02, ACK_CHECK_EN);
        i2c_master_write_byte(cmd, 0x7f, ACK_CHECK_EN);

        // ESP_ERROR_CHECK(i2c_master_write_byte(cmd,I2C_SLAVE_ADDR, ACK_CHECK_EN));
        // i2c_master_write(cmd, rx_data, 2, ACK_CHECK_EN);                                    // manda el dato
        
        i2c_master_stop(cmd);
        i2c_ret = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        // i2c_master_read_from_device(I2C_NUM_0, I2C_SLAVE_ADDR, rx_data, 5, TIMEOUT_MS / portTICK_RATE_MS);
        // Similar a la API de envío de datos, debe especificar el número de puerto I2C, la dirección del esclavo, el búfer de recepción y el valor de tiempo de espera.
        // description tag, buffer Pointer to the buffer array y tamaño de buffer en bytes.
        // ESP_LOG_BUFFER_HEX(TAG, rx_data, 2);

        

        if (i2c_ret == ESP_OK)
        {
            // READ BYTES
            printf("I2C Write OK\n");
        }
        else
        {
            printf("I2C FAILED\n");
        }

        }
}

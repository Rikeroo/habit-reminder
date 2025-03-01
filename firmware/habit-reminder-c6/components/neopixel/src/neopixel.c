/* Blink Example
*/
#include "neopixel.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "neopixel";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 21 /* D3 on Xiao Esp32-c6*/
#define SPI_BUS SPI2_HOST

neopixel_handle_t neopixel_init(uint16_t num_leds)
{
    neopixel_handle_t handle = {
        .num_leds = num_leds,
        .colors = heap_caps_calloc(num_leds, sizeof(neopixel_color_t), MALLOC_CAP_DEFAULT)
    };

    if (!handle.colors) {
        ESP_LOGE(TAG, "Failed to allocate memory for LED colors");
        abort();
    }

    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = num_leds,
    };

    led_strip_spi_config_t spi_config = {
        .spi_bus = SPI_BUS,
        .flags.with_dma = true,
    };

    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &handle.strip_handle));
    neopixel_clear_all(&handle);

    return handle;
}

void neopixel_deinit(neopixel_handle_t *handle)
{
    if (handle && handle->colors) {
        free(handle->colors);
        handle->colors = NULL;
    }
}

esp_err_t neopixel_set_led(neopixel_handle_t *handle, uint16_t index,
                      uint8_t r, uint8_t g, uint8_t b)
{
    if (!handle || index >= handle->num_leds) {
        ESP_LOGE(TAG, "Invalid LED index: %d", index);
        return ESP_ERR_INVALID_ARG;
    }

    handle->colors[index] = (neopixel_color_t){r,g,b};
    led_strip_set_pixel(handle->strip_handle, index, r,g,b);
    return ESP_OK;
}

void neopixel_set_all(neopixel_handle_t *handle, uint8_t r, uint8_t g, uint8_t b)
{
    for (uint16_t i = 0; i < handle->num_leds; i++) {
        handle->colors[i] = (neopixel_color_t){r,g,b};
        led_strip_set_pixel(handle->strip_handle, i, r,g,b);
    }
}

void neopixel_refresh(neopixel_handle_t *handle)
{
    led_strip_refresh(handle->strip_handle);
}

esp_err_t neopixel_clear_led(neopixel_handle_t *handle, uint16_t index)
{
    return neopixel_set_led(handle, index, 0, 0, 0);
}

void neopixel_clear_all(neopixel_handle_t *handle)
{
    neopixel_set_all(handle, 0, 0, 0);
    led_strip_clear(handle->strip_handle);
}

esp_err_t neopixel_get_color(neopixel_handle_t *handle, uint16_t index,
                             neopixel_color_t *color)
{
    if (!handle || index >= handle->num_leds || !color) {
        ESP_LOGE(TAG, "Invalid arguments");
        return ESP_ERR_INVALID_ARG;
    }

    *color = handle->colors[index];
    return ESP_OK;
}
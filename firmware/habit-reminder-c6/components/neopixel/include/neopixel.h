#pragma once
#include <stdbool.h>
#include "led_strip.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLINK_GPIO 21 /* D3 on Xiao Esp32-c6*/
#define SPI_BUS SPI2_HOST

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} neopixel_color_t;

typedef struct {
    led_strip_handle_t strip_handle;
    neopixel_color_t *colors; /* Array of individual LED colors*/
    uint16_t num_leds;
} neopixel_handle_t;

/**
 * @brief Initialize NeoPixel LED strip
 * @param num_leds Number of LEDs in the strip
 * @return Handle to NeoPixel resources
 */
neopixel_handle_t neopixel_init(uint16_t num_leds);

/**
 * @brief Deinitialise NeoPixel LED strip
 * @param handle Initialise NeoPixel handle
 */
void neopixel_deinit(neopixel_handle_t *handle);

/**
 * @brief Set Individual LED color
 * @param handle Initialized NeoPixel handle
 * @param index  Index of NeoPixel LED to set
 * @param r Red value (0-255)
 * @param g Green value (0-255)
 * @param b Blue value (0-255)
 */
esp_err_t neopixel_set_led(neopixel_handle_t *handle, uint16_t index, 
                        uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Set all LED colors
 * @param handle Initialized NeoPixel handle
 * @param r Red value (0-255)
 * @param g Green value (0-255)
 * @param b Blue value (0-255)
 */
void neopixel_set_all(neopixel_handle_t *handle, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Update NeoPixels with current color data
 * @param index Index of NeoPixel LED to refresh
 */
void neopixel_refresh(neopixel_handle_t *handle);

/**
 * @brief Clear all LEDs
 * @param handle Initialized NeoPixel handle
 */
void neopixel_clear_all(neopixel_handle_t *handle);

#ifdef __cplusplus
}
#endif
#include "oled_display.h"
#include "neopixel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "main";

void app_main(void)
{
    esp_log_level_set("neopixel", ESP_LOG_DEBUG);
    vTaskDelay(pdMS_TO_TICKS(100));
    /* Initialise OLED with LVGL demo */
    // oled_display_handle_t oled = oled_display_init();
    // oled_display_show_demo(&oled);

    /* Initialise NeoPixel strip with 3 LEDs */
    neopixel_handle_t leds = neopixel_init(3);
    neopixel_clear_all(&leds);
    neopixel_refresh(&leds);

    // uint8_t active_led = 0;
    neopixel_set_led(&leds, 0, 0, 0, 0);
    neopixel_set_led(&leds, 1, 0, 50, 0);
    neopixel_set_led(&leds, 2, 0, 0, 50);
    vTaskDelay(pdMS_TO_TICKS(100));

    neopixel_refresh(&leds);

    while (1) {
        /* Clear all LEDs */
        // neopixel_clear_all(&leds);
        // ESP_LOGI(TAG, "Current Active LED: %d", active_led);
        
        /* Light up current LED */
        // neopixel_set_led(&leds, 1, 0, 50, 0); /* Green */
        // neopixel_refresh(&leds);
        // active_led = (active_led + 1) % 3;

        // ESP_LOGI(TAG, "Data line state: %d", gpio_get_level(BLINK_GPIO));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
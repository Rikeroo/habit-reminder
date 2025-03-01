#include "oled_display.h"
#include "neopixel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    /* Initialise OLED with LVGL demo */
    oled_display_handle_t oled = oled_display_init();
    oled_display_show_demo(&oled);

    /* Initialise NeoPixel strip with 3 LEDs */
    neopixel_handle_t leds = neopixel_init(3);
    uint8_t active_led = 0;

    while (1) {
        /* Clear all LEDs */
        neopixel_clear_all(&leds);
        
        /* Light up current LED */
        neopixel_set_led(&leds, active_led, 0, 50, 0); /* Green */
        neopixel_refresh(&leds);
        active_led = (active_led + 1) % 3;

        /* Update rate (500ms per LED)*/
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
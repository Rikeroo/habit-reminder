#include "unity.h"
#include "neopixel.h"
#include "esp_log.h"
#include "mock_led_strip.h" // Generated mock for hardware interface

TEST_CASE("neopixel_init_valid", "[neopixel]")
{
    neopixel_handle_t handle = neopixel_init(3);
    TEST_ASSERT_NOT_NULL(handle.colors);
    TEST_ASSERT_EQUAL(3, handle.num_leds);
    neopixel_deinit(&handle);
}

TEST_CASE("neopixel_set_led_color", "[neopixel]")
{
    neopixel_handle_t handle = neopixel_init(3);

    // Test color setting
    TEST_ASSERT_EQUAL(ESP_OK, neopixel_set_led(&handle, 1, 255, 0, 0));

    // Verify internal state
    neopixel_color_t color;
    neopixel_get_color(&handle, 1, &color);
    TEST_ASSERT_EQUAL(255, color.r);
    TEST_ASSERT_EQUAL(0, color.g);
    TEST_ASSERT_EQUAL(0, color.b);

    neopixel_deinit(&handle);
}
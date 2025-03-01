idf_component_register(
    SRCS "src/neopixel.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        led_strip
        driver
        esp_system
)
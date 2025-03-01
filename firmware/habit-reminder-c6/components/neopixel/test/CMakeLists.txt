idf_component_register(SRCS "neopixel_test.c"
                    INCLUDE_DIRS "../../include"
                    REQUIRES 
                        unity
                        led_strip
                        esp_rom
                    PRIV_REQUIRES 
                        cmock
)
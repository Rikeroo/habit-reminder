#pragma once
#include <stdbool.h>
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    esp_lcd_panel_handle_t panel_handle;
    esp_lcd_panel_io_handle_t io_handle;
    lv_disp_t *lvgl_disp;
} oled_display_handle_t;

/**
 * @brief Initialize OLED display and LVGL
 * @return Handle to display resources
 */
oled_display_handle_t oled_display_init(void);

/**
 * @brief Run UI demo
 * @param handle Initialized display handle
 */
void oled_display_show_demo(const oled_display_handle_t *handle);

#ifdef __cplusplus
}
#endif
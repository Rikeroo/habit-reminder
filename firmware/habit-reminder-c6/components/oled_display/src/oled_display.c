#include "oled_display.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/lock.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_ssd1306.h"
#include "esp_lvgl_port.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "lvgl.h"
#include "test_lvgl_ui.h"

static const char *TAG = "oled_display";

#define I2C_BUS_PORT            0

#define LCD_PIXEL_CLOCK_HZ      (400 * 1000)
#define PIN_NUM_SDA             22
#define PIN_NUM_SCL             23
#define PIN_NUM_RST             -1
#define I2C_HW_ADDR             0x3C

#define LCD_H_RES               128
#define LCD_V_RES               64

#define LCD_CMD_BITS            8
#define LCD_PARAM_BITS          8

extern void lvgl_ui_demo(lv_disp_t *disp);

void init_hardware(esp_lcd_panel_handle_t *panel, 
                  esp_lcd_panel_io_handle_t *io)
{
    ESP_LOGI(TAG, "Initialise I2c bus");
    i2c_master_bus_handle_t i2c_bus = NULL;
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = PIN_NUM_SDA,
        .scl_io_num = PIN_NUM_SCL,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = I2C_HW_ADDR,
        .scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_CMD_BITS,
        .dc_bit_offset = 6,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = PIN_NUM_RST,
    };

    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    /* Update pointer values */
    *panel  = panel_handle;
    *io     = io_handle;
}

oled_display_handle_t oled_display_init(void)
{
    oled_display_handle_t handle = {0};

    /* Initialise hardware */
    init_hardware(&handle.panel_handle, &handle.io_handle);

    /* Initialies LVGL */
    ESP_LOGI(TAG,  "Initialise LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = handle.io_handle,
        .panel_handle = handle.panel_handle,
        .buffer_size = LCD_H_RES * LCD_V_RES,
        .double_buffer = true,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };

    // create a lvgl display
    handle.lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    // Set screen rotation
    lv_disp_set_rotation(handle.lvgl_disp, LV_DISP_ROT_NONE);

    return handle;
}

void oled_display_show_demo(const oled_display_handle_t *handle)
{
    ESP_LOGI(TAG, "Display LVGL Scroll Text");
    // lock the mutex due to the LVGL APIs not being thread-safe
    if (lvgl_port_lock(0)) {
        lvgl_ui_demo(handle->lvgl_disp);
        // Release the mutex
        lvgl_port_unlock();
    }
}
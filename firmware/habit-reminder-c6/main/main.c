#include "oled_display.h"
#include "test_lvgl_ui.h"
#include "neopixel.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"

/* Queue and state management */
QueueHandle_t task_queue;
bool task_states[3] = {false};

static const char *TAG = "main";

/* Button handling task */
void button_task(void *pvParam)
{
     const gpio_num_t button_pins[] = {GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2};
     bool last_states[3] = {false};

     for (int i=0; i<3; i++) {
        gpio_reset_pin(button_pins[i]);
        gpio_set_direction(button_pins[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(button_pins[i], GPIO_PULLUP_ONLY);
     }

     while(1){
        for(int i = 0; i<3; i++) {
            bool current = !gpio_get_level(button_pins[i]); // Active low
            if (current && !last_states[i]) {
                task_states[i] = !task_states[i];
                xQueueSend(task_queue, &i, portMAX_DELAY);
            }
            last_states[i] = current;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
     }
}

void app_main(void)
{
    /* Initialise hardware */
    oled_display_handle_t oled = oled_display_init();
    neopixel_handle_t leds = neopixel_init(3);

    /* Initial neopixel LEDs are red */
    neopixel_set_all(&leds, 50, 0, 0);
    neopixel_refresh(&leds);

    /* Create task queue */
    task_queue = xQueueCreate(5, sizeof(int));

    /* Initialise UI */
    lvgl_ui_demo(oled.lvgl_disp);

    /* Create tasks */
    xTaskCreate(button_task, "buttons", 2048, NULL, 2, NULL);

    /* Main loop */
    int task_index;
    while(1) {
        if(xQueueReceive(task_queue, &task_index, portMAX_DELAY)) {
            /* Update LED */
            neopixel_set_led(&leds, task_index,
                            task_states[task_index] ? 0 : 50, /* Green if complete */
                            task_states[task_index] ? 50 : 0, /* Red if not complete */
                            0);

            /* Update checkbox */
            update_checkbox_state(task_index, task_states[task_index]);
            neopixel_refresh(&leds);

            ESP_LOGI(TAG, "Task %d %s", task_index+1,
                    task_states[task_index] ? "completed" : "pending");
        }
        lv_task_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
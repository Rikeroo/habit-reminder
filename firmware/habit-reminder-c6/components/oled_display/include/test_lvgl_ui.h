#pragma once
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void lvgl_ui_demo(lv_disp_t *disp);
void update_checkbox_state(uint8_t task_index, bool completed);

#ifdef __cplusplus
}
#endif
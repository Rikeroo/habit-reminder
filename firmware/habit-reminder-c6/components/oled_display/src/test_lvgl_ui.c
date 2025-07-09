#include "test_lvgl_ui.h"
#include "lvgl.h"

#define TASK_SPACING 16 /* in pixels */

static lv_obj_t *checkboxes[3];
static lv_style_t strike_style;

void lvgl_ui_demo(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);

    /* Create a flex container for the checkboxes */
    lv_obj_t *cont = lv_obj_create(scr);
    lv_obj_set_size(cont, LV_PCT(100), LV_SIZE_CONTENT); // Full width, auto height
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);      // Vertical stacking
    lv_obj_set_style_pad_row(cont, 2, 0);                // 8px vertical spacing between children
    lv_obj_set_style_pad_all(cont, 0, 0);                // No outer padding

    /* Create strike-through style */
    lv_style_init(&strike_style);
    lv_style_set_text_decor(&strike_style, LV_TEXT_DECOR_STRIKETHROUGH);

    /* Create checkboxes with labels */
    const char *tasks[] = {"Dutch", "Vitamins", "Journal"};
    for (int i = 0; i < 3; i++) {
        checkboxes[i] = lv_checkbox_create(cont); // Parent is the container
        lv_checkbox_set_text(checkboxes[i], tasks[i]);
        lv_obj_add_flag(checkboxes[i], LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_set_style_text_font(checkboxes[i], &lv_font_montserrat_12, 0);
    }
}

/* Callback to update checkbox state */
void update_checkbox_state(uint8_t task_index, bool completed)
{
    if(completed) {
        lv_obj_add_style(checkboxes[task_index], &strike_style, LV_PART_MAIN);
    } else {
        lv_obj_remove_style(checkboxes[task_index], &strike_style, LV_PART_MAIN);
    }
}
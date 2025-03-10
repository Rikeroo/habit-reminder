#include "test_lvgl_ui.h"
#include "lvgl.h"

static lv_obj_t *checkboxes[3];
static lv_style_t strike_style;

void lvgl_ui_demo(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);

    /* Create strike-through style */
    lv_style_init(&strike_style);
    lv_style_set_text_decor(&strike_style, LV_TEXT_DECOR_STRIKETHROUGH);
    lv_style_set_text_color(&strike_style, lv_color_hex(0x808080));

    /* Create checkboxes with labels */
    const char *tasks[] = {"Task 1", "Task 2", "Task 3"};
    for (int i=0; i <3; i++) {
        checkboxes[i] = lv_checkbox_create(scr);
        lv_checkbox_set_text(checkboxes[i], tasks[i]);
        lv_obj_align(checkboxes[i], LV_ALIGN_TOP_LEFT, 10, 20 + (i *40));

        /* Initial Style */
        lv_obj_add_flag(checkboxes[i], LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_set_style_text_font(checkboxes[i], &lv_font_montserrat_14, 0);
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
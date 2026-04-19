#include <Elementary.h>
#include "logic.h"
#include <libintl.h>

#define _(String) gettext(String)

static Evas_Object *start_spinner;
static Evas_Object *end_spinner;

static void
on_toggle_changed(void *data, Evas_Object *obj, void *event_info)
{
    int val = elm_check_state_get(obj);
    logic_set_enabled(val);
}

static void
on_slider_changed(void *data, Evas_Object *obj, void *event_info)
{
    int val = (int)elm_slider_value_get(obj);
    logic_set_temperature(val);
}

static void
on_schedule_changed(void *data, Evas_Object *obj, void *event_info)
{
    int start = (int)elm_spinner_value_get(start_spinner);
    int end   = (int)elm_spinner_value_get(end_spinner);

    logic_set_schedule(start, end);
}

static void
on_save_clicked(void *data, Evas_Object *obj, void *event_info)
{
    logic_save();
}

void ui_init(void)
{
    Evas_Object *win = elm_win_util_standard_add("nl-ease", "nl-ease");
    elm_win_autodel_set(win, EINA_TRUE);

    evas_object_resize(win, 300, 240);
    evas_object_size_hint_min_set(win, 300, 240);
    evas_object_size_hint_max_set(win, 300, 240);
    
    elm_win_size_base_set(win, 300, 240);
	elm_win_size_step_set(win, 0, 0);

    Evas_Object *box = elm_box_add(win);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, box);
    elm_win_conformant_set(win, EINA_FALSE);
    elm_box_padding_set(box, 0, 4);
    evas_object_show(box);

    // Toggle
    Evas_Object *toggle = elm_check_add(win);
    elm_object_text_set(toggle, _("Enabled"));
    evas_object_smart_callback_add(toggle, "changed", on_toggle_changed, NULL);
    elm_box_pack_end(box, toggle);
    evas_object_show(toggle);

    // Slider
    Evas_Object *slider = elm_slider_add(win);
    elm_slider_min_max_set(slider, 2500, 6500);
    elm_slider_value_set(slider, 4500);
    elm_object_text_set(slider, _("Temperature"));
    evas_object_smart_callback_add(slider, "changed", on_slider_changed, NULL);
    elm_box_pack_end(box, slider);
    evas_object_show(slider);

	// Start label
	Evas_Object *start_label = elm_label_add(win);
	elm_object_text_set(start_label, _("Start time:"));
	elm_box_pack_end(box, start_label);
	evas_object_show(start_label);

	// Start spinner
	start_spinner = elm_spinner_add(win);
	elm_spinner_min_max_set(start_spinner, 0, 23);
	elm_spinner_value_set(start_spinner, 22);
	evas_object_smart_callback_add(start_spinner, "changed", on_schedule_changed, NULL);
	elm_box_pack_end(box, start_spinner);
	evas_object_show(start_spinner);

	// End label
	Evas_Object *end_label = elm_label_add(win);
	elm_object_text_set(end_label, _("End time:"));
	elm_box_pack_end(box, end_label);
	evas_object_show(end_label);

	// End spinner
	end_spinner = elm_spinner_add(win);
	elm_spinner_min_max_set(end_spinner, 0, 23);
	elm_spinner_value_set(end_spinner, 6);
	evas_object_smart_callback_add(end_spinner, "changed", on_schedule_changed, NULL);
	elm_box_pack_end(box, end_spinner);
	evas_object_show(end_spinner);

    // Save button
    Evas_Object *btn = elm_button_add(win);
    elm_object_text_set(btn, _("Save"));
    evas_object_smart_callback_add(btn, "clicked", on_save_clicked, NULL);
    elm_box_pack_end(box, btn);
    evas_object_show(btn);
	
	// UI sync
	AppState *s = logic_get_state();

	elm_check_state_set(toggle, s->enabled);
	elm_slider_value_set(slider, s->temperature);
	elm_spinner_value_set(start_spinner, s->start_hour);
	elm_spinner_value_set(end_spinner, s->end_hour);
	
    evas_object_show(win);
}

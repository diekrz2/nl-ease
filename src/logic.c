#include <stdlib.h>
#include <stdio.h>
#include <Ecore.h>
#include <time.h>
#include <sys/stat.h>

#include "logic.h"
#include "xrandr.h"

static AppState state;

static Eina_Bool timer_cb(void *data);

static int is_in_schedule(void)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int now = tm->tm_hour;

    if (state.start_hour < state.end_hour) {
        return (now >= state.start_hour && now < state.end_hour);
    } else {
        return (now >= state.start_hour || now < state.end_hour);
    }
}

void logic_apply(void)
{
    if (state.enabled && is_in_schedule()) {
        xrandr_set_temperature(state.temperature);
    } else {
        xrandr_reset();
    }
}

void logic_init(void)
{
    state.enabled = 0;
    state.temperature = 4500;
    state.start_hour = 22;
    state.end_hour = 6;

	logic_load();
   // ecore_timer_add(60.0, timer_cb, NULL);
    logic_apply();
}

AppState *logic_get_state(void)
{
    return &state;
}

void logic_set_enabled(int enabled)
{
    state.enabled = enabled;
    logic_apply();
    logic_save();
}

void logic_set_temperature(int temp)
{
    state.temperature = temp;
    logic_apply();
    logic_save();
}

void logic_set_schedule(int start_hour, int end_hour)
{
    state.start_hour = start_hour;
    state.end_hour = end_hour;
    logic_apply();
    logic_save();
}

static Eina_Bool timer_cb(void *data)
{
    logic_apply();
    return ECORE_CALLBACK_RENEW;
}

// CONFIG

static const char *get_config_path(void)
{
    static char path[256];
    const char *home = getenv("HOME");

    if (!home) home = "/tmp";

    snprintf(path, sizeof(path),
             "%s/.config/nl-ease.conf", home);

    return path;
}

void logic_save(void)
{
    const char *home = getenv("HOME");
    if (!home) return;

    char dir[256];
    snprintf(dir, sizeof(dir), "%s/.config", home);
    mkdir(dir, 0755);

    FILE *f = fopen(get_config_path(), "w");
    if (!f) return;

    fprintf(f, "enabled=%d\n", state.enabled);
    fprintf(f, "temperature=%d\n", state.temperature);
    fprintf(f, "start=%d\n", state.start_hour);
    fprintf(f, "end=%d\n", state.end_hour);

    fclose(f);
}

void logic_load(void)
{
    FILE *f = fopen(get_config_path(), "r");
    if (!f) return;

    fscanf(f, "enabled=%d\n", &state.enabled);
    fscanf(f, "temperature=%d\n", &state.temperature);
    fscanf(f, "start=%d\n", &state.start_hour);
    fscanf(f, "end=%d\n", &state.end_hour);

    fclose(f);
}

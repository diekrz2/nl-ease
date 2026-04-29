#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/wait.h>

static char output_name[64] = {0};

static void detect_output(void)
{
    FILE *fp = popen("xrandr | grep ' connected'", "r");
    if (!fp) return;

    if (fgets(output_name, sizeof(output_name), fp)) {
        char *space = strchr(output_name, ' ');
        if (space) *space = '\0';
    }
    pclose(fp);
}

static int run_command(const char *cmd)
{
    if (!cmd) return -1;

    int status = system(cmd);
    if (status == -1) {
        perror("system() failed");
        return -1;
    }

    if (WIFEXITED(status)) {
        int ret = WEXITSTATUS(status);
        if (ret != 0) {
            fprintf(stderr, "Command exited with code %d: %s\n", ret, cmd);
        }
    }
    return status;
}

void xrandr_set_temperature(int temp)
{
    setlocale(LC_NUMERIC, "C");

    if (output_name[0] == '\0') {
        detect_output();
    }

    if (output_name[0] == '\0') {
        printf("No output detected\n");
        return;
    }

    char cmd[256];
    float ratio = (temp - 1000.0f) / (6500.0f - 1000.0f);

    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;

    float r = 1.0f;
    float g = 0.5f + 0.5f * ratio;
    float b = 0.2f + 0.8f * ratio * ratio;

    snprintf(cmd, sizeof(cmd),
             "xrandr --output %s --gamma %.2f:%.2f:%.2f",
             output_name, r, g, b);

    printf("CMD: %s\n", cmd);
    run_command(cmd);
}

void xrandr_reset(void)
{
    output_name[0] = '\0';     // <- force redetect here
    detect_output();

    if (output_name[0] == '\0') {
        fprintf(stderr, "xrandr_reset: No output detected\n");
        return;
    }

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "xrandr --output %s --gamma 1:1:1", output_name);

    printf("CMD RESET: %s\n", cmd);
    run_command(cmd);
}

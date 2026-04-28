#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

static char output_name[64] = {0};

static void detect_output(void)
{
    FILE *fp = popen("xrandr | grep ' connected'", "r");
    if (!fp) return;

    if (fgets(output_name, sizeof(output_name), fp)) {
        // takes the first token
        char *space = strchr(output_name, ' ');
        if (space) *space = '\0';
    }

    pclose(fp);
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

	float ratio = (temp - 1000.0) / (6500.0 - 1000.0);

	// clamp 
	if (ratio < 0) ratio = 0;
	if (ratio > 1) ratio = 1;

	// natural curve
	float r = 1.0;
	float g = 0.5 + 0.5 * ratio;        // 0.5 -> 1.0
	float b = 0.2 + 0.8 * ratio * ratio; // 0.2 -> 1.0 

    snprintf(cmd, sizeof(cmd),
        "xrandr --output %s --gamma %.2f:%.2f:%.2f",
        output_name, r, g, b);

    printf("CMD: %s\n", cmd);
    system(cmd);
}

void xrandr_reset(void)
{
	// force detect
    output_name[0] = '\0';        
    detect_output();

    if (output_name[0] == '\0') {
        fprintf(stderr, "xrandr_reset: No output detected\n");
        return;
    }

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "xrandr --output %s --gamma 1:1:1", output_name);

    printf("CMD RESET: %s\n", cmd);
    system(cmd);
}

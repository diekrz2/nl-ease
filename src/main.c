#include <Elementary.h>
#include "ui.h"
#include "logic.h"
#include <locale.h>
#include <libintl.h>

#define _(String) gettext(String)

int main(int argc, char **argv)
{
    elm_init(argc, argv);

    logic_init();
    setlocale(LC_ALL, "");
	bindtextdomain("nl-ease", "/usr/share/locale");
	textdomain("nl-ease");
    ui_init();

    elm_run();
    elm_shutdown();

    return 0;
}

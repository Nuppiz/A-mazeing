#include "Shared.h"
#include "Menus.h"

extern struct GameData g;
extern struct Cursor cursor;
extern struct Options opt;

void selections_main(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    if (cursor->selection == 0)
        start_game(g, opt);
    else if (cursor->selection == 1)
    {
        opt->menu_status = MENU_OPTIONS;
        change_menu(opt, cursor);
        cursor->new_y = 65;
        cursor->selection = 0;
    }
    else if (cursor->selection == 2)
    {
        opt->menu_status = MENU_HELP;
        change_menu(opt, cursor);
        draw_help_contents(g);
    }
    else if (cursor->selection == 3)
    {
        opt->menu_status = MENU_STORY;
        change_menu(opt, cursor);
    }
    else if (cursor->selection == 4)
        g->game_running = 0;
}

void selections_options(struct Cursor* cursor, struct Options* opt)
{
    if (cursor->selection == 0)
    {
        if (opt->sfx_on == 1)
            opt->sfx_on = 0;
        else
            opt->sfx_on = 1;
        change_menu(opt, cursor); // optimize later
        render_menu_text(cursor, opt);
    }
    else if (cursor->selection == 1)
    {
        if (opt->music_on == 1)
            opt->music_on = 0;
        else
            opt->music_on = 1;
        change_menu(opt, cursor); // optimize later
        render_menu_text(cursor, opt);
    }
    else if (cursor->selection == 2)
        opt->menu_status = MENU_KEYCONF;
        change_menu(opt, cursor); // optimize later
}

void menu_controller(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    if (opt->menu_status == MENU_MAIN)
    {
        selections_main(g, cursor, opt);
    }
    else if (opt->menu_status == MENU_OPTIONS)
        selections_options(cursor, opt);
}
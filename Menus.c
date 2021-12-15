#include "Shared.h"
#include "Menus.h"

extern struct GameData g;
extern struct Cursor cursor;
extern struct Options opt;

void selections_main(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    if (cursor->selection == 0)
        start_game(g);
    else if (cursor->selection == 1)
    {
        opt->menu_status = MENU_OPTIONS;
        change_menubg();
        cursor->new_y = 65;
        cursor->selection = 0;
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
    }
    else if (cursor->selection == 1)
    {
        if (opt->music_on == 1)
            opt->music_on = 0;
        else
            opt->music_on = 1;
    }
    else if (cursor->selection == 2)
        opt->menu_status = MENU_KEYCONF;
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
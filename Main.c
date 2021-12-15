#include "Shared.h"
#include "Gameplay.h"
#include "Gfx.h"
#include "Keyb.h"
#include "LvlLoad.h"
#include "Menus.h"
#include "Sounds.h"

void start_game(struct GameData* g, struct Options* opt)
{
    start_screen(opt);
    delay(2000);
    
    fill_screen(0);
    g->game_state = GAME_INGAME;
    g->level_num = 1;
    g->player_lives = 3;
    level_loader(g);
}

void init_gamedata(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    /* init game data */
    memset(g, 0, sizeof(struct GameData));

    /* init cursor data */
    cursor->old_x = 115;
    cursor->new_x = 115;
    cursor->old_y = 65;
    cursor->new_y = 65;
    cursor->selection = 0;

    /* init options */
    opt->menu_status = MENU_MAIN;
    opt->debugmode = 0;
    opt->sfx_on = 1;
    opt->music_on = 1;
}

void init(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{       
    /* init gfx */
    set_mode(VGA_256_COLOR_MODE);
    load_tiles();
    load_special_gfx();
    set_tile_gfx();
    create_composites();

    /* init gamedata */
    init_gamedata(g, cursor, opt);
    g->game_running = 1;
    g->game_state = GAME_MENU;
    
    /* init keyboard */
    init_keyboard();
    init_default_keys(opt);
}

void quit()
{
    deinit_keyboard();
    set_mode(TEXT_MODE);
}

void main()
{
    /* game options and menu "status" struct */
    struct Options opt;

    /* local variables all inside GameData struct called g */
    struct GameData g;

    /* cursor location inside a struct so it can easily be accessed everywhere */
    struct Cursor cursor;
    
    /* initialize everything */
    init(&g, &cursor, &opt);

    /* run game, get keyboard state first */
    while (g.game_running == 1)
    {
        process_input(&g, &cursor, &opt);
        game_logic(&g, &opt);
        render(&g, &cursor, &opt);
        if (opt.music_on == 1 && g.game_state != GAME_MENU)
        {
            play_song();
            delay(75);
        }
        else
            delay(100);
    }
    
    /* quit */
    quit();
}

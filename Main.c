#include "Shared.h"
#include "Gameplay.h"
#include "Gfx.h"
#include "Keyb.h"
#include "LvlLoad.h"
#include "Menus.h"
#include "Sounds.h"

uint16_t *clock=(uint16_t *)0x0000046C;    /* this points to the 18.2hz system clock. */
int vrets = 0;
int vretlen = 0;
float t1;

void init_gamedata(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    /* init game data */
    memset(g, 0, sizeof(struct GameData));

    /* init cursor data */
    cursor->x = 105;
    cursor->old_y = 65;
    cursor->new_y = 65;
    cursor->selection = 0;
    cursor->old_selection = 0;

    /* init options */
    opt->menu_status = MENU_MAIN;
    opt->debugmode = 0;
    opt->sfx_on = 1;
    opt->music_on = 1;
}

void wait_for_retrace()
{
    /* wait until done with vertical retrace */
    while  ((inp(INPUT_STATUS) & VRETRACE));
    /* wait until done refreshing */
    while (!(inp(INPUT_STATUS) & VRETRACE));
}

void init_clock()
{
    /* calculates the time (in milliseconds) for one vertical retrace */
    uint16_t start = *clock;

    while (t1 < 1.0)
    {
        wait_for_retrace();
        vrets++;
        t1=(*clock-start)/18.2;
    }
    vretlen = t1 / vrets * 1000;
}

void init(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    /* init gfx */
    set_mode(VGA_256_COLOR_MODE);
    printf("Initialising graphics...");
    load_tiles();
    load_special_gfx();
    set_tile_gfx();
    create_composites();

    printf("OK!\n");

    printf("Initialising game data...");

    /* init gamedata */
    init_gamedata(g, cursor, opt);
    g->game_running = 1;
    g->game_state = GAME_MENU;
    change_menu(opt, cursor);

    printf("OK!\n");
    
    printf("Initialising keyboard...");

    /* init keyboard */
    init_keyboard();
    init_default_keys(opt);

    printf("OK!\n");

    printf("Synchronising game clock...");

    /* initialize game clock */
    init_clock();

    printf("OK!\n");
    delay(200);
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
        game_logic(&g, &cursor, &opt);
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

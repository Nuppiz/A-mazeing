#include "Shared.h"
#include "Gameplay.h"
#include "Gfx.h"
#include "Keyb.h"
#include "LvlLoad.h"
#include "Menus.h"
#include "Sounds.h"

uint16_t *clock=(uint16_t *)0x0000046C;    /* this points to the 18.2hz system clock. */
uint32_t vretlen = 0;
uint32_t secdiv = 0;
uint32_t timer = 0;

/* game options and menu "status" struct */
struct Settings opt;
/* local variables all inside GameData struct called g */
struct GameData g;

extern uint16_t song_i;

void init_gamedata()
{
    /* init game data */
    memset(g, 0, sizeof(struct GameData));

    /* init options */
    opt.debugmode = FALSE;
    opt.sfx_on = TRUE;
    opt.music_on = TRUE;

    song_i = 0;
}

void vretrace_test()
{
    /* wait until done with vertical retrace */
    while  ((inp(INPUT_STATUS) & VRETRACE));
    /* wait until done refreshing */
    while (!(inp(INPUT_STATUS) & VRETRACE));
}

void vretrace()
{
    timer += vretlen;

    /* wait until done with vertical retrace */
    while  ((inp(INPUT_STATUS) & VRETRACE));
    /* wait until done refreshing */
    while (!(inp(INPUT_STATUS) & VRETRACE));
}

void init_clock()
{
    /* calculates the time (in milliseconds) for one vertical retrace */
    uint16_t start = *clock;
    float vrets = 0;
    float t1;

    while (t1 < 1.0)
    {
        vretrace_test();
        vrets++;
        t1=(*clock-start)/18.2;
    }
    vretlen = t1 / vrets * 1000;
    secdiv = vrets + 50;
}

void init()
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
    menu_main();
    init_gamedata();
    g.game_running = TRUE;
    g.game_state = GAME_MENU;
    change_menu();

    printf("OK!\n");
    
    printf("Initialising keyboard...");

    /* init keyboard */
    init_keyboard();
    init_default_keys();

    printf("OK!\n");

    printf("Synchronising game clock...");

    /* initialize game clock */
    init_clock();

    printf("OK!\n");
    delay(200);

    init_music();
}

void quit()
{
    close_speaker();
    deinit_keyboard();
    set_mode(TEXT_MODE);
}

void main()
{
    uint32_t last_logic = 0;
    uint32_t last_video = 0;
    
    /* initialize everything */
    init();

    /* run game, get keyboard state first */
    while (g.game_running == TRUE)
    {

        if (last_logic + LOGIC_INTERVAL < timer)
        {
            last_logic = timer;
            process_input();
            game_logic();
        }

        if (g.game_state != GAME_MENU)
            play_sequence();

        if (last_video + RENDER_INTERVAL < timer)
        {
            last_video = timer;
            render();
        }
        else
            vretrace();
    }
    
    /* quit */
    quit();
}

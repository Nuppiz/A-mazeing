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
uint32_t last_movement = 0;

/* game options and menu "status" struct */
struct Settings opt;
/* local variables all inside GameData struct called g */
struct GameData g;

System_t System;

extern uint16_t song_i;

static void interrupt (far *old_Timer_ISR)(void);

void interrupt far Timer(void)
{
    static long last_clock_time = 0;

    System.time++;

    // keeps the PC clock ticking in the background
    if (last_clock_time + 182 < System.time)
    {
        last_clock_time = System.time;
        old_Timer_ISR();
    }
}

void setTimer(uint16_t new_count)
{
    outportb(CONTROL_8253, CONTROL_WORD);
    outportb(COUNTER_0, LOW_BYTE(new_count));
    outportb(COUNTER_0, HIGH_BYTE(new_count));
}

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

void initSystem()
{
    System.running    = 1;
    System.time       = 0;
    System.seconds    = 0;
    System.ticks      = 0;
    System.frames     = 0;
    System.tick_interval  = 1000/TICK_RATE;
    System.frame_interval = 1000/FRAME_RATE;
    System.tick_rate  = TICK_RATE;
    System.frame_rate = FRAME_RATE;
    System.fps        = 0;
    System.fps_avg    = 0;
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
    //init_clock();
    old_Timer_ISR = _dos_getvect(TIME_KEEPER_INT);
    _dos_setvect(TIME_KEEPER_INT, Timer);
    setTimer(TIMER_1000HZ);

    printf("OK!\n");
    delay(200);

    init_music();
    initSystem();
}

void quit()
{
    close_speaker();
    setTimer(TIMER_18HZ);
    _dos_setvect(TIME_KEEPER_INT, old_Timer_ISR);
    deinit_keyboard();
    set_mode(TEXT_MODE);
}

void main()
{
    time_t last_time   = 0; // Used for accumulating seconds & FPS calculation
    ticks_t last_tick   = 0; // Tracks time elapsed since last tick started
    time_t last_frame  = 0; // Tracks time elapsed since last draw started
    time_t accumulator = 0; // Incremented by frame draw duration, decremented by ticks
    int frame_count    = 0; // Counts frames in a second so far; used by debug
    
    /* initialize everything */
    init();

    /* run game, get keyboard state first */
    while (g.game_running == 1)
    {  
        if (last_tick + System.tick_interval < System.time) // tick
        {
            do
            {
                last_tick = System.time;

                process_input();
                game_logic();

                accumulator -= System.tick_interval;
                System.ticks++;
            }
            while (accumulator >= System.tick_interval);
        }

        if (last_frame + System.frame_interval < System.time) // frame
        {
            last_frame = System.time;

            render();
            if (g.game_state != GAME_MENU)
                play_sequence();

            System.frames++;
            frame_count++;
            accumulator += System.time - last_frame;
        }

        if (last_time + 1000 < System.time) // FPS calculation; optional for debugging
        {
            last_time += 1000;
            System.seconds++;
            System.fps_avg = (float)System.frames/System.seconds;
            System.fps = frame_count;
            frame_count = 0;
        }
    }
    
    /* quit */
    quit();
}

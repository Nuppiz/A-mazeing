#include "Shared.h"
#include "Gameplay.h"
#include "Gfx.h"
#include "Keyb.h"
#include "Sounds.h"

extern char* lvl;
extern char* keys;
extern char* lives;

// function definitions

void set_cursor(int x, int y)
{
    // Escape code to set the cursor position
    printf("%c[%d;%df", 0x1B, y+1, x+1);
}

void start_game(struct GameData* g)
{
    start_screen();
    delay(2000);
    
    fill_screen(0);
    g->game_state = GAME_INGAME;
    g->debugmode = 0;
    g->level_num = 1;
    g->player_lives = 3;
    level_loader(g);
}

void init_gamedata(struct GameData* g)
{
    /* init game data */
    memset(g, 0, sizeof(struct GameData));
}

void init(struct GameData* g)
{       
    /* init gfx */
    set_mode(VGA_256_COLOR_MODE);
    load_graphics();

    /* init gamedata */
    init_gamedata(g);
    g->level_num = 1;
    g->game_running = 1;
    g->game_state = GAME_MENU;
    
    /* init keyboard */
    init_keyboard();
}

void quit()
{
    deinit_keyboard();
    set_mode(TEXT_MODE);
}

void main()
{
    /* local variables all inside GameData struct called g */
    struct GameData g;
    
    /* initialize everything */
    init(&g);

    /* run game, get keyboard state first */
    while (g.game_running == 1)
    {
        process_input(&g);
        game_logic(&g);
        render(&g);
        //play_audio(&g);
        delay(100);
    }
    
    /* quit */
    quit();
}

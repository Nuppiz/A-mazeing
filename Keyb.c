#include "Keyb.h"
#include "Shared.h"

uint8_t kb_array[128];
uint8_t kb_queue[NUM_SCAN_QUE];
uint8_t kb_queue_head;
uint8_t kb_queue_tail;
unsigned char readch, oldreadch, extended; // needed for keyboard interrupt,
// don't know what for lmao

extern struct GameData g;

void interrupt far keyhandler()
{
    oldreadch = readch;
    asm {
        in al, 0x60
        mov readch, al
        in al, 0x61
        or al, 0x80
        out 0x61, al
        and al, 0x7f
        out 0x61, al
    }
    if (readch == 0xe0)
        extended = 128;
    else
    {
        if (oldreadch != readch)
            kb_queue[(readch&127)+extended] = ((readch>>7)^1);
        extended = 0;
    }
    asm {
        mov al, 0x20
        out 0x20, al
    }
    
    kb_queue[kb_queue_tail++] = readch;
}

void (interrupt far *oldkeyhandler)();

void init_keyboard()
{
    oldkeyhandler = _dos_getvect(0x9);
    _disable(); _dos_setvect(0x9, keyhandler); _enable();
    memset(kb_array, 0, 128);
}

void deinit_keyboard()
{
   _disable(); _dos_setvect(0x9, oldkeyhandler); _enable();
}

void control_menu(struct GameData* g)
{
    if (WAS_HIT(KEY_SPACE))
        start_game(g);
}

void control_end(struct GameData* g)
{
    if (WAS_HIT(KEY_SPACE))
        g->game_state = GAME_MENU;
}

void control_ingame(struct GameData* g)
{    
    static axis = 0;
    struct Actor* player = g->Actors+0; // same as &g->Actors[0]
    
    player->x_vel = 0;
    player->y_vel = 0;
    
    if (WAS_HIT(KEY_UP) || WAS_HIT(KEY_DOWN))
    {
        axis = Y_AXIS;
        player->y_vel =
        (WAS_HIT(KEY_DOWN)>>1) - (WAS_HIT(KEY_UP)>>1);
    }
    else if (WAS_HIT(KEY_LEFT) || WAS_HIT(KEY_RIGHT))
    {
        axis = X_AXIS;
        player->x_vel =
        (WAS_HIT(KEY_RIGHT)>>1) - (WAS_HIT(KEY_LEFT)>>1);
    }
    else if (axis == Y_AXIS)
    {
        player->y_vel =
        IS_PRESSED(KEY_DOWN) - IS_PRESSED(KEY_UP);
        
        if (player->y_vel == 0)
            player->x_vel =
            IS_PRESSED(KEY_RIGHT) - IS_PRESSED(KEY_LEFT);
    }
    else if (axis == X_AXIS)
    {
        player->x_vel =
        IS_PRESSED(KEY_RIGHT) - IS_PRESSED(KEY_LEFT);
        
        if (player->x_vel == 0)
            player->y_vel =
            IS_PRESSED(KEY_DOWN) - IS_PRESSED(KEY_UP);
    }
    
    // clear collision flags
    //g->Actors[0].coll_x = 0;
    //g->Actors[0].coll_y = 0;

    // Misc
    if (WAS_HIT(KEY_Q) && WAS_HIT(KEY_BKSPC) && g->debugmode == 0)
    {
        g->debugmode = 1;
        debug_screen_e();
    }
    
    else if (WAS_HIT(KEY_Q) && WAS_HIT(KEY_BKSPC) && g->debugmode == 1)
    {
        g->debugmode = 0;
        debug_screen_d();
    }
    
    if (g->debugmode == 1)
    {
        if (WAS_HIT(KEY_CHEAT_K))
            add_key(g);
        else if (WAS_HIT(KEY_CHEAT_L))
            add_life(g);
        else if (WAS_HIT(KEY_CHEAT_D))
            remove_life(g);
        else if (WAS_HIT(KEY_CHEAT_S))
            level_skip(g);
    }
}

void get_keyboard()
{
    int key;
    
    while (kb_queue_head != kb_queue_tail)
    {
        key = kb_queue[kb_queue_head];
        kb_queue_head++;
     
        /* key pressed down code (0-127) */
        if (key < 128)
        {
            /* if the key was not pressed down before, set the KEY_HIT bit (2nd bit) on */
            if ( (kb_array[key] & KEY_PRESSED) == 0)
                kb_array[key] |= KEY_HIT;
            
            /* in any case, set the KEY_PRESSED bit (1st bit) on */
            kb_array[key] |= KEY_PRESSED;
        }
        /* key released code (128-255) */
        else
        {
            /* if there was something else than 000000000 for this key, and
             we detect a key release, then set KEY_REL bit (3rd bit, binary value of 4) on */
            if (kb_array[key-128] & KEY_PRESSED)
                kb_array[key-128] |= KEY_REL;
            
            /* in any case, clear the KEY_PRESSED bit (1st bit), off */
            kb_array[key-128] &= (~KEY_PRESSED);
        }
    }
}

void clear_keys()
{
    int i = 0;
    
    while (i < 128)
    {
        kb_array[i] &= (~KEY_HIT);
        kb_array[i] &= (~KEY_REL);
        i++;
    }
}

void process_input(struct GameData* g)
{
    get_keyboard();
    
    if (g->game_state == GAME_MENU)
        control_menu(g);
    else if (g->game_state == GAME_END)
        control_end(g);
    else if (g->game_state == GAME_INGAME)
        control_ingame(g);
    
    // esc always exits, wherever you are
    if (WAS_HIT(KEY_ESC))
        g->game_running = 0;
    
    clear_keys();
}
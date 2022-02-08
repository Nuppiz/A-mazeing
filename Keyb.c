#include "Shared.h"
#include "Keyb.h"
#include "Keys.h"

extern struct GameData g;
extern struct Settings opt;

extern uint8_t minutes;
extern uint16_t seconds;
extern uint16_t timer;
extern int16_t song_i;

extern Menu_t* current_menu;
extern Menu_t mainmenu;
extern Menu_t kconfmenu;

Input_t Input = {0};
Input_t* g_Input = &Input;
uint8_t* g_Keyboard = Input.kb_array;

static void interrupt (far *old_keyhandler)(void);

void interrupt far keyhandler()
{
    uint8_t key_scan = 0;
    static uint8_t key_scan_last = 0;
    static uint8_t key_special = 0;
    static uint8_t key_special_last = 0;
    // obtain scancode
    asm {
        cli // clear interrupt flag; prevent other external interrupts
        in  al, 0x60
        mov key_scan, al
        in  al, 0x61 // get keyboard status
        mov bl, al
        or  al, 0x80 // MSB set
        out 0x61, al // write (ACK)
        mov al, bl   // MSB clear
        out 0x61, al // write
    }
    // remember the last special key code
    if (key_scan_last == KEY_SPECIAL_CODE)
        key_special = key_scan;
    else if (key_scan != KEY_SPECIAL_CODE)
        key_special = 0;
    // place scancode on queue unless it's the same as the previous one
    if (key_scan != key_scan_last && (key_special != key_special_last || key_special_last == 0))
    {
        g_Input->kb_queue[g_Input->kb_tail] = key_scan;
        if (g_Input->kb_tail+1 != g_Input->kb_head)
            g_Input->kb_tail++;
    }
    key_scan_last = key_scan;
    key_special_last = key_special;
    // PIC 8259 reset
    asm {
        mov al, 0x20
        out 0x20, al
        sti // set interrupt flag; allow external interrupts again
    }
}

void init_keyboard()
{
    uint8_t far *bios_key_state;
    asm cli
    // save address of current keyhandler interrupt function
    old_keyhandler = _dos_getvect(9);
    // caps lock & num lock off
    bios_key_state = MK_FP(0x040, 0x017);
    *bios_key_state &= (~(32|64));
    old_keyhandler(); 
    // replace old keyhandler with new keyhandler function
    _dos_setvect(9, keyhandler);
    asm sti
}

void init_default_keys()
{
    opt.up_movement = KEY_UP;
    opt.left_movement = KEY_LEFT;
    opt.down_movement = KEY_DOWN;
    opt.right_movement = KEY_RIGHT;
}

void deinit_keyboard()
{
    // restore old keyhandler
    asm cli
    _dos_setvect(9, old_keyhandler);
    asm sti

    g_Input = NULL;
}

void cursor_up()
{
    change_menu(); // optimize later
    if (current_menu->cursor_loc == 0) // if already at the top selection...
    {
        current_menu->cursor_y += (current_menu->num_selections - 1) * current_menu->cursor_spacing; // move cursor to bottom selection
        current_menu->cursor_loc = current_menu->num_selections - 1;
    }
    else
    {
        current_menu->cursor_y -= current_menu->cursor_spacing;
        current_menu->cursor_loc--;
    }
}

void cursor_down()
{
    change_menu(); // optimize later
    if (current_menu->cursor_loc == current_menu->num_selections - 1) // if already at the bottom selection...
    {
        current_menu->cursor_y = current_menu->start_y; // move cursor to top selection
        current_menu->cursor_loc = 0;
    }
    else
    {
        current_menu->cursor_y += current_menu->cursor_spacing;;
        current_menu->cursor_loc++;
    }
}

void control_menu()
{
    if (KEY_WAS_HIT(KEY_ENTER))
    {
        if (opt.sfx_on == TRUE)
        {
            play_note(100, 10);
        }
        menu_controller();
    }

    else if (KEY_WAS_HIT(KEY_UP))
    {
        if (opt.sfx_on == TRUE)
            play_note(600, 10);
        cursor_up(opt);
    }

    else if (KEY_WAS_HIT(KEY_DOWN))
    {
        if (opt.sfx_on == TRUE)
            play_note(600, 10);
        cursor_down(opt);
    }

    if (KEY_WAS_HIT(KEY_ESC))
    {
        if (current_menu == &kconfmenu)
        {
            menu_options();
        }
        else
        {
            menu_main();
        }
    }
}

void control_end()
{
    if (KEY_WAS_HIT(KEY_SPACE))
    {
        close_speaker();
        minutes = 0;
        seconds = 0;
        song_i = 0;
        g.game_state = GAME_MENU;
        menu_main();
        current_menu->cursor_y = current_menu->start_y;
    }
}

void control_ingame()
{
    static axis = 0;
    struct Actor* player = g.Actors+0; /* same as &g.Actors[0]*/
    
    player->x_vel = 0;
    player->y_vel = 0;
    
    if (KEY_WAS_HIT(KEY_UP) || KEY_WAS_HIT(KEY_DOWN))
    {
        axis = Y_AXIS;
        player->y_vel =
        KEY_WAS_HIT(KEY_DOWN) - KEY_WAS_HIT(KEY_UP);
    }
    else if (KEY_WAS_HIT(KEY_LEFT) || KEY_WAS_HIT(KEY_RIGHT))
    {
        axis = X_AXIS;
        player->x_vel =
        KEY_WAS_HIT(KEY_RIGHT) - KEY_WAS_HIT(KEY_LEFT);
    }
    else if (axis == Y_AXIS)
    {
        player->y_vel =
        (KEY_IS_PRESSED(KEY_DOWN)>>1) - (KEY_IS_PRESSED(KEY_UP)>>1);
        
        if (player->y_vel == 0)
            player->x_vel =
            (KEY_IS_PRESSED(KEY_RIGHT)>>1) - (KEY_IS_PRESSED(KEY_LEFT)>>1);
    }
    else if (axis == X_AXIS)
    {
        player->x_vel =
        (KEY_IS_PRESSED(KEY_RIGHT)>>1) - (KEY_IS_PRESSED(KEY_LEFT)>>1);
        
        if (player->x_vel == 0)
            player->y_vel =
            (KEY_IS_PRESSED(KEY_DOWN)>>1) - (KEY_IS_PRESSED(KEY_UP)>>1);
    }

    // Misc
    if (KEY_WAS_HIT(KEY_Q) && KEY_WAS_HIT(KEY_BACKSPACE) && opt.debugmode == 0)
    {
        opt.debugmode = TRUE;
        debug_screen_e();
    }
    
    else if (KEY_WAS_HIT(KEY_Q) && KEY_WAS_HIT(KEY_BACKSPACE) && opt.debugmode == 1)
    {
        opt.debugmode = FALSE;
        debug_screen_d();
    }

    else if(KEY_WAS_HIT(KEY_ESC))
    {
        minutes = 0;
        seconds = 0;
        song_i = 0;
        close_speaker();
        g.game_state = GAME_MENU;
        menu_main();
        current_menu->cursor_y = current_menu->start_y;
    }
    
    if (opt.debugmode == TRUE)
    {
        if (KEY_WAS_HIT(KEY_K))
            add_key();
        else if (KEY_WAS_HIT(KEY_L))
            add_life();
        else if (KEY_WAS_HIT(KEY_D))
            remove_life();
        else if (KEY_WAS_HIT(KEY_S))
            level_skip();
        else if (KEY_WAS_HIT(KEY_HOME))
            suicide();
    }
}

void get_keyboard()
{
    int i;
    uint8_t key_scan;
    uint8_t extended = 0;
    uint8_t released = 0;

    while (g_Input->kb_head != g_Input->kb_tail)
    {
        key_scan = g_Input->kb_queue[g_Input->kb_head++];
        /* handle special keys, but only if a second scancode follows*/
        if (key_scan == KEY_SPECIAL_CODE)
                extended = 128;
        /* remember the release flag before clearing it*/
        released = key_scan & KEY_RELEASED_FLAG;
        key_scan &= ~KEY_RELEASED_FLAG;
        if (released)
        {
            g_Input->kb_array[key_scan+extended] &= KEY_HIT_FLAG;
            g_Input->kb_array[key_scan+extended] |= KEY_RELEASED_FLAG;
        }
        else
            g_Input->kb_array[key_scan+extended] |= (KEY_HIT_FLAG|KEY_PRESSED_FLAG);

        extended = 0;
    }
}

void clear_keys()
{
    int i;
    for (i = 0; i < KB_ARRAY_LENGTH; i++)
        g_Input->kb_array[i] &= KEY_PRESSED_FLAG;
}

void process_input()
{
    get_keyboard();
    
    if (g.game_state == GAME_MENU)
        control_menu();
    else if (g.game_state == GAME_END)
        control_end();
    else if (g.game_state == GAME_INGAME)
        control_ingame();
    
    // F10 always exits, wherever you are
    if (KEY_WAS_HIT(KEY_F10))
        g.game_running = FALSE;

    clear_keys();
}
#include "Shared.h"
#include "Keyb.h"
#include "Keys.h"

extern struct GameData g;
extern struct Cursor cursor;
extern struct Options opt;

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

void init_default_keys(struct Options* opt)
{
    opt->up_movement = KEY_UP;
    opt->left_movement = KEY_LEFT;
    opt->down_movement = KEY_DOWN;
    opt->right_movement = KEY_RIGHT;
}

void deinit_keyboard()
{
    // restore old keyhandler
    asm cli
    _dos_setvect(9, old_keyhandler);
    asm sti

    g_Input = NULL;
}

void cursor_up(struct Cursor* cursor, struct Options* opt, uint8_t max_choices, uint8_t choice_spacing)
{
    change_menu(opt, cursor); // optimize later
    if (cursor->selection == 0) // if already at the top selection...
    {
        cursor->old_y = cursor->new_y;
        cursor->new_y = cursor->old_y + max_choices * choice_spacing; // move cursor to bottom selection
        cursor->selection = max_choices;
    }
    else
    {
        cursor->old_y = cursor->new_y;
        cursor->new_y = cursor->new_y - choice_spacing;
        cursor->selection--;
    }
}

void cursor_down(struct Cursor* cursor, struct Options* opt, uint8_t max_choices, uint8_t choice_spacing)
{
    change_menu(opt, cursor); // optimize later
    if (cursor->selection == max_choices) // if already at the bottom selection...
    {
        cursor->new_y = cursor->new_y - max_choices * choice_spacing; // move cursor to top selection
        cursor->selection = 0;
    }
    else
    {
        cursor->new_y = cursor->new_y + choice_spacing;
        cursor->selection++;
    }
}

void control_menu(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    uint8_t max_choices;
    uint8_t choice_spacing;

    if (opt->menu_status == MENU_MAIN)
    {
        max_choices = 4;
        choice_spacing = 15;
    }

    else if (opt->menu_status == MENU_OPTIONS)
    {
        max_choices = 2;
        choice_spacing = 40;
    }


    if (opt->menu_status != MENU_HELP && opt->menu_status != MENU_STORY) // disable cursor controls in Help and Story menus (as they have nothing to select)
    {
        if (KEY_WAS_HIT(KEY_ENTER))
        {
            if (opt->sfx_on == 1)
            {
                play_note(100, 10);
            }
            cursor->old_selection = cursor->selection;
            cursor->old_y = cursor->new_y;
            menu_controller(g, cursor, opt);
        }

        else if (KEY_WAS_HIT(KEY_UP))
        {
            if (opt->sfx_on == 1)
                play_note(600, 10);
            cursor_up(cursor, opt, max_choices, choice_spacing);
        }

        else if (KEY_WAS_HIT(KEY_DOWN))
        {
            if (opt->sfx_on == 1)
                play_note(600, 10);
            cursor_down(cursor, opt, max_choices, choice_spacing);
        }
    }

    if (KEY_WAS_HIT(KEY_ESC))
    {
        if (opt->menu_status != MENU_MAIN && opt->menu_status != MENU_KEYCONF)
        {
            opt->menu_status = MENU_MAIN;
            cursor->new_y = cursor->old_y;
            cursor->selection = cursor->old_selection;
            change_menu(opt, cursor);
        }
        else if (opt->menu_status == MENU_KEYCONF)
        {
            opt->menu_status = MENU_OPTIONS;
            cursor->new_y = 145;
            cursor->selection = 2;
            cursor->old_y = 80;
            cursor->old_selection = 1;
            change_menu(opt, cursor);
        }
    }
}

void control_end(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    if (KEY_WAS_HIT(KEY_SPACE))
    {
        g->game_state = GAME_MENU;
        opt->menu_status = MENU_MAIN;
        change_menu(opt, cursor);
        cursor->new_y = 65;
        cursor->selection = 0;
    }
}

void control_ingame(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    static axis = 0;
    struct Actor* player = g->Actors+0; // same as &g->Actors[0]
    
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
    if (KEY_WAS_HIT(KEY_Q) && KEY_WAS_HIT(KEY_BACKSPACE) && opt->debugmode == 0)
    {
        opt->debugmode = 1;
        debug_screen_e();
    }
    
    else if (KEY_WAS_HIT(KEY_Q) && KEY_WAS_HIT(KEY_BACKSPACE) && opt->debugmode == 1)
    {
        opt->debugmode = 0;
        debug_screen_d();
    }

    else if(KEY_WAS_HIT(KEY_ESC))
    {
        g->game_state = GAME_MENU;
        opt->menu_status = MENU_MAIN;
        change_menu(opt, cursor);
        cursor->new_y = 65;
        cursor->selection = 0;
    }
    
    if (opt->debugmode == 1)
    {
        if (KEY_WAS_HIT(KEY_K))
            add_key(g);
        else if (KEY_WAS_HIT(KEY_L))
            add_life(g);
        else if (KEY_WAS_HIT(KEY_D))
            remove_life(g);
        else if (KEY_WAS_HIT(KEY_S))
            level_skip(g);
        else if (KEY_WAS_HIT(KEY_HOME))
            suicide(g);
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
        //handle special keys, but only if a second scancode follows
        if (key_scan == KEY_SPECIAL_CODE)
                extended = 128;
        // remember the release flag before clearing it
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

void process_input(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{
    get_keyboard();
    
    if (g->game_state == GAME_MENU)
        control_menu(g, cursor, opt);
    else if (g->game_state == GAME_END)
        control_end(g, cursor, opt);
    else if (g->game_state == GAME_INGAME)
        control_ingame(g, cursor, opt);
    
    // F10 always exits, wherever you are
    if (KEY_WAS_HIT(KEY_F10))
        g->game_running = 0;

    clear_keys();
}
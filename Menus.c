#include "Shared.h"
#include "Menus.h"

extern struct GameData g;
extern struct Settings opt;

uint8_t story_text[] =
    "I COULD WRITE A FANCY STORY BUT" 
    "CAN'T BE ARSED RN LOL";

Menu_t* current_menu;

Option_t mainmenu_options[] =
{
  {"START",     start_game}, 
  {"OPTIONS",   menu_options},
  {"HELP!",     menu_help},
  {"STORY",     menu_story},
  {"QUIT",      quit_game}
};

Option_t settings_options[] =
{
  {"SOUND",         opt_sfx}, 
  {"MUSIC",         opt_music},
  {"KEY CONFIG",    menu_kconf}
};

Option_t kconf_options[] =
{
  {"UP",      dummy}, 
  {"LEFT",    dummy},
  {"DOWN",    dummy},
  {"RIGHT",   dummy}
};

Option_t basic_options[] =
{
  {"BACK TO MAIN", menu_main}
};

Menu_t mainmenu =
{
  "GFX/SPLASH.7UP",
  5,
  0,
  65,
  65,
  105,
  15,
  mainmenu_options
};

Menu_t optionsmenu =
{
  "GFX/OPTIONS.7UP",
  3,
  0,
  65,
  65,
  91,
  40,
  settings_options
};

Menu_t kconfmenu =
{
  "GFX/KCONF.7UP",
  4,
  0,
  65,
  65,
  80,
  30,
  kconf_options
};

Menu_t helpmenu =
{
  "GFX/HELP.7UP",
  1,
  0,
  180,
  180,
  80,
  0,
  basic_options
};

Menu_t storymenu =
{
  "GFX/STORY.7UP",
  1,
  0,
  180,
  180,
  80,
  0,
  basic_options
};

void menu_main()
{
    current_menu = &mainmenu;
    change_menu();
}

void menu_options()
{
    current_menu = &optionsmenu;
    change_menu();
}

void menu_kconf()
{
    current_menu = &kconfmenu;
    change_menu();
}

void menu_help()
{
    current_menu = &helpmenu;
    change_menu();
    draw_help_contents();
}

void menu_story()
{
    current_menu = &storymenu;
    change_menu();
    render_text(230, 185, "V.0.0023A", 0);
    typewriter(4, 52, story_text, 15);
}

void start_game()
{
    start_screen();
    delay(2000);
    
    fill_screen(0);
    g.game_state = GAME_INGAME;
    g.level_num = 1;
    g.player_lives = 3;
    level_loader();
    music_track_select();
}

void quit_game()
{
    g.game_running = FALSE;
}

void opt_sfx()
{
    if (opt.sfx_on == TRUE)
        opt.sfx_on = FALSE;
    else
        opt.sfx_on = TRUE;
    change_menu(); // optimize later
}

void opt_music()
{
    if (opt.music_on == TRUE)
        opt.music_on = FALSE;
    else
        opt.music_on = TRUE;
    change_menu(); // optimize later
}

void dummy()
{

}

void menu_controller()
{
    current_menu->options[current_menu->cursor_loc].action();
}
#include "Shared.h"
#include "Gfx.h"

// globals
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t far screen_buf [64000]; // double buffer
uint8_t minutes = 0;
uint16_t seconds = 0;

union REGS regs;

extern struct GameData g;
extern struct Cursor cursor;
extern struct Options opt;
extern uint32_t timer;
extern int secdiv;

// reserve memory for sprites
uint8_t far menu_bground [64000];
uint8_t alphabet [4240];
uint8_t sprites[NUM_SPRITES][TILE_AREA];
uint8_t temp_tile [64];

uint8_t* gfx_table[256];

char bgfilename [13];

uint8_t story_text[] =
    "I COULD WRITE A FANCY STORY BUT" 
    "CAN'T BE ARSED RN LOL";

uint8_t anims[NUM_ANIMS][192];

struct Sprite player_sprite = {anims[SPR_PLAYER], 8, 8, 64, 3, 0};
struct Sprite guard_sprite = {anims[SPR_GUARD], 8, 8, 64, 3, 0};
struct Sprite explo_sprite = {anims[SPR_EXPLO], 8, 8, 64, 3, 0};

void set_tile_gfx()
{
    _fmemset(gfx_table, SPR_ERROR, 256);
    gfx_table['W'] = sprites[TILE_WALL];
    gfx_table['-'] = sprites[TILE_FLOOR];
    gfx_table['e'] = sprites[TILE_EXIT];
    gfx_table['|'] = sprites[COMP_DOOR_C];
    gfx_table['_'] = sprites[TILE_DOOR_O];
    gfx_table['*'] = sprites[COMP_KEY];
    gfx_table['^'] = sprites[COMP_MINE];
}

void set_mode(uint8_t mode)
{
    regs.h.ah = SET_MODE;
    regs.h.al = mode;
    int86(VIDEO_INT, &regs, &regs);
}

void fill_screen(uint8_t color)
{
    _fmemset(screen_buf, color, SCREEN_SIZE);
}

void load_gfx(char* filename, uint8_t* destination, uint16_t data_size)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "rb");
    fread(destination, 1, data_size, file_ptr);
    fclose(file_ptr);
}

void load_sprite(int tile_id, char* filename)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "rb");
    fread(sprites[tile_id], 1, TILE_AREA, file_ptr);
    fclose(file_ptr);
}

void load_anim(int anim_id, char* filename)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "rb");
    fread(anims[anim_id], 1, 192, file_ptr);
    fclose(file_ptr);
}

void load_tiles()
{
    load_sprite(TILE_FLOOR, "GFX/FLOOR.7UP");
    load_sprite(TILE_WALL, "GFX/BRICKS.7UP");
    load_sprite(TILE_DOOR_C, "GFX/DOORC.7UP");
    load_sprite(TILE_DOOR_O, "GFX/DOORO.7UP");
    load_sprite(TILE_EXIT, "GFX/EXIT.7UP");
    load_sprite(ITEM_KEY, "GFX/KEY.7UP");
    load_sprite(ITEM_MINE, "GFX/MINE.7UP");
    load_sprite(SHAD_IN_COR, "GFX/SHAD_IC.7UP");
    load_sprite(SHAD_HORZ, "GFX/SHAD_H.7UP");
    load_sprite(SHAD_VERT, "GFX/SHAD_V.7UP");
    load_sprite(SHAD_OUT_COR, "GFX/SHAD_OC.7UP");
    load_sprite(SHAD_MINE, "GFX/SHAD_M.7UP");
    load_sprite(SHAD_KEY, "GFX/SHAD_K.7UP");
    load_anim(SPR_PLAYER, "GFX/PLAYER.7UP");
    load_anim(SPR_GUARD, "GFX/GUARD.7UP");
    load_anim(SPR_EXPLO, "GFX/EXPLO.7UP");
    load_sprite(SPR_GRAVE, "GFX/GRAVE.7UP");
    load_sprite(SPR_ERROR, "GFX/ERROR.7UP");
}

void load_special_gfx()
{
    load_gfx("GFX/FONT.7UP", alphabet, 4240);
}

void composite_sprite(uint8_t* background, uint8_t* foreground, uint8_t* destination)
{
    draw_temp(background);
    draw_temp(foreground);
    _fmemcpy(destination, temp_tile, TILE_AREA);
}

void create_composites()
{
    composite_sprite(sprites[TILE_FLOOR], sprites[ITEM_MINE], sprites[COMP_MINE]);
    composite_sprite(sprites[TILE_FLOOR], sprites[ITEM_KEY], sprites[COMP_KEY]);
    composite_sprite(sprites[TILE_FLOOR], sprites[TILE_DOOR_C], sprites[COMP_DOOR_C]);
}

void draw_sprite(struct GameData* g, int x, int y, uint8_t* sprite)
{
    int index_x = 0;
    int index_y = 0;
    int i = 0;

    x += g->render_offset_x;
    y += g->render_offset_y;

    for (index_y=0;index_y<TILE_HEIGHT;index_y++)
    {
        for (index_x=0;index_x<TILE_WIDTH;index_x++)
        {
            SET_PIXEL(x, y, sprite[i]);
            i++;
            x++;
        }
        index_x = 0;
        x -= TILE_WIDTH;
        y++;
    }
    index_y = 0;
}

void draw_temp(uint8_t* sprite)
{
    uint8_t index_x = 0;
    uint8_t index_y = 0;
    uint8_t i = 0;

    for (index_y=0;index_y<TILE_HEIGHT;index_y++)
    {
        for (index_x=0;index_x<TILE_WIDTH;index_x++)
        {
            if (sprite[i] != 13)
            {
                temp_tile[i]=sprite[i];
                i++;
            }
            else
            {
                i++;
            }
        }
        index_x = 0;
    }
    index_y = 0;
}

void draw_sprite_tr(struct GameData* g, int x, int y, uint8_t* sprite)
{
    uint8_t index_x = 0;
    uint8_t index_y = 0;
    int pixel = 0;
    
    x += g->render_offset_x;
    y += g->render_offset_y;

    for (index_y=0;index_y<TILE_HEIGHT;index_y++)
    {
        for (index_x=0;index_x<TILE_WIDTH;index_x++)
        {
            if (sprite[pixel] != 13)
            {
                SET_PIXEL(x, y, sprite[pixel]);
                pixel++;
                x++;
            }
            else
            {
                pixel++;
                x++;
            }
        }
        index_x = 0;
        x = x - TILE_WIDTH;
        y++;
    }
    index_y = 0;
}

void draw_big(int x, int y, int w, int h, uint8_t* sprite)
{
    int index_x = 0;
    int index_y = 0;
    int i = 0;

    for (index_y=0; index_y<h;index_y++)
    {
        for (index_x=0; index_x<w;index_x++)
        {
            SET_PIXEL(x, y, sprite[i]);
            i++;
            x++;
        }
        index_x = 0;
        x -= w;
        y++;
    }
    index_y = 0;
}

void draw_rectangle(int x, int y, int w, int h, uint8_t color)
{
    int index_x = 0;
    int index_y = 0;

    for (index_y=0; index_y<h;index_y++)
    {
        for (index_x=0; index_x<w;index_x++)
        {
            SET_PIXEL(x, y, color);
            x++;
        }
        index_x = 0;
        x -= w;
        y++;
    }
    index_y = 0;
}

void draw_shadow(struct GameData* g, int x, int y)
{    
    int pixel_x = x*TILE_WIDTH;
    int pixel_y = y*TILE_HEIGHT;

    if (TILE_AT(x, y) == MAP_FLOOR)
    {
        if (TILE_AT(x-1, y-1) == MAP_WALL &&
            TILE_AT(x,   y-1) == MAP_WALL &&
            TILE_AT(x-1, y  ) == MAP_WALL)
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_IN_COR]);
            
        else if (TILE_AT(x, y-1) == MAP_WALL)
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
        
        else if (TILE_AT(x-1, y) == MAP_WALL)
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_VERT]);
        
        else if (TILE_AT(x-1, y-1) == MAP_WALL)
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_OUT_COR]);
    }

    else if (TILE_AT(x, y) == MAP_MINE)
    {
        if (TILE_AT(x-1, y) == MAP_WALL && TILE_AT(x, y-1) == MAP_WALL)
        {
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_MINE]);
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
        }
        else if  (TILE_AT(x-1, y) == MAP_WALL)
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_MINE]);

        else if (TILE_AT(x, y-1) == MAP_WALL)
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
    }

    else if (TILE_AT(x, y) == MAP_KEY)
    {

        if (TILE_AT(x-1, y) == MAP_WALL && TILE_AT(x, y-1) == MAP_WALL)
        {
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_KEY]);
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
        }
        else if  (TILE_AT(x-1, y) == MAP_WALL)
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_KEY]);

        else if (TILE_AT(x, y-1) == MAP_WALL)
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
    }
}

void draw_text(int x, int y, int i, uint8_t color)
{
    uint8_t index_x = 0;
    uint8_t index_y = 0;
    i = i * CHARACTER_SIZE;

    for (index_y=0;index_y<TILE_HEIGHT;index_y++)
    {
        for (index_x=0;index_x<TILE_WIDTH;index_x++)
        {
            if (alphabet[i] != 13)
            {
                SET_PIXEL(x, y, alphabet[i] + color);
                i++;
                x++;
            }
            else
            {
                i++;
                x++;
            }
        }
        index_x = 0;
        x = x - TILE_WIDTH;
        y++;
    }
    index_y = 0;
    i= 0;
}

void render_text(int x, int y, char* string, uint8_t color)
{
    int i = 0;
    char c;
    
    while (string[i] != 0)
    {
        c = string[i];
        draw_text(x, y, c - 32, color);
        x = x + 10;
        i++;
    }
}

void render_floor(struct GameData* g, int x, int y) // used to empty tiles after an item is picked up
{
    draw_sprite     (g, x * TILE_WIDTH,y * TILE_HEIGHT,sprites[TILE_FLOOR]);
    draw_shadow     (g, x, y);
}

void render_door(struct GameData* g, int x, int y) // used to draw an open door
{
    draw_sprite     (g, x * TILE_WIDTH,y * TILE_HEIGHT, sprites[TILE_FLOOR]);
    draw_shadow     (g, x, y);
    draw_sprite_tr  (g, x * TILE_WIDTH,y * TILE_HEIGHT, sprites[TILE_DOOR_O]);
}

void render_mine(struct GameData* g, int x, int y) // used to draw mines, duh
{
    draw_sprite (g, x * TILE_WIDTH,y * TILE_HEIGHT,sprites[COMP_MINE]);
    draw_shadow (g, x, y);
}

void typewriter(struct Options* opt, int x, int y, char* string, uint8_t color)
{
    int old_x = x;

    int i = 0;
    char c;
    
    while (string[i] != 0)
    {
        c = string[i];
        draw_text(x, y, c - 32, color);
        x = x + 10;
        i++;
        if (opt->sfx_on == 1)
            sound_typing();
        memcpy(VGA,screen_buf,SCREEN_SIZE);
        delay(100);
        if (x > 311)
        {
            x = old_x;
            y = y + 10;
        }
    }
}

void render_stats(struct GameData* g)
{
    char lvl_str[12];
    char keys_str[12];
    char lives_str[12];

    sprintf(lvl_str, "LEVEL: %d", g->level_num);
    sprintf(keys_str, "KEYS: %d", g->keys_acquired);
    sprintf(lives_str, "LIVES: %d", g->player_lives);

    render_text(1, 1, lvl_str, 15);

    if (g->keys_acquired < 100)
        render_text(120, 1, keys_str, 15);
    else
        render_text(120, 1, "KEYS: 99", 15);
    
    if (g->player_lives > -1 && g->player_lives < 100)
        render_text(230, 1, lives_str, 15);
    else if (g->player_lives > 99)
        render_text(230, 1, "LIVES: 99", 15);
    else if (g->player_lives > -2)
        render_text(230, 1, "LIVES: 0", 15);
}

void render_maze(struct GameData* g)
{   
    int counter = 0;
    int y = 0;
    int x = 0;
    
    while (y < g->level_height)
    {
        while (x < g->level_width)
        {
            draw_sprite(g, x * TILE_WIDTH, y * TILE_HEIGHT, gfx_table[g->tile_data[counter]]);
            draw_shadow(g, x, y);
            counter++;
            x++;
        }
    y++;
    x = 0;
    }

    render_stats(g);
}

void anim_explosion(struct GameData* g)
{
    struct Actor p = g->Actors[0];
    uint8_t* pixels;

    p.sprite = explo_sprite;
    pixels = p.sprite.pixels;

    SET_TILE(p.x, p.y, MAP_FLOOR); // "delete" mine
    render_floor(g, p.x, p.y); // "delete" player sprite

    for (p.sprite.frame = 0;p.sprite.frame < p.sprite.num_frames;p.sprite.frame++)
    {
        delay(150);
        draw_sprite_tr(g, p.x * TILE_WIDTH,
                       p.y * TILE_HEIGHT,
                       pixels + (p.sprite.frame * p.sprite.size));
        memcpy(VGA,screen_buf,SCREEN_SIZE);
    }
    
    for (p.sprite.frame = 2;p.sprite.frame > -1;p.sprite.frame--)
    {
        delay(150);
        render_floor(g, p.x, p.y);
        draw_sprite_tr(g, p.x * TILE_WIDTH,
                       p.y * TILE_HEIGHT,
                       pixels + (p.sprite.frame * p.sprite.size));
        memcpy(VGA,screen_buf,SCREEN_SIZE);
    }

    delay(150);
    render_floor(g, p.x, p.y);
    
    p.sprite = player_sprite;   
}

void render_actors(struct GameData* g)
{
    uint8_t* pixels;
    int i;
    
    i = 0;
    while (i < g->actor_count)
    {
        
        if (pixels == sprites[SPR_ERROR])
        {
            i++;
            continue;
        }

        else
            pixels = g->Actors[i].sprite.pixels;

        g->Actors[i].sprite.frame++;

        if (g->Actors[i].sprite.frame >= g->Actors[i].sprite.num_frames)
            g->Actors[i].sprite.frame = 0;
        
        draw_sprite_tr(g, g->Actors[i].x * TILE_WIDTH,
                    g->Actors[i].y * TILE_HEIGHT,
                    pixels + (g->Actors[i].sprite.frame * g->Actors[i].sprite.size));

        // if actor has changed position, render an empty floor tile in the old location, unless it's a door frame or mine         
        if (g->Actors[i].x != g->Actors[i].old_x || g->Actors[i].y != g->Actors[i].old_y)
        {
            if (TILE_AT(g->Actors[i].old_x, g->Actors[i].old_y) == MAP_FLOOR)
                render_floor(g, g->Actors[i].old_x, g->Actors[i].old_y);
            else if (TILE_AT(g->Actors[i].old_x, g->Actors[i].old_y) == MAP_DOOR_O)
                render_door(g, g->Actors[i].old_x, g->Actors[i].old_y);
            else if (TILE_AT(g->Actors[i].old_x, g->Actors[i].old_y) == MAP_MINE)
                render_mine(g, g->Actors[i].old_x, g->Actors[i].old_y);
        }
                     
        i++;
    }
    
    i = 0;
    while (i < g->actor_count)
    {
        
        switch (g->Actors[i].type)
        {
            case ACTOR_GRAVE:   pixels = sprites[SPR_GRAVE];  break;
            case ACTOR_EMPTY:   pixels = sprites[SPR_GRAVE];  break;
            default:            pixels = sprites[SPR_ERROR];  break;
        }
        
        if (pixels == sprites[SPR_ERROR])
        {
            i++;
            continue;
        }
        
        draw_sprite_tr(g, g->Actors[i].x * TILE_WIDTH,
                       g->Actors[i].y * TILE_HEIGHT,
                       pixels);                       
        i++;
    }
}

void render_menu_text(struct Options* opt, struct Cursor* cursor)
{
    if (opt->menu_status == MENU_MAIN)
    {
        cursor->x = 105;
        render_text(125, 65, "START", 15);
        render_text(125, 80, "OPTIONS", 15);
        render_text(125, 95, "HELP!", 15);
        render_text(125, 110, "STORY", 15);
        render_text(125, 125, "QUIT", 15);
    }

    else if(opt->menu_status == MENU_OPTIONS)
    {
        cursor->x = 91;
        render_text(111, 65, "SOUNDS", 15);
        if (opt->sfx_on == 1)
            render_text(191, 65, "ON", 15);
        else if (opt->sfx_on == 0)
            render_text(191, 65, "OFF", 15);
        render_text(111, 105, "MUSIC", 15);
        if (opt->music_on == 1)
            render_text(191, 105, "ON", 15);
        else if (opt->music_on == 0)
            render_text(191, 105, "OFF", 15);
        render_text(111, 145, "KEY CONFIG", 15);
    }
    else if(opt->menu_status == MENU_STORY)
    {
        render_text(200, 185, "VER. 0.0019A", 0); 
        typewriter(opt, 4, 52, story_text, 15);
    }

    render_text(200, 185, "VER. 0.0019A", 0);    
}

void draw_help_contents(struct GameData* g)
{
    // reset offsets in case they have been changed in-game
    g->render_offset_x = 0;
    g->render_offset_y = 0;

    draw_sprite_tr(g, 48, 64, anims[SPR_PLAYER]);
    draw_sprite(g, 48, 76, sprites[TILE_WALL]);
    draw_sprite_tr(g, 48, 88, anims[SPR_GUARD]);
    draw_sprite_tr(g, 48, 100, sprites[ITEM_MINE]);
    draw_sprite_tr(g, 48, 112, sprites[TILE_DOOR_C]);
    draw_sprite_tr(g, 48, 124, sprites[ITEM_KEY]);
    draw_sprite(g, 48, 135,sprites[TILE_EXIT]);
}

void change_menu(struct Options* opt, struct Cursor* cursor)
{
    char* bgfilename;

    if (opt->menu_status == MENU_OPTIONS)
        bgfilename = "GFX/OPTIONS.7UP";
    
    else if (opt->menu_status == MENU_KEYCONF)
        bgfilename = "GFX/KCONF.7UP";

    else if (opt->menu_status == MENU_HELP)
        bgfilename = "GFX/HELP.7UP";

    else if (opt->menu_status == MENU_STORY)
        bgfilename = "GFX/STORY.7UP";

    else
        bgfilename = "GFX/SPLASH.7UP";

    load_gfx(bgfilename, menu_bground, 64000);

    draw_big(0, 0, 320, 200, menu_bground);

    render_menu_text(opt, cursor);
}

void render_cursor(int x, int old_y, int new_y)
{
    render_text(x, new_y, "->", 15);
}

void render_menu(struct Cursor* cursor)
{
    render_cursor(cursor->x, cursor->old_y, cursor->new_y);
}

void render_end()
{
    fill_screen(0);
    render_text(122, 96, "YOU WIN!", 14);
}

void start_screen(struct Options* opt)
{
    fill_screen(0);
    
    typewriter(opt, 102, 96, "GET PSYCHED!", 1);
}

void gameover_screen()
{
    draw_rectangle(111, 95, 98, 10, 0);
    render_text(112, 96, "GAME OVER!", 4);
    memcpy(VGA,screen_buf,SCREEN_SIZE);
}

void debug_screen_e()
{
    draw_rectangle(66, 19, 188, 10, 1);
    render_text(67, 20, "DEBUG MODE ENABLED!", 15);
    memcpy(VGA,screen_buf,SCREEN_SIZE);
    delay(1000);
    draw_rectangle(61, 19, 198, 10, 0);
}

void debug_screen_d()
{
    draw_rectangle(61, 19, 198, 10, 1);
    render_text(62, 20, "DEBUG MODE DISABLED!", 15);
    memcpy(VGA,screen_buf,SCREEN_SIZE);
    delay(1000);
    draw_rectangle(61, 19, 198, 10, 0);
}

void render_time()
{
    char time_str[12];
    uint8_t r_seconds;

    seconds = timer / secdiv; // actual seconds passed
    r_seconds = seconds - minutes * 60; // rendered seconds

    if (r_seconds >= 60)
    {
        minutes++;
        sprintf(time_str, "TIME: %d:0", minutes);
    }
    else
        sprintf(time_str, "TIME: %d:%d", minutes, r_seconds);
        
    draw_rectangle(270, 185, 50, 8, 0);
    render_text(210, 185, time_str, 15);
}

void render(struct GameData* g, struct Cursor* cursor, struct Options* opt)
{        
    // in case playing, just died, or exited
    // draw play field and objects
    if (g->game_state == GAME_INGAME ||
        g->game_state == GAME_OVER ||
        g->game_state == GAME_WIN)
    {
        render_time();
        render_actors(g);
        if (g->Actors[0].type == ACTOR_EXPLO)
            anim_explosion(g);
    }
    // if main, options or keyconfig menu ...
    else if (g->game_state == GAME_MENU)
    {
        if (opt->menu_status != MENU_HELP && opt->menu_status != MENU_STORY)
            render_menu(cursor);
    }
    // if end ...
    else if (g->game_state == GAME_END)
    {
        render_end();
    }
    
    vretrace(&g, &cursor, &opt);
    memcpy(VGA,screen_buf,SCREEN_SIZE);
}
#include "Shared.h"
#include "Gfx.h"

// globals
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t far screen_buf [64000]; // double buffer

union REGS regs;

extern struct GameData g;

// reserve memory for sprites
uint8_t far splash_screen [64000];
uint8_t alphabet [4240];
uint8_t sprites[NUM_SPRITES][TILE_AREA];
uint8_t temp_tile [64];

uint8_t* gfx_table[256];

void set_tile_gfx()
{
    _fmemset(gfx_table, ENT_ERROR, 256);
    gfx_table['W'] = sprites[TILE_WALL];
    gfx_table['-'] = sprites[TILE_FLOOR];
    gfx_table['e'] = sprites[TILE_EXIT];
    gfx_table['|'] = sprites[TILE_DOOR_C];
    gfx_table['_'] = sprites[TILE_DOOR_O];
    gfx_table['*'] = sprites[ITEM_KEY];
    gfx_table['^'] = sprites[ITEM_MINE];
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

void load_tiles()
{
    load_sprite(TILE_FLOOR, "GFX/FLOOR.7UP");
    load_sprite(TILE_WALL, "GFX/BRICKS.7UP");
    load_sprite(TILE_DOOR_C, "GFX/DOORC.7UP");
    load_sprite(TILE_DOOR_O, "GFX/DOORO.7UP");
    load_sprite(TILE_EXIT, "GFX/EXIT.7UP");
    load_sprite(ITEM_KEY, "GFX/KEY.7UP");
    load_sprite(ITEM_MINE, "GFX/MINE.7UP");
    load_sprite(ENT_PLAYER, "GFX/PLAYER.7UP");
    load_sprite(ENT_GUARD, "GFX/GUARD.7UP");
    load_sprite(ENT_EXPLO, "GFX/EXPLO.7UP");
    load_sprite(ENT_GRAVE, "GFX/GRAVE.7UP");
    load_sprite(ENT_ERROR, "GFX/ERROR.7UP");
    load_sprite(SHAD_IN_COR, "GFX/SHAD_IC.7UP");
    load_sprite(SHAD_HORZ, "GFX/SHAD_H.7UP");
    load_sprite(SHAD_VERT, "GFX/SHAD_V.7UP");
    load_sprite(SHAD_OUT_COR, "GFX/SHAD_OC.7UP");
    load_sprite(SHAD_MINE, "GFX/SHAD_M.7UP");
    load_sprite(SHAD_KEY, "GFX/SHAD_K.7UP");
}

void load_special_gfx()
{
    load_gfx("GFX/SPLASH.7UP", splash_screen, 64000);
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
    composite_sprite(sprites[TILE_FLOOR], sprites[ITEM_MINE], sprites[ITEM_MINE]);
    composite_sprite(sprites[TILE_FLOOR], sprites[ITEM_KEY], sprites[ITEM_KEY]);
    composite_sprite(sprites[TILE_FLOOR], sprites[TILE_DOOR_C], sprites[TILE_DOOR_C]);
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
            screen_buf[y*SCREEN_WIDTH+x]=sprite[i];
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
    uint8_t i = 0;
    
    x += g->render_offset_x;
    y += g->render_offset_y;

    for (index_y=0;index_y<TILE_HEIGHT;index_y++)
    {
        for (index_x=0;index_x<TILE_WIDTH;index_x++)
        {
            if (sprite[i] != 13)
            {
                screen_buf[y*SCREEN_WIDTH+x]=sprite[i];
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
            screen_buf[y*SCREEN_WIDTH+x]=sprite[i];
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
    int i = 0;

    for (index_y=0; index_y<h;index_y++)
    {
        for (index_x=0; index_x<w;index_x++)
        {
            screen_buf[y*SCREEN_WIDTH+x]=color;
            i++;
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

    if (TILE_AT(x, y) == '-')
    {
        if (TILE_AT(x-1, y-1) == 'W' &&
            TILE_AT(x,   y-1) == 'W' &&
            TILE_AT(x-1, y  ) == 'W')
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_IN_COR]);
            
        else if (TILE_AT(x, y-1) == 'W')
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
        
        else if (TILE_AT(x-1, y) == 'W')
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_VERT]);
        
        else if (TILE_AT(x-1, y-1) == 'W')
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_OUT_COR]);
    }

    else if (TILE_AT(x, y) == '^')
    {
        if (TILE_AT(x-1, y) == 'W' && TILE_AT(x, y-1) == 'W')
        {
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_MINE]);
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
        }
        else if  (TILE_AT(x-1, y) == 'W')
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_MINE]);

        else if (TILE_AT(x, y-1) == 'W')
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
    }

    else if (TILE_AT(x, y) == '*')
    {

        if (TILE_AT(x-1, y) == 'W' && TILE_AT(x, y-1) == 'W')
        {
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_KEY]);
            draw_sprite_tr(g, pixel_x, pixel_y, sprites[SHAD_HORZ]);
        }
        else if  (TILE_AT(x-1, y) == 'W')
            draw_sprite_tr (g, pixel_x, pixel_y, sprites[SHAD_KEY]);

        else if (TILE_AT(x, y-1) == 'W')
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
                screen_buf[y*SCREEN_WIDTH+x]=alphabet[i] + color;
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
    draw_sprite     (g, x * TILE_WIDTH,y * TILE_HEIGHT,sprites[TILE_FLOOR]);
    draw_shadow     (g, x, y);
    draw_sprite_tr  (g, x * TILE_WIDTH,y * TILE_HEIGHT,sprites[TILE_DOOR_O]);
}

void render_mine(struct GameData* g, int x, int y) // used to draw mines, duh
{
    draw_sprite (g, x * TILE_WIDTH,y * TILE_HEIGHT,sprites[ITEM_MINE]);
    draw_shadow (g, x, y);
}

void typewriter(int x, int y, char* string, uint8_t color)
{
    int i = 0;
    char c;
    
    while (string[i] != 0)
    {
        c = string[i];
        draw_text(x, y, c - 32, color);
        x = x + 10;
        i++;
        sound_typing();
        memcpy(VGA,screen_buf,SCREEN_SIZE);
        delay(100);
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

void render_actors(struct GameData* g)
{
    uint8_t* sprite;
    int i;
    
    i = 0;
    while (i < g->actor_count)
    {
        
        switch (g->Actors[i].type)
        {
            case ACTOR_PLAYER:  sprite = sprites[ENT_PLAYER]; break;
            case ACTOR_GUARD:   sprite = sprites[ENT_GUARD];  break;
            default:            sprite = sprites[ENT_ERROR];  break;
        }
        
        if (sprite == sprites[ENT_ERROR])
        {
            i++;
            continue;
        }
        
        draw_sprite_tr(g, g->Actors[i].x * TILE_WIDTH,
                       g->Actors[i].y * TILE_HEIGHT,
                       sprite);

        // if actor has changed position, render an empty floor tile in the old location, unless it's a door frame or mine         
        if (g->Actors[i].x != g->Actors[i].old_x || g->Actors[i].y != g->Actors[i].old_y)
        {
            if (TILE_AT(g->Actors[i].old_x, g->Actors[i].old_y) == '-')
                render_floor(g, g->Actors[i].old_x, g->Actors[i].old_y);
            else if (TILE_AT(g->Actors[i].old_x, g->Actors[i].old_y) == '_')
                render_door(g, g->Actors[i].old_x, g->Actors[i].old_y);
            else if (TILE_AT(g->Actors[i].old_x, g->Actors[i].old_y) == '^')
                render_mine(g, g->Actors[i].old_x, g->Actors[i].old_y);
        }
                     
        i++;
    }
    
    i = 0;
    while (i < g->actor_count)
    {
        
        switch (g->Actors[i].type)
        {
            case ACTOR_GRAVE:   sprite = sprites[ENT_GRAVE];  break;
            case ACTOR_EXPLO:   sprite = sprites[ENT_EXPLO];  break;
            case ACTOR_EMPTY:   sprite = sprites[ENT_GRAVE];  break;
            default:            sprite = sprites[ENT_ERROR];  break;
        }
        
        if (sprite == sprites[ENT_ERROR])
        {
            i++;
            continue;
        }
        
        draw_sprite_tr(g, g->Actors[i].x * TILE_WIDTH,
                       g->Actors[i].y * TILE_HEIGHT,
                       sprite);                       
        i++;
    }
}

void render_menu()
{
    draw_big(0, 0, 320, 200, splash_screen); // change to menu gfx or text later
}

void render_end()
{
    fill_screen(0);
    render_text(122, 96, "YOU WIN!", 14);
}

void start_screen()
{
    fill_screen(0);
    
    typewriter(102, 96, "GET PSYCHED!", 1);
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

void render(struct GameData* g)
{        
    // in case playing, just died, or exited
    // draw play field and objects
    if (g->game_state == GAME_INGAME ||
        g->game_state == GAME_OVER ||
        g->game_state == GAME_WIN)
    {
        render_actors(g);
    }
    // if menu ...
    else if (g->game_state == GAME_MENU)
    {
        render_menu();
        render_text(200, 1, "VER. 0.0010A", 0);
    }
    // if end ...
    else if (g->game_state == GAME_END)
    {
        render_end();
    }
    memcpy(VGA,screen_buf,SCREEN_SIZE);
}
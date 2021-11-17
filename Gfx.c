#include "Gfx.h"
#include "Shared.h"

// globals
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t far screen_buf [64000]; // double buffer
int render_offset_x = 0;
int render_offset_y = 0;

union REGS regs;

extern struct GameData g;

// reserve memory for sprites
uint8_t far splash_screen   [64000];
uint8_t brick_wall      [TILE_AREA];
uint8_t player_sprite   [TILE_AREA];
uint8_t guard_sprite    [TILE_AREA];
uint8_t key_sprite      [TILE_AREA];
uint8_t floor_sprite    [TILE_AREA];
uint8_t door_c_sprite   [TILE_AREA];
uint8_t door_o_sprite   [TILE_AREA];
uint8_t exit_sprite     [TILE_AREA];
uint8_t error_sprite    [TILE_AREA];
uint8_t mine_sprite     [TILE_AREA];
uint8_t expl_sprite     [TILE_AREA];
uint8_t grave_sprite    [TILE_AREA];
uint8_t shad_in_cor     [TILE_AREA];
uint8_t shad_horz       [TILE_AREA];
uint8_t shad_vert       [TILE_AREA];
uint8_t shad_out_cor    [TILE_AREA];
uint8_t shad_mine       [TILE_AREA];
uint8_t shad_key        [TILE_AREA];
uint8_t alphabet        [4240];

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

void load_sprite(char* filename, uint8_t* source_data, uint16_t data_size)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "rb");
    fread(source_data, 1, data_size, file_ptr);
    fclose(file_ptr);
}

void load_graphics()
{
    load_sprite("GFX/SPLASH.7UP", splash_screen, 64000);
    load_sprite("GFX/FLOOR.7UP", floor_sprite, 64);
    load_sprite("GFX/BRICKS.7UP", brick_wall, 64);
    load_sprite("GFX/PLAYER.7UP", player_sprite, 64);
    load_sprite("GFX/GUARD.7UP", guard_sprite, 64);
    load_sprite("GFX/KEY.7UP", key_sprite, 64);
    load_sprite("GFX/DOORC.7UP", door_c_sprite, 64);
    load_sprite("GFX/DOORO.7UP", door_o_sprite, 64);
    load_sprite("GFX/EXIT.7UP", exit_sprite, 64);
    load_sprite("GFX/ERROR.7UP", error_sprite, 64);
    load_sprite("GFX/MINE.7UP", mine_sprite, 64);
    load_sprite("GFX/EXPLO.7UP", expl_sprite, 64);
    load_sprite("GFX/GRAVE.7UP", grave_sprite, 64);
    load_sprite("GFX/SHAD_IC.7UP", shad_in_cor, 64);
    load_sprite("GFX/SHAD_H.7UP", shad_horz, 64);
    load_sprite("GFX/SHAD_V.7UP", shad_vert, 64);
    load_sprite("GFX/SHAD_OC.7UP", shad_out_cor, 64);
    load_sprite("GFX/SHAD_M.7UP", shad_mine, 64);
    load_sprite("GFX/SHAD_K.7UP", shad_key, 64);
    load_sprite("GFX/FONT.7UP", alphabet, 4240);
}

void draw_sprite(int x, int y, uint8_t* sprite)
{
    int index_x = 0;
    int index_y = 0;
    int i = 0;

    x += render_offset_x;
    y += render_offset_y;

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

void draw_sprite_tr(int x, int y, uint8_t* sprite)
{
    uint8_t index_x = 0;
    uint8_t index_y = 0;
    uint8_t i = 0;
    
    x += render_offset_x;
    y += render_offset_y;

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
    
    if (TILE_AT(x-1, y-1) == TILE_WALL &&
        TILE_AT(x,   y-1) == TILE_WALL &&
        TILE_AT(x-1, y  ) == TILE_WALL)
        draw_sprite_tr(pixel_x, pixel_y, shad_in_cor);
        
    else if (TILE_AT(x, y-1) == TILE_WALL)
        draw_sprite_tr(pixel_x, pixel_y, shad_horz);
    
    else if (TILE_AT(x-1, y) == TILE_WALL)
        draw_sprite_tr(pixel_x, pixel_y, shad_vert);
    
    else if (TILE_AT(x-1, y-1) == TILE_WALL)
        draw_sprite_tr(pixel_x, pixel_y, shad_out_cor);
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

void render_maze(struct GameData* g)
{   
    int counter = 0;
    int y = 0;
    int x = 0;
    
    while (y < g->level_height)
    {
        while (x < g->level_width)
        {
            if (g->level_data[counter] == TILE_WALL)
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,brick_wall);
            else if (g->level_data[counter] == TILE_FLOOR)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
            }
            else if (g->level_data[counter] == ITEM_MINE && TILE_AT(x-1, y) != TILE_WALL)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
                draw_sprite_tr  (x * TILE_WIDTH,y * TILE_HEIGHT,mine_sprite);
            }
            else if (g->level_data[counter] == ITEM_MINE && TILE_AT(x-1, y) == TILE_WALL)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
                draw_sprite_tr(x * TILE_WIDTH,y * TILE_HEIGHT, shad_mine);
            }
            else if (g->level_data[counter] == ITEM_KEY && TILE_AT(x-1, y) != TILE_WALL)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
                draw_sprite_tr  (x * TILE_WIDTH,y * TILE_HEIGHT,key_sprite);
            }
            else if (g->level_data[counter] == ITEM_KEY && TILE_AT(x-1, y) == TILE_WALL)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
                draw_sprite_tr(x * TILE_WIDTH,y * TILE_HEIGHT, shad_key);
            }
            else if (g->level_data[counter] == TILE_DOOR_C)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
                draw_sprite_tr  (x * TILE_WIDTH,y * TILE_HEIGHT,door_c_sprite);
            }
            else if (g->level_data[counter] == TILE_DOOR_O)
            {
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
                draw_shadow(g, x, y);
                draw_sprite_tr  (x * TILE_WIDTH,y * TILE_HEIGHT,door_o_sprite);
            }
            else if (g->level_data[counter] == TILE_EXIT)
                draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT,exit_sprite);
                
            counter = counter++;
            x = x++;
        }
        y = y++;
        x = 0;
    }
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
            case ACTOR_PLAYER:  sprite = player_sprite; break;
            case ACTOR_GUARD:   sprite = guard_sprite;  break;
            default:            sprite = error_sprite;  break;
        }
        
        if (sprite == error_sprite)
        {
            i++;
            continue;
        }
        
        draw_sprite_tr(g->Actors[i].x * TILE_WIDTH,
                       g->Actors[i].y * TILE_HEIGHT,
                       sprite);
                       
        i++;
    }
    
    i = 0;
    while (i < g->actor_count)
    {
        
        switch (g->Actors[i].type)
        {
            case ACTOR_GRAVE:   sprite = grave_sprite;  break;
            case ACTOR_EXPLO:   sprite = expl_sprite;   break;
            default:            sprite = error_sprite;  break;
        }
        
        if (sprite == error_sprite)
        {
            i++;
            continue;
        }
        
        draw_sprite_tr(g->Actors[i].x * TILE_WIDTH,
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

void render(struct GameData* g)
{    
    int level_oversized = 0; //will come back to this later
    
    char lvl[12] = "";
    char keys[12] = "";
    char lives[12] = "";
    
    // in case playing, just died, or exited
    // draw play field and objects
    if (g->game_state == GAME_INGAME ||
        g->game_state == GAME_OVER ||
        g->game_state == GAME_WIN)
    {

        //centered on level
        if (level_oversized == 0)
        {
            render_offset_x = (SCREEN_WIDTH  - g->level_width *TILE_WIDTH) / 2;
            render_offset_y = (SCREEN_HEIGHT - g->level_height*TILE_HEIGHT) / 2;
        }
        
        // centered on player
        else
        {
            fill_screen(0);
            render_offset_x = SCREEN_WIDTH/2 - g->Actors[0].x*TILE_WIDTH;
            render_offset_y = SCREEN_HEIGHT/2 -g->Actors[0].y*TILE_HEIGHT;
        }

        render_maze(g);
        render_actors(g);
        
        sprintf(lvl, "LEVEL: %d", g->level_num);
        sprintf(keys, "KEYS: %d", g->keys_acquired);
        sprintf(lives, "LIVES: %d", g->player_lives);
        
        render_text(1, 1, lvl, 15);
        //render_text(120, 1, keys, 15);
        if (g->keys_acquired < 100)
            render_text(120, 1, keys, 15);
        else
            render_text(120, 1, "KEYS: 99", 15);
        
        if (g->player_lives > -1 && g->player_lives < 100)
            render_text(230, 1, lives, 15);
        else if (g->player_lives > 99)
            render_text(230, 1, "LIVES: 99", 15);
        else if (g->player_lives >= -1)
            render_text(230, 1, "LIVES: 0", 15);

        render_offset_x = 0;
        render_offset_y = 0;
    }
    // if menu ...
    else if (g->game_state == GAME_MENU)
    {
        render_menu();
        render_text(200, 1, "VER. 0.0003A", 0);
    }
    // if end ...
    else if (g->game_state == GAME_END)
    {
        render_end();
    }
    memcpy(VGA,screen_buf,SCREEN_SIZE);
}
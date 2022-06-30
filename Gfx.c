#include "Shared.h"
#include "Gfx.h"

// globals
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t far screen_buf [64000]; // double buffer
uint8_t minutes = 0;
uint16_t seconds = 0;

union REGS regs;

extern struct GameData g;
extern struct Settings opt;
extern Menu_t* current_menu;
extern Menu_t mainmenu;
extern Menu_t optionsmenu;
extern Projectile bullet_array[];
extern System_t System;

// reserve memory for sprites
Sprite sprites[NUM_SPRITES];

uint8_t alphabet [4240];
uint8_t temp_tile [64];

uint8_t* gfx_table[128];

void set_tile_gfx()
{
    gfx_table['W'] = sprites[TILE_WALL].pixels;
    gfx_table['-'] = sprites[TILE_FLOOR].pixels;
    gfx_table['e'] = sprites[TILE_EXIT].pixels;
    gfx_table['|'] = sprites[COMP_DOOR_C].pixels;
    gfx_table['_'] = sprites[TILE_DOOR_O].pixels;
    gfx_table['*'] = sprites[COMP_KEY].pixels;
    gfx_table['^'] = sprites[COMP_MINE].pixels;
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

void load_sprite(int sprite_id, char* filename)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "rb");
    fread(&sprites[sprite_id].w, 2, 1, file_ptr);
    fseek(file_ptr, 2, SEEK_SET);
    fread(&sprites[sprite_id].h, 2, 1, file_ptr);
    fseek(file_ptr, 4, SEEK_SET);
    fread(&sprites[sprite_id].num_frames, 2, 1, file_ptr);
    fseek(file_ptr, 6, SEEK_SET);
    fread(&sprites[sprite_id].transparency, 2, 1, file_ptr);
    fseek(file_ptr, 8, SEEK_SET);
    sprites[sprite_id].pixels = malloc(sprites[sprite_id].w * sprites[sprite_id].h);
    fread(sprites[sprite_id].pixels, 1, sprites[sprite_id].w * sprites[sprite_id].h, file_ptr);
    sprites[sprite_id].size = (sprites[sprite_id].w * sprites[sprite_id].h) / sprites[sprite_id].num_frames;
    fclose(file_ptr);
}

void load_tiles()
{
    load_sprite(TILE_FLOOR, "GFX/FLOOR.7UP");
    load_sprite(TILE_WALL, "GFX/BRICKS.7UP");
    load_sprite(TILE_EXIT, "GFX/EXIT.7UP");
    load_sprite(TILE_DOOR_C, "GFX/DOORC.7UP");
    load_sprite(TILE_DOOR_O, "GFX/DOORO.7UP");
    load_sprite(ITEM_KEY, "GFX/KEY.7UP");
    load_sprite(ITEM_MINE, "GFX/MINE.7UP");
    load_sprite(SPR_PLAYER, "GFX/PLAYER.7UP");
    load_sprite(SPR_GUARD, "GFX/GUARD.7UP");
    load_sprite(SPR_EXPLO, "GFX/EXPLO.7UP");
    load_sprite(SPR_BULLET, "GFX/BULLET.7UP");
    load_sprite(SPR_GRAVE, "GFX/GRAVE.7UP");
    load_sprite(SPR_CORPSE, "GFX/CORPSE.7UP");
    load_sprite(SPR_ERROR, "GFX/ERROR.7UP");
    load_sprite(SHAD_IN_COR, "GFX/SHAD_IC.7UP");
    load_sprite(SHAD_HORZ, "GFX/SHAD_H.7UP");
    load_sprite(SHAD_VERT, "GFX/SHAD_V.7UP");
    load_sprite(SHAD_OUT_COR, "GFX/SHAD_OC.7UP");
    load_sprite(SHAD_MINE, "GFX/SHAD_M.7UP");
    load_sprite(SHAD_KEY, "GFX/SHAD_K.7UP");
    // malloc memory
    sprites[COMP_DOOR_C].pixels = malloc(TILE_AREA);
    sprites[COMP_KEY].pixels = malloc(TILE_AREA);
    sprites[COMP_MINE].pixels = malloc(TILE_AREA);
}

void load_special_gfx()
{
    load_gfx("GFX/FONT.7UP", alphabet, 4240);
}

void composite_sprite(uint8_t* background, uint8_t* foreground, uint8_t* destination)
{
    draw_temp(background);
    draw_temp(foreground);
    memcpy(destination, temp_tile, TILE_AREA);
}

void create_composites()
{
    composite_sprite(sprites[TILE_FLOOR].pixels, sprites[TILE_DOOR_C].pixels, sprites[COMP_DOOR_C].pixels);
    composite_sprite(sprites[TILE_FLOOR].pixels, sprites[ITEM_KEY].pixels, sprites[COMP_KEY].pixels);
    composite_sprite(sprites[TILE_FLOOR].pixels, sprites[ITEM_MINE].pixels, sprites[COMP_MINE].pixels);
}

void draw_sprite(int x, int y, uint8_t* sprite)
{
    int index_x = 0;
    int index_y = 0;
    int i = 0;

    x += g.render_offset_x;
    y += g.render_offset_y;

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

void draw_sprite_tr(int x, int y, uint8_t* sprite)
{
    uint8_t index_x = 0;
    uint8_t index_y = 0;
    int pixel = 0;
    
    x += g.render_offset_x;
    y += g.render_offset_y;

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

void draw_shadow(int x, int y)
{    
    int pixel_x = x*TILE_WIDTH;
    int pixel_y = y*TILE_HEIGHT;

    if (TILE_AT(x, y) == MAP_FLOOR)
    {
        if (TILE_AT(x-1, y-1) == MAP_WALL &&
            TILE_AT(x,   y-1) == MAP_WALL &&
            TILE_AT(x-1, y  ) == MAP_WALL)
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_IN_COR].pixels);
            
        else if (TILE_AT(x, y-1) == MAP_WALL)
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_HORZ].pixels);
        
        else if (TILE_AT(x-1, y) == MAP_WALL)
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_VERT].pixels);
        
        else if (TILE_AT(x-1, y-1) == MAP_WALL)
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_OUT_COR].pixels);
    }

    else if (TILE_AT(x, y) == MAP_MINE)
    {
        if (TILE_AT(x-1, y) == MAP_WALL && TILE_AT(x, y-1) == MAP_WALL)
        {
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_MINE].pixels);
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_HORZ].pixels);
        }
        else if  (TILE_AT(x-1, y) == MAP_WALL)
            draw_sprite_tr (pixel_x, pixel_y, sprites[SHAD_MINE].pixels);

        else if (TILE_AT(x, y-1) == MAP_WALL)
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_HORZ].pixels);
    }

    else if (TILE_AT(x, y) == MAP_KEY)
    {

        if (TILE_AT(x-1, y) == MAP_WALL && TILE_AT(x, y-1) == MAP_WALL)
        {
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_KEY].pixels);
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_HORZ].pixels);
        }
        else if  (TILE_AT(x-1, y) == MAP_WALL)
            draw_sprite_tr (pixel_x, pixel_y, sprites[SHAD_KEY].pixels);

        else if (TILE_AT(x, y-1) == MAP_WALL)
            draw_sprite_tr(pixel_x, pixel_y, sprites[SHAD_HORZ].pixels);
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

void render_floor(int x, int y) // used to empty tiles after an item is picked up
{
    draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT, sprites[TILE_FLOOR].pixels);
    draw_shadow     (x, y);
}

void render_door(int x, int y) // used to draw an open door
{
    draw_sprite     (x * TILE_WIDTH,y * TILE_HEIGHT, sprites[TILE_FLOOR].pixels);
    draw_shadow     (x, y);
    draw_sprite_tr  (x * TILE_WIDTH,y * TILE_HEIGHT, sprites[TILE_DOOR_O].pixels);
}

void render_mine(int x, int y) // used to draw mines, duh
{
    draw_sprite (x * TILE_WIDTH,y * TILE_HEIGHT, sprites[COMP_MINE].pixels);
    draw_shadow (x, y);
}

void typewriter(int x, int y, char* string, uint8_t color)
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
        if (opt.sfx_on == TRUE)
            sound_typing();
        memcpy(VGA,screen_buf,SCREEN_SIZE);
        delay(1000);
        if (x > 311)
        {
            x = old_x;
            y = y + 10;
        }
    }
}

void render_stats()
{
    char lvl_str[12];
    char keys_str[12];
    char lives_str[12];

    sprintf(lvl_str, "LEVEL: %d", g.level_num);
    sprintf(keys_str, "KEYS: %d", g.keys_acquired);
    sprintf(lives_str, "LIVES: %d", g.player_lives);

    render_text(1, 1, lvl_str, 15);

    if (g.keys_acquired < 100)
        render_text(120, 1, keys_str, 15);
    else
        render_text(120, 1, "KEYS: 99", 15);
    
    if (g.player_lives > -1 && g.player_lives < 100)
        render_text(230, 1, lives_str, 15);
    else if (g.player_lives > 99)
        render_text(230, 1, "LIVES: 99", 15);
    else if (g.player_lives > -2)
        render_text(230, 1, "LIVES: 0", 15);
}

void render_maze()
{   
    int counter = 0;
    int y = 0;
    int x = 0;
    
    while (y < g.level_height)
    {
        while (x < g.level_width)
        {
            draw_sprite(x * TILE_WIDTH, y * TILE_HEIGHT, gfx_table[g.tile_data[counter]]);
            draw_shadow(x, y);
            counter++;
            x++;
        }
    y++;
    x = 0;
    }

    render_stats();
}

void anim_explosion()
{
    struct Actor p = g.Actors[0];
    uint8_t* pixels;

    p.sprite = sprites[SPR_EXPLO];
    pixels = p.sprite.pixels;

    SET_TILE(p.x, p.y, MAP_FLOOR); // "delete" mine
    render_floor(p.x, p.y); // "delete" player sprite

    for (p.sprite.frame = 0;p.sprite.frame < p.sprite.num_frames;p.sprite.frame++)
    {
        delay(150);
        draw_sprite_tr(p.x * TILE_WIDTH,
                       p.y * TILE_HEIGHT,
                       pixels + (p.sprite.frame * p.sprite.size));
        memcpy(VGA,screen_buf,SCREEN_SIZE);
    }
    
    for (p.sprite.frame = 2;p.sprite.frame > -1;p.sprite.frame--)
    {
        delay(150);
        render_floor(p.x, p.y);
        draw_sprite_tr(p.x * TILE_WIDTH,
                       p.y * TILE_HEIGHT,
                       pixels + (p.sprite.frame * p.sprite.size));
        memcpy(VGA,screen_buf,SCREEN_SIZE);
    }

    delay(150);
    render_floor(p.x, p.y);
    
    p.sprite = sprites[SPR_PLAYER];   
}

void render_prev_pro(Projectile* proj)
{
    if (TILE_AT(proj->old_x, proj->old_y) == MAP_FLOOR)
        render_floor(proj->old_x, proj->old_y);
    else if (TILE_AT(proj->old_x, proj->old_y) == MAP_DOOR_O)
        render_door(proj->old_x, proj->old_y);
    else if (TILE_AT(proj->old_x, proj->old_y) == MAP_MINE)
        render_mine(proj->old_x, proj->old_y);
}

void render_previous(struct Actor* actor)
{
    if (TILE_AT(actor->old_x, actor->old_y) == MAP_FLOOR)
        render_floor(actor->old_x, actor->old_y);
    else if (TILE_AT(actor->old_x, actor->old_y) == MAP_DOOR_O)
        render_door(actor->old_x, actor->old_y);
    else if (TILE_AT(actor->old_x, actor->old_y) == MAP_MINE)
        render_mine(actor->old_x, actor->old_y);
}

void render_actors()
{
    uint8_t* pixels;
    int i;
    
    i = 0;
    while (i < g.actor_count)
    {
        if (pixels == sprites[SPR_ERROR].pixels)
        {
            i++;
            continue;
        }

        // if actor has changed position, render an empty floor tile in the old location, unless it's a door frame or mine         
        if (g.Actors[i].x != g.Actors[i].old_x || g.Actors[i].y != g.Actors[i].old_y)
        {
            render_previous(&g.Actors[i]);
        }

        if (g.Actors[i].type != ACTOR_CORPSE)
        {
            pixels = g.Actors[i].sprite.pixels;

            g.Actors[i].sprite.frame++;

            if (g.Actors[i].sprite.frame >= g.Actors[i].sprite.num_frames)
                g.Actors[i].sprite.frame = 0;
            
            draw_sprite_tr(g.Actors[i].x * TILE_WIDTH,
                        g.Actors[i].y * TILE_HEIGHT,
                        pixels + (g.Actors[i].sprite.frame * g.Actors[i].sprite.size));
        }      
        i++;
    }
    
    i = 0;
    while (i < g.actor_count)
    {
        
        switch (g.Actors[i].type)
        {
            case ACTOR_GRAVE:   pixels = sprites[SPR_GRAVE].pixels;  break;
            case ACTOR_EMPTY:   pixels = sprites[SPR_GRAVE].pixels;  break;
            case ACTOR_CORPSE:  pixels = sprites[SPR_CORPSE].pixels;  break;
            default:            pixels = sprites[SPR_ERROR].pixels;  break;
        }
        
        if (pixels == sprites[SPR_ERROR].pixels)
        {
            i++;
            continue;
        }
        
        draw_sprite_tr(g.Actors[i].x * TILE_WIDTH,
                       g.Actors[i].y * TILE_HEIGHT,
                       pixels);                       
        i++;
    }
}

void render_bullets()
{
    uint8_t* pixels;
    int i = 0;

    while (i < MAX_BULLETS)
    {
        if (bullet_array[i].active == TRUE)
        {
            bullet_array[i].sprite = sprites[SPR_BULLET];
            
            pixels = bullet_array[i].sprite.pixels;

            if (bullet_array[i].direction == UP)
                bullet_array[i].sprite.frame = 0;
            else if (bullet_array[i].direction == RIGHT)
                bullet_array[i].sprite.frame = 1;
            else if (bullet_array[i].direction == DOWN)
                bullet_array[i].sprite.frame = 2;
            else if (bullet_array[i].direction == LEFT)
                bullet_array[i].sprite.frame = 3;

            draw_sprite_tr(bullet_array[i].loc_x * TILE_WIDTH,
                bullet_array[i].loc_y * TILE_HEIGHT,
                pixels + (bullet_array[i].sprite.frame * bullet_array[i].sprite.size));
        }
        render_prev_pro(&bullet_array[i]);
        i++;
    }
}

void render_menu_text()
{
    int i = 0;
    int y = current_menu->start_y;

    if (current_menu == &optionsmenu)
    {
        for(i = 0; i < current_menu->num_selections; i++)
        {
            render_text(current_menu->cursor_x + 20, y, current_menu->options[i].text, 15);
            y += current_menu->cursor_spacing;
        }
        if (opt.sfx_on == TRUE)
            render_text(191, 65, "ON", 15);
        else if (opt.sfx_on == FALSE)
            render_text(191, 65, "OFF", 15);
        if (opt.music_on == TRUE)
            render_text(191, 105, "ON", 15);
        else if (opt.music_on == FALSE)
            render_text(191, 105, "OFF", 15);
    }

    else
    {
        for(i = 0; i < current_menu->num_selections; i++)
        {
            render_text(current_menu->cursor_x + 20, y, current_menu->options[i].text, 15);
            y += current_menu->cursor_spacing;
        }
    }

    render_text(230, 185, "V.0.0023A", 0);
}

void draw_help_contents()
{
    // reset offsets in case they have been changed in-game
    g.render_offset_x = 0;
    g.render_offset_y = 0;
    
    draw_sprite_tr(48, 64, sprites[SPR_PLAYER].pixels);
    draw_sprite(48, 76, sprites[TILE_WALL].pixels);
    draw_sprite_tr(48, 88, sprites[SPR_GUARD].pixels);
    draw_sprite_tr(48, 100, sprites[ITEM_MINE].pixels);
    draw_sprite_tr(48, 112, sprites[TILE_DOOR_C].pixels);
    draw_sprite_tr(48, 124, sprites[ITEM_KEY].pixels);
    draw_sprite(48, 135, sprites[TILE_EXIT].pixels);
}

void change_menu()
{
    load_gfx(current_menu->bgfilename, screen_buf, 64000);

    render_menu_text();
}

void render_cursor(int x, int y)
{
    render_text(x, y, "->", 15);
}

void render_menu()
{
    render_cursor(current_menu->cursor_x, current_menu->cursor_y);
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

void render_time()
{
    char time_str[50];

    sprintf(time_str, "TIME: %ld:%ld", System.seconds/60, System.seconds%60);
        
    draw_rectangle(270, 185, 50, 8, 0);
    render_text(210, 185, time_str, 15);
}

void render()
{        
    // in case playing, just died, or exited
    // draw play field and objects
    if (g.game_state == GAME_INGAME ||
        g.game_state == GAME_OVER ||
        g.game_state == GAME_WIN)
    {
        render_time();
        render_bullets();
        render_actors();
        if (g.Actors[0].type == ACTOR_EXPLO)
            anim_explosion();
    }
    // if menu ...
    else if (g.game_state == GAME_MENU)
    {
        render_menu();
    }
    // if end ...
    else if (g.game_state == GAME_END)
    {
        render_end();
    }
    
    vretrace();
    memcpy(VGA,screen_buf,SCREEN_SIZE);
}
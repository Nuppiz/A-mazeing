#include "Shared.h"
#include "Gameplay.h"

extern struct GameData g;

void level_loader(struct GameData* g)
{
    FILE* level_file;
    char buffer[100];
    char filename[13];
    char c;
    int i = 0;
    
    /* build filename and load it */
    sprintf(filename, "LEVELS/level%d.txt", g->level_num);
    level_file = fopen(filename, "r");
    
    if (level_file == NULL)
    {
        set_mode(TEXT_MODE);
        printf("Unable to open file: %s\n", filename);
        printf("Please check the file actually exists!\n");
        deinit_keyboard();
        exit(EXIT_FAILURE);
    }
    
    g->actor_count = 0;
    g->keys_acquired = 0;
    _fmemset(g->object_map, ACTOR_EMPTY, MAPSIZE);
    
    while ((c = fgetc(level_file)) != EOF)
    {
        if (c == '$')
        {
            fscanf(level_file, "%s", buffer);
            if (strcmp(buffer, "leveldim") == 0)
            {
                fscanf(level_file, "%d %d", &g->level_width, &g->level_height);
            }
            else if (strcmp(buffer, "player") == 0)
            {
                fscanf(level_file, "%d %d %d %d",
                    &g->Actors[0].x,
                    &g->Actors[0].y,
                    &g->Actors[0].x_vel,
                    &g->Actors[0].y_vel
                );
                
                g->Actors[0].type = ACTOR_PLAYER;
                g->object_map[g->Actors[0].y * g->level_width + g->Actors[0].x] = ACTOR_PLAYER;
                g->Actors[0].old_x = g->Actors[0].x;
                g->Actors[0].old_y = g->Actors[0].y;

                g->actor_count++;
            }
            else if (strcmp(buffer, "guard") == 0)
            {
                if (g->actor_count < MAX_ACTORS)
                {
                    fscanf(level_file, "%d %d %d %d",
                        &g->Actors[g->actor_count].x,
                        &g->Actors[g->actor_count].y,
                        &g->Actors[g->actor_count].x_vel,
                        &g->Actors[g->actor_count].y_vel
                    );
                        
                    g->Actors[g->actor_count].type = ACTOR_GUARD;
                    g->object_map[g->Actors[g->actor_count].y * g->level_width + g->Actors[g->actor_count].x] = ACTOR_GUARD;
                    g->Actors[g->actor_count].old_x = g->Actors[g->actor_count].x;
                    g->Actors[g->actor_count].old_y = g->Actors[g->actor_count].y;
                    g->actor_count++;
                }
            }
            else if (strcmp(buffer, "tiledata") == 0)
            {
                while ((c = fgetc(level_file)) != EOF)
                {
                    if (c != '\n')
                    {
                        g->tile_data[i] = c;
                        i++;
                    }
                    else
                        putchar('\n');
                }
            }
        }
    }
    fclose(level_file);
    //centered on level
    if (g->level_width <= 40 && g->level_height <= 25)
    {
        g->render_offset_x = (SCREEN_WIDTH  - g->level_width *TILE_WIDTH) / 2;
        g->render_offset_y = (SCREEN_HEIGHT - g->level_height*TILE_HEIGHT) / 2;
    }
    
    // centered on player
    else
    {
        fill_screen(0);
        g->render_offset_x = SCREEN_WIDTH/2 - g->Actors[0].x*TILE_WIDTH;
        g->render_offset_y = SCREEN_HEIGHT/2 -g->Actors[0].y*TILE_HEIGHT;
    }
    render_maze(g);
}

void player_death(struct GameData* g)
{
    struct Actor* p = &g->Actors[0];
    
    render(g);
    sound_death();
    delay(1000);
    
    p->type = ACTOR_PLAYER;
    _fmemset(g->object_map, ACTOR_EMPTY, MAPSIZE);
    fill_screen(0);
    level_loader(g);
}

void add_key(struct GameData* g)
{
    g->keys_acquired++;
    sound_key();
    draw_rectangle(180, 1, 18, 8, 0);
    render_maze(g);
}

void remove_key(struct GameData* g)
{
    g->keys_acquired--;
    draw_rectangle(180, 1, 18, 8, 0);
    render_maze(g);
}

void add_life(struct GameData* g)
{
    g->player_lives++;
    draw_rectangle(300, 1, 18, 8, 0);
    render_maze(g);
}

void remove_life(struct GameData* g)
{
    g->player_lives--;
    draw_rectangle(300, 1, 18, 8, 0);
    render_maze(g);
}

void level_skip(struct GameData* g)
{
    g->game_state = GAME_WIN;
}

void move_actors(struct GameData* g)
{
    int new_x, new_y;
    int i = 0;
    while (i < g->actor_count)
    {
        struct Actor* p_actor = &g->Actors[i++];
        struct Actor* player = &g->Actors[0];

        // set coordinates for new square to (potentially) move into
        // based on adding velocity to current position...
        p_actor->old_x = p_actor->x;
        p_actor->old_y = p_actor->y;
        new_x = p_actor->x + p_actor->x_vel;
        new_y = p_actor->y + p_actor->y_vel;
        
        // if it's NOT a wall, then move into the square and clear the previous grid slot
        if (TILE_AT(new_x, new_y) != TILE_WALL && p_actor->type == ACTOR_PLAYER)
        {
            p_actor->x = new_x;
            p_actor->y = new_y;

            if (g->object_map[new_y * g->level_width + new_x] == ACTOR_GUARD)
            {
            player->type = ACTOR_GRAVE;
            p_actor->type = ACTOR_GRAVE;
            remove_life(g);
            
                if (g->player_lives < 0)
                    g->game_state = GAME_OVER;
                else
                {
                    player_death(g);
                }
            break;
            }
            else
            {
                g->object_map[p_actor->y * g->level_width + p_actor->x] = p_actor->type;
                g->object_map[p_actor->old_y * g->level_width + p_actor->old_x] = ACTOR_EMPTY;
            }
        }

        else if (TILE_AT(new_x, new_y) != TILE_WALL && p_actor->type == ACTOR_GUARD)
        {
            p_actor->x = new_x;
            p_actor->y = new_y;

            if (g->object_map[new_y * g->level_width + new_x] == ACTOR_PLAYER)
            {
            player->type = ACTOR_GRAVE;
            p_actor->type = ACTOR_GRAVE;
            remove_life(g);
            
                if (g->player_lives < 0)
                    g->game_state = GAME_OVER;
                else
                {
                    player_death(g);
                }
            break;
            }
            else
            {
                g->object_map[p_actor->y * g->level_width + p_actor->x] = p_actor->type;
                g->object_map[p_actor->old_y * g->level_width + p_actor->old_x] = ACTOR_EMPTY;
            }
        }
        // if colliding into a wall and is NOT player, reverse dir
        else if (p_actor->type != ACTOR_PLAYER)
        {
            p_actor->x_vel *= -1;
            p_actor->y_vel *= -1;
        }
    }
}

void player_hit_detect(struct GameData* g)
{
    struct Actor* p = &g->Actors[0];
    int i = 1; // ignore player
    
    // step on mine
    if (ACTOR_ON_TILE(p, ITEM_MINE))
    {
        p->type = ACTOR_EXPLO;
        remove_life(g);
        
        if (g->player_lives < 0)
            g->game_state = GAME_OVER;
        else
            player_death(g);
    }
    // collect key
    else if (ACTOR_ON_TILE(p, ITEM_KEY))
    {
        add_key(g);
        SET_TILE(p->x, p->y, TILE_FLOOR);
        render_floor(g, p->x, p->y);
    }
    // try to open door
    else if (ACTOR_ON_TILE(p, TILE_DOOR_C))
    {
        // no keys; door remains closed, so cancel movement
        if (g->keys_acquired < 1)
        {
            p->x -= p->x_vel;
            p->y -= p->y_vel;
        }
        // otherwise, we remain in the door tile and change tile to DOOR_O
        else
        {
            remove_key(g);
            SET_TILE(p->x, p->y, TILE_DOOR_O);
            render_door(g, p->x, p->y);
        }
    }
    // exit and win the level
    else if (ACTOR_ON_TILE(p, TILE_EXIT))
        g->game_state = GAME_WIN;

    //see if we hit a guard
    while (i < g->actor_count)
    {
        struct Actor* enemy = &g->Actors[i++];
        
        if (g->object_map[p->y * g->level_width + p->x] == ACTOR_GUARD)
        {
            p->type = ACTOR_GRAVE;
            enemy->type = ACTOR_GRAVE;
            remove_life(g);
            
            if (g->player_lives < 0)
                g->game_state = GAME_OVER;
            else
            {
                player_death(g);
            }
            break;
        }
    }
}

void next_level(struct GameData* g)
{
    g->game_state = GAME_INGAME;
    g->level_num++;
    fill_screen(0);
    level_loader(g);
}

void check_state(struct GameData* g)
{
    if (g->game_state == GAME_OVER)
    {
        render_actors(g);
        gameover_screen();
        sound_gameover();
        delay(1000);
        g->game_state = GAME_MENU;
    }
    else if (g->game_state == GAME_WIN)
    {
        end_song();
        delay(100);
        
        if (g->level_num == LAST_LEVEL)
            g->game_state = GAME_END;
        else
            next_level(g);
    }
    else if (g->game_state == GAME_END)
    {
        end_song();
        delay(100);
        g->game_state = GAME_MENU;
    }
}

void game_logic(struct GameData* g)
{
    check_state(g);
    if (g->game_state == GAME_INGAME)
    {
        move_actors(g);
        player_hit_detect(g);
    }
}
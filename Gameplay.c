#include "Shared.h"
#include "Gameplay.h"

extern struct GameData g;
extern struct Options opt;

void player_death(struct GameData* g, struct Options* opt)
{
    struct Actor* p = &g->Actors[0];
    
    fill_screen(0);
    render_maze(g);
    render_actors(g);
    render(g);

    if (opt->sfx_on == 1)
        sound_death();
    delay(1000);
    
    p->type = ACTOR_PLAYER;
    _fmemset(g->object_map, ACTOR_EMPTY, MAPSIZE);
    fill_screen(0);
    level_loader(g);
}

void add_key(struct GameData* g, struct Options* opt)
{
    g->keys_acquired++;
    if (opt->sfx_on == 1)
        sound_key();
    draw_rectangle(180, 1, 18, 8, 0);
    if (g->level_height > 23)
        render_maze(g);
    else
        render_stats(g);
}

void remove_key(struct GameData* g)
{
    g->keys_acquired--;
    draw_rectangle(180, 1, 18, 8, 0);
    if (g->level_height > 23)
        render_maze(g);
    else
        render_stats(g);
}

void add_life(struct GameData* g)
{
    g->player_lives++;
    draw_rectangle(300, 1, 18, 8, 0);
    if (g->level_height > 23)
        render_maze(g);
    else
        render_stats(g);
}

void remove_life(struct GameData* g)
{
    g->player_lives--;
    draw_rectangle(300, 1, 18, 8, 0);
    if (g->level_height > 23)
        render_maze(g);
    else
        render_stats(g);
}

void level_skip(struct GameData* g)
{
    g->game_state = GAME_WIN;
}

void suicide(struct GameData* g)
{
    g->game_state = GAME_OVER;
}

void move_actors(struct GameData* g, struct Options* opt)
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
        if (TILE_AT(new_x, new_y) != MAP_WALL && p_actor->type == ACTOR_PLAYER)
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
                        player_death(g, opt);
                break;
            }
            else
            {
                g->object_map[p_actor->y * g->level_width + p_actor->x] = p_actor->type;
                g->object_map[p_actor->old_y * g->level_width + p_actor->old_x] = ACTOR_EMPTY;
            }
        }

        else if (TILE_AT(new_x, new_y) != MAP_WALL && p_actor->type == ACTOR_GUARD)
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
                        player_death(g, opt);
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

void player_hit_detect(struct GameData* g, struct Options* opt)
{
    struct Actor* p = &g->Actors[0];
    
    // step on mine
    if (ACTOR_ON_TILE(p, MAP_MINE))
    {
        p->type = ACTOR_EXPLO;
        remove_life(g);
        
        if (g->player_lives < 0)
            g->game_state = GAME_OVER;
        else
            player_death(g, opt);
    }
    // collect key
    else if (ACTOR_ON_TILE(p, MAP_KEY))
    {
        add_key(g, opt);
        SET_TILE(p->x, p->y, MAP_FLOOR);
        render_floor(g, p->x, p->y);
    }
    // try to open door
    else if (ACTOR_ON_TILE(p, MAP_DOOR_C))
    {
        // no keys; door remains closed, so cancel movement
        if (g->keys_acquired < 1)
        {
            p->x -= p->x_vel;
            p->y -= p->y_vel;
            if (opt->sfx_on == 1)
                sound_door_c();
        }
        // otherwise, we remain in the door tile and change tile to DOOR_O
        else
        {
            remove_key(g);
            SET_TILE(p->x, p->y, MAP_DOOR_O);
            render_door(g, p->x, p->y);
            if (opt->sfx_on == 1)
                sound_door_o();
        }
    }
    // exit and win the level
    else if (ACTOR_ON_TILE(p, MAP_EXIT))
        g->game_state = GAME_WIN;
}

void check_state(struct GameData* g, struct Options* opt)
{
    if (g->game_state == GAME_OVER)
    {
        fill_screen(0);
        render_maze(g);
        render(g);
        gameover_screen();
        if (opt->sfx_on == 1)
        {
            sound_gameover();
            delay(1000);
        }
        else
            delay(3000);
        g->game_state = GAME_MENU;
    }
    else if (g->game_state == GAME_WIN)
    {
        if (opt->music_on == 1)
        {
            end_song();
            delay(100);
        }
        else
            delay(1500);
        
        if (g->level_num == LAST_LEVEL)
            g->game_state = GAME_END;
        else
            next_level(g);
    }
    else if (g->game_state == GAME_END)
    {
        if (opt->music_on == 1)
        {
            end_song();
            delay(100);
        }
        else
            delay(1500);
        
        g->game_state = GAME_MENU;
    }
}

void game_logic(struct GameData* g, struct Options* opt)
{
    check_state(g, opt);
    if (g->game_state == GAME_INGAME)
    {
        move_actors(g, opt);
        player_hit_detect(g, opt);
    }
}
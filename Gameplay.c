#include "Gameplay.h"
#include "Shared.h"

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
        printf("Unable to open file: %s\n", filename);
        printf("Please check the file actually exists\n");
        exit(EXIT_FAILURE);
    }
    
    g->actor_count = 0;
    g->keys_acquired = 0;
    
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
                    g->actor_count++;
                }
            }
            else if (strcmp(buffer, "tiledata") == 0)
            {
                while ((c = fgetc(level_file)) != EOF)
                {
                    if (c != '\n')
                    {
                        g->level_data[i] = c;
                        i++;
                    }
                    else
                        putchar('\n');
                }
            }
        }
    }
    fclose(level_file);
}

void player_death(struct GameData* g)
{
    struct Actor* p = &g->Actors[0];
    
    render(g);
    sound_death();
    delay(1000);
    
    p->type = ACTOR_PLAYER;
    fill_screen(0);
    level_loader(g);
}

void move_actors(struct GameData* g)
{
    int new_x, new_y;
    int i = 0;
    while (i < g->actor_count)
    {
        struct Actor* p_actor = &g->Actors[i++];
        
        // set coordinates for new square to (potentially) move into
        // based on adding velocity to current position...
        new_x = p_actor->x + p_actor->x_vel;
        new_y = p_actor->y + p_actor->y_vel;
        
        // if it's NOT a wall, then move into the square
        if (TILE_AT(new_x, new_y) != TILE_WALL)
        {
            p_actor->x = new_x;
            p_actor->y = new_y;
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
        g->player_lives--;
        
        if (g->player_lives < 0)
            g->game_state = GAME_OVER;
        else
            player_death(g);
    }
    // collect key
    else if (ACTOR_ON_TILE(p, ITEM_KEY))
    {
        g->keys_acquired++;
        SET_TILE(p->x, p->y, TILE_FLOOR);
        sound_key();
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
            SET_TILE(p->x, p->y, TILE_DOOR_O);
            g->keys_acquired--;
        }
    }
    // exit and win the level
    else if (ACTOR_ON_TILE(p, TILE_EXIT))
        g->game_state = GAME_WIN;

    // see if we hit a guard
    while (i < g->actor_count)
    {
        struct Actor* enemy = &g->Actors[i++];
        
        if (p->x == enemy->x && p->y == enemy->y)
        {
            p->type = ACTOR_GRAVE;
            enemy->type = ACTOR_GRAVE;
            g->player_lives--;
            
            if (g->player_lives < 1)
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
    level_loader(g);
}

void check_state(struct GameData* g)
{
    if (g->game_state == GAME_OVER)
    {
        set_cursor(15, 11);
        printf("GAME OVER!");
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
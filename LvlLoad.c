#include "Shared.h"
#include "LvlLoad.h"

extern struct GameData g;
extern struct Sprite player_sprite;
extern struct Sprite guard_sprite;

extern int16_t song_i;

void level_loader()
{
    FILE* level_file;
    char buffer[100];
    char filename[13];
    char c;
    int i = 0;
    
    /* build filename and load it */
    sprintf(filename, "LEVELS/level%d.txt", g.level_num);
    level_file = fopen(filename, "r");
    
    if (level_file == NULL)
    {
        set_mode(TEXT_MODE);
        printf("Unable to open file: %s\n", filename);
        printf("Please check the file actually exists!\n");
        deinit_keyboard();
        exit(EXIT_FAILURE);
    }
    
    g.actor_count = 0;
    g.keys_acquired = 0;
    _fmemset(g.object_map, ACTOR_EMPTY, MAPSIZE);
    
    while ((c = fgetc(level_file)) != EOF)
    {
        if (c == '$')
        {
            fscanf(level_file, "%s", buffer);
            if (strcmp(buffer, "leveldim") == 0)
            {
                fscanf(level_file, "%d %d", &g.level_width, &g.level_height);
            }
            else if (strcmp(buffer, "player") == 0)
            {
                fscanf(level_file, "%d %d %d %d",
                    &g.Actors[0].x,
                    &g.Actors[0].y,
                    &g.Actors[0].x_vel,
                    &g.Actors[0].y_vel
                );
                
                g.Actors[0].type = ACTOR_PLAYER;
                g.object_map[g.Actors[0].y * g.level_width + g.Actors[0].x] = ACTOR_PLAYER;
                g.Actors[0].old_x = g.Actors[0].x;
                g.Actors[0].old_y = g.Actors[0].y;

                g.Actors[0].sprite = player_sprite;

                g.actor_count++;
            }
            else if (strcmp(buffer, "guard") == 0)
            {
                if (g.actor_count < MAX_ACTORS)
                {
                    fscanf(level_file, "%d %d %d %d",
                        &g.Actors[g.actor_count].x,
                        &g.Actors[g.actor_count].y,
                        &g.Actors[g.actor_count].x_vel,
                        &g.Actors[g.actor_count].y_vel
                    );
                        
                    g.Actors[g.actor_count].type = ACTOR_GUARD;
                    g.object_map[g.Actors[g.actor_count].y * g.level_width + g.Actors[g.actor_count].x] = ACTOR_GUARD;
                    g.Actors[g.actor_count].old_x = g.Actors[g.actor_count].x;
                    g.Actors[g.actor_count].old_y = g.Actors[g.actor_count].y;
                    g.Actors[g.actor_count].sprite = guard_sprite;
                    g.actor_count++;
                }
            }
            else if (strcmp(buffer, "tiledata") == 0)
            {
                while ((c = fgetc(level_file)) != EOF)
                {
                    if (c != '\n')
                    {
                        g.tile_data[i] = c;
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
    if (g.level_width <= 40 && g.level_height <= 25)
    {
        g.render_offset_x = (SCREEN_WIDTH  - g.level_width *TILE_WIDTH) / 2;
        g.render_offset_y = (SCREEN_HEIGHT - g.level_height*TILE_HEIGHT) / 2;
    }
    
    // centered on player
    else
    {
        fill_screen(0);
        g.render_offset_x = SCREEN_WIDTH/2 - g.Actors[0].x*TILE_WIDTH;
        g.render_offset_y = SCREEN_HEIGHT/2 -g.Actors[0].y*TILE_HEIGHT;
    }
    render_maze(g);
}

void next_level()
{
    g.game_state = GAME_INGAME;
    g.level_num++;
    fill_screen(0);
    level_loader(g);
    music_track_select(g);
    song_i = 0;
}
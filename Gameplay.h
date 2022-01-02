#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#define ACTOR_ON_TILE(a, tile)  g->tile_data[a->y * g->level_width + a->x] == tile

void start_game();
void add_key();
void remove_key();
void add_life();
void remove_life();
void level_skip();
void suicide();
void move_actors();
void player_hit_detect();
void check_state();
void game_logic();

#endif
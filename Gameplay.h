#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define ACTOR_ON_TILE(a, tile)  g->tile_data[a->y * g->level_width + a->x] == tile

void level_loader();
void player_death();
void add_key();
void remove_key();
void add_life();
void remove_life();
void level_skip();
void move_actors();
void player_hit_detect();
void next_level();
void check_state();
void game_logic();

#endif
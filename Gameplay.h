#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define LAST_LEVEL          3

#define ACTOR_ON_TILE(a, tile)  g->level_data[a->y * g->level_width + a->x] == tile

void level_loader();
void player_death();
void move_actors();
void player_hit_detect();
void next_level();
void check_state();
void game_logic();

#endif
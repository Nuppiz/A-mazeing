#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dos.h>

#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define GAME_MENU			0
#define GAME_INGAME			1
#define GAME_OVER 			2
#define GAME_WIN 			3
#define GAME_END			4
#define GAME_DEATH			5

#define MAX_ACTORS			10

#define ACTOR_PLAYER		0
#define ACTOR_GUARD 		1
#define ACTOR_GRAVE			2	
#define ACTOR_EXPLO			3

#define TILE_WALL			87
#define TILE_FLOOR			45
#define TILE_DOOR_C			124
#define TILE_DOOR_O			250
#define TILE_EXIT			101
#define ITEM_KEY			42
#define ITEM_MINE			94

//macros
#define TILE_AT(x, y)           g->level_data[(y) * g->level_width + (x)]
#define SET_TILE(x, y, tile)   	TILE_AT(x, y) = tile

struct Actor
{
	int8_t x, y, x_vel, y_vel, type;
	int8_t direction;
};

struct GameData
{	
	int game_running;
	int game_state;
	/*int new_game;*/

	uint8_t level_data[1000];	
	int level_num;
	int level_width;
	int level_height;
	int8_t player_lives;	
	int keys_acquired;
	int actor_count;	

	struct Actor Actors[MAX_ACTORS];
};
#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dos.h>
#include "Watcom.h"

#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels */
#define SCREEN_HEIGHT       200       /* height in pixels */

#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

#define FALSE               0
#define TRUE                1

#define TILE_WIDTH          8
#define TILE_HEIGHT         8

#define GAME_MENU           0
#define GAME_INGAME         1
#define GAME_OVER           2
#define GAME_WIN            3
#define GAME_END            4

#define MAX_ACTORS          10
#define LEVEL_SIZE          level_width * level_height

#define ACTOR_EMPTY         0
#define ACTOR_PLAYER        1
#define ACTOR_GUARD         2
#define ACTOR_GRAVE         3   
#define ACTOR_EXPLO         4
#define ACTOR_CORPSE        5
#define OBJ_BULLET          6

#define WEAP_NONE           0
#define WEAP_PISTOL         1

#define AMMO_BULLET         1

#define MAX_BULLETS         10

#define UP                  1
#define RIGHT               2
#define DOWN                3
#define LEFT                4

#define MAPSIZE             1000

#define LAST_LEVEL          3

#define MAP_WALL            87
#define MAP_FLOOR           45
#define MAP_DOOR_C          124
#define MAP_DOOR_O          250
#define MAP_EXIT            101
#define MAP_KEY             42
#define MAP_MINE            94

#define LOGIC_INTERVAL      100
#define RENDER_INTERVAL     10
#define AUDIO_INTERVAL      750
#define MOVEMENT_INTERVAL   500

#define FRAME_RATE          10
#define TICK_RATE           10

#define CONTROL_8253        0x43
#define CONTROL_WORD        0x3C
#define COUNTER_0           0x40
#define TIME_KEEPER_INT     0x1C 
#define TIMER_18HZ          0xFFFF
#define TIMER_1000HZ        1193

#define LOW_BYTE(n)         (n & 0x00ff)
#define HIGH_BYTE(n)        ((n>>8) & 0x00ff)

#define SCREEN_SIZE         64000

#define TICKS               (*(volatile unsigned long far *)(0x0040006CL))

// macros
#define TILE_AT(x, y)           g.tile_data[(y) * g.level_width + (x)]
#define SET_TILE(x, y, tile)    TILE_AT(x, y) = tile

// structs

typedef struct
{
  uint8_t* pixels;
  uint16_t w, h, size, num_frames, transparency;
  int16_t frame;
} Sprite;

struct Actor
{
  int8_t x, y, x_vel, y_vel, type, old_x, old_y;
  int8_t direction, aim, health;
  Sprite sprite;
};

struct GameData
{   
  int game_running;
  int game_state;

  int level_num;
  int level_width;
  int level_height;
  int render_offset_x;
  int render_offset_y;
  uint8_t tile_data[MAPSIZE];
  uint8_t object_map[MAPSIZE];
  int8_t player_lives;
  int keys_acquired;
  int actor_count;

  struct Actor Actors[MAX_ACTORS];
};

struct Settings
{
  uint8_t debugmode;
  uint8_t sfx_on;
  uint8_t music_on;
  uint8_t up_movement;
  uint8_t left_movement;
  uint8_t down_movement;
  uint8_t right_movement;
};

typedef void (*fnp)();

typedef struct
{
  char* text;
  fnp action;
} Option_t;

typedef struct
{
  char* bgfilename;
  int num_selections;
  int cursor_loc;
  int start_y;
  int cursor_y;
  int cursor_x;
  int cursor_spacing;
  Option_t* options;
} Menu_t;

typedef struct
{
  uint8_t damage;
  uint8_t ammo_type;
  uint8_t firerate;
} Weapon;

typedef struct
{
  uint8_t active;
  uint8_t type;
  int loc_x;
  int loc_y;
  int old_x;
  int old_y;
  int x_vel;
  int y_vel;
  int direction;
  int speed;
  uint8_t damage;
  Sprite sprite;
} Projectile;

typedef long time_t;

typedef struct
{
    int    running;
    time_t time;        // global timer
    time_t seconds;     // second timer (time/1000)
    time_t ticks;       // total game ticks
    time_t frames;      // total frames drawn
    time_t tick_time;   // target time interval of logic cycles
    time_t frame_time;  // target time interval between draws
    int    tick_rate;   // target rate of logic cycles
    int    frame_rate;  // target frame rate
    int    fps;         // actual measured fps
    float  fps_avg;     // average fps (frames/seconds)
} System_t;

enum SPRITES
{
    TILE_FLOOR,
    TILE_WALL,
    TILE_EXIT,
    TILE_DOOR_C,
    TILE_DOOR_O,
    ITEM_KEY,
    ITEM_MINE,
    SPR_PLAYER,
    SPR_GUARD,
    SPR_EXPLO,
    SPR_BULLET,
    SPR_GRAVE,
    SPR_CORPSE,
    SPR_ERROR,
    SHAD_IN_COR,
    SHAD_HORZ,
    SHAD_VERT,
    SHAD_OUT_COR,
    SHAD_MINE,
    SHAD_KEY,
    COMP_DOOR_C,
    COMP_KEY,
    COMP_MINE,
    NUM_SPRITES
};

void start_game();

#endif /* SHARED_H */

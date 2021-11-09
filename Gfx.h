#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define EGA_16_COLOR_MODE   0xD

#define SCREEN_WIDTH        320       /* width in pixels */
#define SCREEN_HEIGHT       200       /* height in pixels */
#define SCREEN_SIZE         SCREEN_WIDTH*SCREEN_HEIGHT
#define NUM_COLORS          256        /* number of colors in VGA */

#define TILE_WIDTH          8
#define TILE_HEIGHT         8
#define TILE_AREA           TILE_WIDTH*TILE_HEIGHT

void set_mode();
void fill_screen();
void load_sprite();
void load_graphics();
void draw_sprite();
void draw_sprite_tr();
void draw_big();
void draw_shadow();
void render_maze();
void render_actors();
void render_menu();
void render_end();
void render();

#endif
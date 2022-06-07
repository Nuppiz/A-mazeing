#ifndef GFX_H
#define GFX_H

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define EGA_16_COLOR_MODE   0xD

#define NUM_COLORS          256        /* number of colors in VGA */

#define TILE_AREA           TILE_WIDTH*TILE_HEIGHT
#define CHARACTER_SIZE      72

#if SCREEN_WIDTH == 320
#define SET_PIXEL(x,y,color) screen_buf[(((y)<<8)+((y)<<6)) + (x)] = color
#else
#define SET_PIXEL(x,y,color) screen_buf[(y)*SCREEN_WIDTH + (x)] = color
#endif

void set_tile_gfx();
void set_mode();
void fill_screen();
void load_gfx();
void load_sprite();
void load_tiles();
void load_special_gfx();
void composite_sprite();
void create_composites();
void draw_sprite();
void draw_temp();
void draw_sprite_tr();
void draw_big();
void draw_rectangle();
void draw_shadow();
void draw_text();
void render_text();
void typewriter();
void render_stats();
void render_maze();
void anim_explosion();
void render_previous();
void render_actors();
void render_menu_text();
void draw_help_contents();
void change_menu();
void render_cursor();
void render_menu();
void render_end();
void start_screen();
void gameover_screen();
void debug_screen_e();
void debug_screen_d();
void render();

#endif /* GFX_H */

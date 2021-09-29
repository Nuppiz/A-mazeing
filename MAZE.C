#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dos.h>
#include <conio.h>
#include <time.h>

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define EGA_16_COLOR_MODE   0xD
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels */
#define SCREEN_HEIGHT       200       /* height in pixels */
#define NUM_COLORS          16        /* number of colors in EGA */

#define TILE_WIDTH			8
#define TILE_HEIGHT			8
#define TILE_AREA 			TILE_WIDTH*TILE_HEIGHT

uint8_t far *EGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */

uint8_t width = 10;
uint8_t height = 10;

uint8_t game_running = 1;
 
uint8_t level01[] =
    "WWWWWWWWWW"
    "W*W-^-W--W"
    "W-W---W--W"
    "W-WWW----W"
    "W------WWW"
    "W^W-W^^-^W"
    "W------WWW"
    "W-WWW--|-W"
    "W--^---WeW"
    "WWWWWWWWWW";
 
int player_y = 6;
int player_x = 5;

/*int enemy_y = 4;
int enemy_x = 4;
int direction = 2;*/

uint8_t brick_wall[TILE_AREA];
uint8_t player_sprite[TILE_AREA];
uint8_t guard_sprite[TILE_AREA];
uint8_t key_sprite[TILE_AREA];
uint8_t floor_sprite[TILE_AREA];
uint8_t door_c_sprite[TILE_AREA];
uint8_t door_o_sprite[TILE_AREA];
uint8_t exit_sprite[TILE_AREA];
uint8_t mine_sprite[TILE_AREA];
uint8_t expl_sprite[TILE_AREA];
uint8_t grave_sprite[TILE_AREA];

struct Enemy
{
  uint8_t x;
  uint8_t y;
  uint8_t direction;
};

void ticker(float seconds)
{
    int milliseconds = 1000 * seconds;
    time_t startTime = clock();
    while(clock() < startTime + milliseconds);
}

void set_mode(uint8_t mode)
{
	union REGS regs;

	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(VIDEO_INT, &regs, &regs);
}

/*int end_game(int ending)
{
	int response;
	
	if (ending == 1)
	{
		printf("You stepped on a landmine and blew up! Enter Y for a new game, any other key to quit:\n");
	}
	else if (ending == 2)
	{
		printf("You were caught by a guard! Enter Y for a new game, any other key to quit:\n");
	}
	else if (ending == 3)
	{
		printf("You managed to escape! Enter Y for a new game, any other key to quit:\n");
	}
		
	scanf("%d", &response);
	if (response == 89 || response == 121)
	{
		return 1;
	}
	else
		return 0;
}*/

void draw_sprite(int x, int y, uint8_t* sprite)
{
	uint8_t index_x = 0;
	uint8_t index_y = 0;
	uint8_t i = 0;

	for (index_y=0;index_y<TILE_HEIGHT;index_y++)
	{
		for (index_x=0;index_x<TILE_WIDTH;index_x++)
		{
			VGA[y*SCREEN_WIDTH+x]=sprite[i];
			i++;
			x++;
		}
		index_x = 0;
        x = x - TILE_WIDTH;
		y++;
	}
	index_y = 0;
}

void render_maze()
{   
    uint16_t counter = 0;
    uint8_t y = 0;
    uint8_t x = 0;
    
    while (y < height)
    {
        while (x < width)
        {
			if (level01[counter] == 87)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,brick_wall);
			if (level01[counter] == 45)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
			if (level01[counter] == 94)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,mine_sprite);
			if (level01[counter] == 42)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,key_sprite);
			if (level01[counter] == 124)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,door_c_sprite);
			if (level01[counter] == 250)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,door_o_sprite);
			if (level01[counter] == 101)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,exit_sprite);
				
            counter = counter + 1;
            x = x + 1;
        }
        y = y + 1;
        x = 0;
    }
}

void save_sprite(char* filename, uint8_t* source_data, uint16_t data_size)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "wb+");
    fwrite(source_data, 1, data_size, file_ptr);
    fclose(file_ptr);
}

void load_sprite(char* filename, uint8_t* source_data, uint16_t data_size)
{
    FILE* file_ptr;
    file_ptr = fopen(filename, "rb");
    fread(source_data, 1, data_size, file_ptr);
    fclose(file_ptr);
}

void load_graphics()
{
	load_sprite("FLOOR.TEX", floor_sprite, 64);
	load_sprite("BRICKS.TEX", brick_wall, 64);
	load_sprite("PLAYER.TEX", player_sprite, 64);
	load_sprite("GUARD.TEX", guard_sprite, 64);
	load_sprite("KEY.TEX", key_sprite, 64);
	load_sprite("DOORC.TEX", door_c_sprite, 64);
	load_sprite("DOORO.TEX", door_o_sprite, 64);
	load_sprite("EXIT.TEX", exit_sprite, 64);
	load_sprite("MINE.TEX", mine_sprite, 64);
	load_sprite("EXPLO.TEX", expl_sprite, 64);
	load_sprite("GRAVE.TEX", grave_sprite, 64);
}

void enemy_movement(struct Enemy* p_enemy)
{
    uint8_t en_new_y;
    uint8_t en_new_x;

    if (p_enemy->direction == 1) /*up*/
        p_enemy->y = p_enemy->y - 1;
    else if (p_enemy->direction == 2) /*left*/
        p_enemy->x = p_enemy->x - 1;
    else if (p_enemy->direction == 3) /*down*/
        p_enemy->y = p_enemy->y + 1;
    else if (p_enemy->direction == 4) /*right*/
        p_enemy->x = p_enemy->x + 1;
	
	en_new_y = p_enemy->y;
	en_new_x = p_enemy->x;

		if (level01[en_new_y * width + en_new_x] == 'W')
			{
				if (p_enemy->direction == 1) /*up*/
				{
					p_enemy->direction = 3;
					en_new_y = p_enemy->y + 1;
				}
				else if (p_enemy->direction == 2) /*left*/
				{
					p_enemy->direction = 4;
					en_new_x = p_enemy->x + 1;
				}
				else if (p_enemy->direction == 3) /*down*/
				{
					p_enemy->direction = 1;
					en_new_y = p_enemy->y - 1;
				}
				else if (p_enemy->direction == 4) /*right*/
				{
					p_enemy->direction = 2;
					en_new_x = p_enemy->x - 1;
				}
			}
		
	p_enemy->y = en_new_y;
	p_enemy->x = en_new_x;
	draw_sprite(p_enemy->x * TILE_WIDTH,p_enemy->y * TILE_HEIGHT,guard_sprite);
	
	if (player_y == p_enemy->y && player_x == p_enemy->x)
	{
		draw_sprite(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,grave_sprite);
		getch();
		game_running = 0;
		//new_game = end_game(2);
	}
}

void draw_enemy(struct Enemy* p_enemy)
{
	draw_sprite(p_enemy->x * TILE_WIDTH,p_enemy->y * TILE_HEIGHT,guard_sprite);
}

void main()
{
	uint8_t movement;
	uint8_t key_acquired = 0;
	struct Enemy Guard1;
	struct Enemy Guard2;
	
	uint8_t new_x = player_x;
    uint8_t new_y = player_y;
	uint8_t new_game;
	
	Guard1.x = 4;
	Guard1.y = 4;
	Guard1.direction = 2;
		
	Guard2.x = 7;
	Guard2.y = 2;
	Guard2.direction = 1;
	
	set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */
	
	load_graphics();
    
    render_maze();
    
    while(game_running == 1)
    {				
		draw_enemy(&Guard1);
		draw_enemy(&Guard2);
        
        if (kbhit())
        {
            movement = getchar();
 
            if (movement == 119) /*w*/
                new_y = player_y - 1;
            else if (movement == 97) /*a*/
                new_x = player_x - 1;
            else if (movement == 115) /*s*/
                new_y = player_y + 1;
            else if (movement == 100) /*d*/
                new_x = player_x + 1;
            else if (movement == 27) /*ESC*/
                game_running = 0;
        }
        if (level01[new_y * width + new_x] != 'W')
        {
			if (level01[new_y * width + new_x] == 124 && key_acquired == 1)
			{
				level01[new_y * width + new_x] = 250;
				player_y = new_y;
				player_x = new_x;
				//render_maze();
				draw_sprite(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,player_sprite);
			}
			else
            {
				player_y = new_y;
				player_x = new_x;
				render_maze();
				draw_sprite(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,player_sprite);
			
				if (level01[player_y * width + player_x] == 42)
                {
                    key_acquired = 1;
					level01[player_y * width + player_x] = 45;
                }
                else if (level01[player_y * width + player_x] == 94)
                {
					draw_sprite(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,expl_sprite);
					draw_enemy(&Guard1);
					draw_enemy(&Guard2);
					getch();
					game_running = 0;
                    //new_game = end_game(1);
                }
				else if (level01[player_y * width + player_x] == 101)
                {
					draw_enemy(&Guard1);
					draw_enemy(&Guard2);
					getch();	
					game_running = 0;
                    //new_game = end_game(3);
                }
				if (new_game == 0)
					game_running = 0;
			}
		}
		enemy_movement(&Guard1);
		enemy_movement(&Guard2);
        ticker(0.01);
    }
	set_mode(TEXT_MODE);
	return;
}
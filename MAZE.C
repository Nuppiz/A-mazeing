#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include <string.h>

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

#define MAX_ENEMIES			10

#define TILE_WALL			'W'
#define TILE_FLOOR			'-'
#define TILE_DOOR_C			'|'
#define TILE_DOOR_O			'_'
#define TILE_EXIT			'e'
#define ITEM_KEY			'*'
#define ITEM_MINE			'^'

uint8_t far *EGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */

uint8_t width;
uint8_t height;

uint8_t game_running = 1;

char* filename;
uint8_t level_num = 1;
 
uint8_t* level_data;
 
uint8_t player_y;
uint8_t player_x;

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
	int8_t direction;
};

struct Enemy* enemy_array[MAX_ENEMIES];

void level_loader(char* levelname)
{
	FILE* level_file;
	char buffer[100];
	char c;
	uint16_t i = 0;
	uint8_t enemy_count = 0;
	
	level_file = fopen(levelname, "r");
    
	if (level_file == NULL)
	{
		printf("Unable to open file: %s\n", levelname);
		printf("Please check the file actually exists\n");
		exit(EXIT_FAILURE);
	}
	
	while ((c = fgetc(level_file)) != EOF)
	{
		if (c == '$')
		{
			fscanf(level_file, "%s", buffer);
			if (strcmp(buffer, "leveldim") == 0)
			{
				fscanf(level_file, "%d %d", &width, &height);
			}
			else if (strcmp(buffer, "playerloc") == 0)
			{
				fscanf(level_file, "%d %d", &player_y, &player_x);
			}
			else if (strcmp(buffer, "tiledata") == 0)
			{
				while ((c = fgetc(level_file)) != EOF)
				{
					if (c != '\n')
					{
						level_data[i] = c;
						i++;
					}
				}
			}
			else if (strcmp(buffer, "enemy") == 0)
			{
				if (enemy_count < MAX_ENEMIES)
				{
					fscanf(level_file, "%d %d %d", &enemy_array[enemy_count]->x, &enemy_array[enemy_count]->y, &enemy_array[enemy_count]->direction);
					enemy_count++;
				}
			}
		}
	}
	fclose(level_file);
}

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

void draw_sprite_tr(int x, int y, uint8_t* sprite)
{
	uint8_t index_x = 0;
	uint8_t index_y = 0;
	uint8_t i = 0;

	for (index_y=0;index_y<TILE_HEIGHT;index_y++)
	{
		for (index_x=0;index_x<TILE_WIDTH;index_x++)
		{
			if (sprite[i] != 13)
			{
				VGA[y*SCREEN_WIDTH+x]=sprite[i];
				i++;
				x++;
			}
			else
			{
				i++;
				x++;
			}
		}
		index_x = 0;
		x = x - TILE_WIDTH;
		y++;
	}
	index_y = 0;
}

void render_maze(uint8_t player_y, uint8_t player_x)
{   
	uint16_t counter = 0;
	uint8_t y = 0;
	uint8_t x = 0;
    
	while (y < height)
	{
		while (x < width)
		{
			if (level_data[counter] == TILE_WALL)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,brick_wall);
			else if (level_data[counter] == TILE_FLOOR)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
			else if (level_data[counter] == ITEM_MINE)
			{
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr(x * TILE_WIDTH,y * TILE_HEIGHT,mine_sprite);
			}
			else if (level_data[counter] == ITEM_KEY)
			{
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr(x * TILE_WIDTH,y * TILE_HEIGHT,key_sprite);
			}
			else if (level_data[counter] == TILE_DOOR_C)
			{
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr(x * TILE_WIDTH,y * TILE_HEIGHT,door_c_sprite);
			}
			else if (level_data[counter] == TILE_DOOR_O)
			{
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr(x * TILE_WIDTH,y * TILE_HEIGHT,door_o_sprite);
			}
			else if (level_data[counter] == TILE_EXIT)
				draw_sprite(x * TILE_WIDTH,y * TILE_HEIGHT,exit_sprite);
				
			counter = counter++;
			x = x++;
        }
		y = y++;
		x = 0;
    }
	draw_sprite_tr(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,player_sprite);
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
	load_sprite("FLOOR.7UP", floor_sprite, 64);
	load_sprite("BRICKS.7UP", brick_wall, 64);
	load_sprite("PLAYER.7UP", player_sprite, 64);
	load_sprite("GUARD.7UP", guard_sprite, 64);
	load_sprite("KEY.7UP", key_sprite, 64);
	load_sprite("DOORC.7UP", door_c_sprite, 64);
	load_sprite("DOORO.7UP", door_o_sprite, 64);
	load_sprite("EXIT.7UP", exit_sprite, 64);
	load_sprite("MINE.7UP", mine_sprite, 64);
	load_sprite("EXPLO.7UP", expl_sprite, 64);
	load_sprite("GRAVE.7UP", grave_sprite, 64);
}

void enemy_movement(struct Enemy* p_enemy)
{
	//temporary movement values are set to current position
	uint8_t en_new_y = p_enemy->y;
	uint8_t en_new_x = p_enemy->x;

	//make a temporary move based on what direction value is given
	if (p_enemy->direction == 1) /*up*/
		en_new_y = p_enemy->y - 1;
	else if (p_enemy->direction == 2) /*left*/
		en_new_x = p_enemy->x - 1;
	else if (p_enemy->direction == -1) /*down*/
		en_new_y = p_enemy->y + 1);
	else if (p_enemy->direction == -2) /*right*/
		en_new_x = p_enemy->x + 1;
	
	//if the temporary movement would result in the enemy going inside of a wall, reverse the movement and direction
	if (level_data[en_new_y * width + en_new_x] == 'W')
	{
		if (p_enemy->direction == 1) /*up*/
			en_new_y = p_enemy->y + 1;
		else if (p_enemy->direction == 2) /*left*/
			en_new_x = p_enemy->x + 1;
		else if (p_enemy->direction == -1) /*down*/
			en_new_y = p_enemy->y - 1;
		else if (p_enemy->direction == -2) /*right*/
			en_new_x = p_enemy->x - 1;
		p_enemy->direction = -p_enemy->direction;
	}
		
	//temporary movement values become the current movement values
	p_enemy->y = en_new_y;
	p_enemy->x = en_new_x;
	
	//draw enemy at the updated location
	draw_sprite_tr(p_enemy->x * TILE_WIDTH,p_enemy->y * TILE_HEIGHT,guard_sprite);
	
	//if player and enemy are in the same square, kill the player
	if (player_y == p_enemy->y && player_x == p_enemy->x)
	{
		draw_sprite_tr(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,grave_sprite);
		getch();
		game_running = 0;
		//new_game = end_game(2);
	}
}

void draw_enemy(struct Enemy* p_enemy)
{
	draw_sprite_tr(p_enemy->x * TILE_WIDTH,p_enemy->y * TILE_HEIGHT,guard_sprite);
}

void main()
{
	uint8_t movement;
	uint8_t key_acquired = 0;
	
	uint8_t new_x = player_x;
	uint8_t new_y = player_y;
	uint8_t new_game;
	
	char buf[13];
	
	filename = malloc(13);
	level_data = malloc(1000);
	
	sprintf(buf, "level%d.txt", level_num);
	filename = buf;
	level_loader(filename);
	
	set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */
	
	load_graphics();
    
	render_maze(player_y, player_x);
    
	while(game_running == 1)
	{				        
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
			else if (movement == 99) /*c*/
				key_acquired = 1;
        	}
        	if (level_data[new_y * width + new_x] != 'W')
		{
			if (level_data[new_y * width + new_x] == 124 && key_acquired == 0)
			{
				new_y = player_y;
				new_x = player_x;
			}
			else if (level_data[new_y * width + new_x] == 124 && key_acquired == 1)
			{
				level_data[new_y * width + new_x] = 250;
				player_y = new_y;
				player_x = new_x;
			}
			else
			{
			player_y = new_y;
			player_x = new_x;
				
				if (level_data[player_y * width + player_x] == 42)
				{
					key_acquired = 1;
					level_data[player_y * width + player_x] = 45;
				}
				else if (level_data[player_y * width + player_x] == 94)
				{
					render_maze(player_y, player_x);
					draw_sprite_tr(player_x * TILE_WIDTH,player_y * TILE_HEIGHT,expl_sprite);
					draw_enemy(&enemy_array);
					getch();
					game_running = 0;
					//new_game = end_game(1);
				}
				else if (level_data[player_y * width + player_x] == 101)
				{
					render_maze(player_y, player_x);
					draw_enemy(&enemy_array);
					getch();	
					game_running = 0;
					//new_game = end_game(3);
				}
				if (new_game == 0)
					game_running = 0;
			}
		}
		if (game_running == 1)
		{
			render_maze(player_y, player_x);
			enemy_movement(&enemy_array);
			ticker(0.01);
		}
	}
	set_mode(TEXT_MODE);
	return;
}

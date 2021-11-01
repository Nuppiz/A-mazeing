#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dos.h>
//#include <conio.h>
//#include <time.h>
//#include <string.h>

#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define EGA_16_COLOR_MODE   0xD
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels */
#define SCREEN_HEIGHT       200       /* height in pixels */
#define NUM_COLORS          16        /* number of colors in EGA */

//#define BYTE unsigned 		char
#define NUM_SCAN_QUE 		256
#define TICKS	    		(*(volatile unsigned long far *)(0x0040006CL))

#define KEY_PRESSED 		1 /* 1st bit */
#define KEY_HIT 			2 /* 2nd bit */
#define KEY_REL 			4 /* 3rd bit */

#define KEY_ESC 			1
#define KEY_SPACE 			57
#define KEY_UP				72
#define KEY_LEFT			75
#define KEY_RIGHT			77
#define KEY_DOWN			80
#define KEY_CHEAT  			46

#define IS_HIT(k) 			(kb_array[k] & KEY_HIT)
#define IS_REL(k) 			(kb_array[k] & KEY_REL)
#define IS_PRESSED(k) 		(kb_array[k] & KEY_PRESSED)

#define TILE_WIDTH			8
#define TILE_HEIGHT			8
#define TILE_AREA 			TILE_WIDTH*TILE_HEIGHT

#define MAX_ACTORS			10

#define TILE_WALL			87
#define TILE_FLOOR			45
#define TILE_DOOR_C			124
#define TILE_DOOR_O			250
#define TILE_EXIT			101
#define ITEM_KEY			42
#define ITEM_MINE			94

#define ACTOR_PLAYER		0
#define ACTOR_GUARD 		1
#define ACTOR_GRAVE			2	
#define ACTOR_EXPLO			3

#define GAME_MENU			0
#define GAME_INGAME			1
#define GAME_OVER 			2
#define GAME_WIN 			3
#define GAME_END			4

#define LAST_LEVEL			2

// macros
#define TILE_AT(x, y) 			g->level_data[y * g->level_width + x]
#define ACTOR_ON_TILE(a, tile) 	g->level_data[a->y * g->level_width + a->x] == tile
#define SET_TILE(x, y, tile)   	g->level_data[y * g->level_width + x] = tile

// globals
uint8_t far *VGA=(uint8_t *)0xA0000000L;        /* this points to video memory. */
uint8_t kb_array[128];
uint8_t kb_scan; // a keyboard interrupt will store a scan code in this variable
uint8_t kb_queue[NUM_SCAN_QUE];
uint8_t kb_queue_head;
uint8_t kb_queue_tail;
unsigned char readch, oldreadch, extended; // needed for keyboard interrupt,
// don't know what for lmao 

// sprites
uint8_t brick_wall		[TILE_AREA];
uint8_t player_sprite	[TILE_AREA];
uint8_t guard_sprite	[TILE_AREA];
uint8_t key_sprite		[TILE_AREA];
uint8_t floor_sprite	[TILE_AREA];
uint8_t door_c_sprite	[TILE_AREA];
uint8_t door_o_sprite	[TILE_AREA];
uint8_t exit_sprite		[TILE_AREA];
uint8_t error_sprite	[TILE_AREA];
uint8_t mine_sprite		[TILE_AREA];
uint8_t expl_sprite		[TILE_AREA];
uint8_t grave_sprite	[TILE_AREA];

// structs
union REGS regs;

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
	int keys_acquired;
	int actor_count;	

	struct Actor Actors[MAX_ACTORS];
};

// function forward declarations / prototypes
void level_loader(struct GameData* g);

// function definitions
//void interrupt far keyhandler(void);
void interrupt far keyhandler()
{
	oldreadch = readch;
	asm {
		in al, 0x60
		mov readch, al
		in al, 0x61
		or al, 0x80
		out 0x61, al
		and al, 0x7f
		out 0x61, al
	}
	if (readch == 0xe0)
		extended = 128;
	else
	{
		if (oldreadch != readch)
			kb_queue[(readch&127)+extended] = ((readch>>7)^1);
		extended = 0;
	}
	asm {
		mov al, 0x20
		out 0x20, al
	}
	
	kb_queue[kb_queue_tail++] = readch;//kb_scan;
}

void (interrupt far *oldkeyhandler)();

void init_keyboard()
{
	oldkeyhandler = _dos_getvect(0x9);
	_disable(); _dos_setvect(0x9, keyhandler); _enable();
	memset(kb_array, 0, 128);
}

void start_game(struct GameData* g)
{
	g->game_state = GAME_INGAME;
	g->level_num = 1;
	level_loader(g);
}

void control_menu(struct GameData* g)
{
	if (IS_HIT(KEY_SPACE))
		start_game(g);
}

void control_end(struct GameData* g)
{
	if (IS_HIT(KEY_SPACE))
		g->game_state = GAME_MENU;
}

void control_ingame(struct GameData* g)
{
	// Y axis / Up / Down
	if (IS_PRESSED(KEY_UP))
		g->Actors[0].y_vel = -1;
	else if (IS_PRESSED(KEY_DOWN))
		g->Actors[0].y_vel = 1;
	else
		g->Actors[0].y_vel = 0;
	
	// X axis / Left / Right
	if (IS_PRESSED(KEY_LEFT))
		g->Actors[0].x_vel = -1;
	else if (IS_PRESSED(KEY_RIGHT))
		g->Actors[0].x_vel = 1;
	else
		g->Actors[0].x_vel = 0;
	
	// Misc
	if (IS_HIT(KEY_CHEAT))
		g->keys_acquired = 1;
}

void get_keyboard()
{
	int key;
	
	while (kb_queue_head != kb_queue_tail)
	{
		key = kb_queue[kb_queue_head];
		kb_queue_head++;
	 
		/* key pressed down code (0-127) */
		if (key < 128)
		{
			/* if the key was not pressed down before, set the KEY_HIT bit (2nd bit) on */
			if ( (kb_array[key] & KEY_PRESSED) == 0)
				kb_array[key] |= KEY_HIT;
			
			/* in any case, set the KEY_PRESSED bit (1st bit) on */
			kb_array[key] |= KEY_PRESSED;
		}
		/* key released code (128-255) */
		else
		{
			/* if there was something else than 000000000 for this key, and
			 we detect a key release, then set KEY_REL bit (3rd bit, binary value of 4) on */
			if (kb_array[key-128] & KEY_PRESSED)
				kb_array[key-128] |= KEY_REL;
			
			/* in any case, clear the KEY_PRESSED bit (1st bit), off */
			kb_array[key-128] &= (~KEY_PRESSED);
		}
	}
}

void clear_keys()
{
	int i = 0;
	
	while (i < 128)
	{
		kb_array[i] &= (~KEY_HIT);
		kb_array[i] &= (~KEY_REL);
		i++;
	}
}

void process_input(struct GameData* g)
{
	get_keyboard();
	
	if (g->game_state == GAME_MENU)
		control_menu(g);
	else if (g->game_state == GAME_END)
		control_end(g);
	else if (g->game_state == GAME_INGAME)
		control_ingame(g);
	
	// esc always exits, wherever you are
	if (IS_HIT(KEY_ESC))
		g->game_running = 0;
}

void move_actors(struct GameData* g)
{
	int new_x, new_y;
	int i = 0;
	while (i < g->actor_count)
	{
		struct Actor* p_actor = &g->Actors[i++];
		
		// set coordinates for new square to (potentially) move into
		// based on adding velocity to current position...
		new_x = p_actor->x + p_actor->x_vel;
		new_y = p_actor->y + p_actor->y_vel;
		
		// if it's NOT a wall, then move into the square
		if (TILE_AT(new_x, new_y) != TILE_WALL)
		{
			p_actor->x = new_x;
			p_actor->y = new_y;
		}
		// if colliding into a wall and is NOT player, reverse dir
		else if (p_actor->type != ACTOR_PLAYER)
		{
			p_actor->x_vel *= -1;
			p_actor->y_vel *= -1;
		}
	}
}

void player_hit_detect(struct GameData* g)
{
	struct Actor* p = &g->Actors[0];
	int i = 1; // ignore player
	
	// step on mine
	if (ACTOR_ON_TILE(p, ITEM_MINE))
	{
		g->game_state = GAME_OVER;
		p->type = ACTOR_EXPLO;
	}
	// collect key
	else if (ACTOR_ON_TILE(p, ITEM_KEY))
	{
		g->keys_acquired++;
		SET_TILE(p->x, p->y, TILE_FLOOR);
	}
	// try to open door
	else if (ACTOR_ON_TILE(p, TILE_DOOR_C))
	{
		// no keys; door remains closed, so cancel movement
		if (g->keys_acquired == 0)
		{
			p->x -= p->x_vel;
			p->y -= p->y_vel;
		}
		// otherwise, we remain in the door tile and change tile to DOOR_O
		else
			SET_TILE(p->x, p->y, TILE_DOOR_O);
	}
	// exit and win the level
	else if (ACTOR_ON_TILE(p, TILE_EXIT))
		g->game_state = GAME_WIN;

	// see if we hit a guard
	while (i < g->actor_count)
	{
		struct Actor* enemy = &g->Actors[i++];
		
		if (p->x == enemy->x && p->y == enemy->y)
		{
			g->game_state = GAME_OVER;
			p->type = ACTOR_GRAVE;
			break;
		}
	}
}

void next_level(struct GameData* g)
{
	g->game_state = GAME_INGAME;
	g->level_num++;
	level_loader(g);
}

void check_state(struct GameData* g)
{
	if (g->game_state == GAME_OVER)
	{
		delay(2000);
		g->game_state = GAME_MENU;
	}
	else if (g->game_state == GAME_WIN)
	{
		delay(2000);
		
		if (g->level_num == LAST_LEVEL)
			g->game_state = GAME_END;
		else
		{
			next_level(g);
		}
	}
	else if (g->game_state == GAME_END)
	{
		;
		//delay(5000);
		// idk something lol
	}
}

void game_logic(struct GameData* g)
{
	check_state(g);
	if (g->game_state == GAME_INGAME)
	{
		move_actors(g);
		player_hit_detect(g);
	}
}

void set_mode(uint8_t mode)
{
	//union REGS regs;

	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(VIDEO_INT, &regs, &regs);
}

void load_sprite(char* filename, uint8_t* source_data, uint16_t data_size)
{
	FILE* file_ptr;
	file_ptr = fopen(filename, "rb");
	fread(source_data, 1, data_size, file_ptr);
	fclose(file_ptr);
}

/*
void save_sprite(char* filename, uint8_t* source_data, uint16_t data_size)
{
	FILE* file_ptr;
	file_ptr = fopen(filename, "wb+");
	fwrite(source_data, 1, data_size, file_ptr);
	fclose(file_ptr);
}
*/

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
	load_sprite("ERROR.7UP", error_sprite, 64);
	load_sprite("MINE.7UP", mine_sprite, 64);
	load_sprite("EXPLO.7UP", expl_sprite, 64);
	load_sprite("GRAVE.7UP", grave_sprite, 64);
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

void render_maze(struct GameData* g)
{   
	int counter = 0;
	int y = 0;
	int x = 0;
    
	while (y < g->level_height)
	{
		while (x < g->level_width)
		{
			if (g->level_data[counter] == TILE_WALL)
				draw_sprite		(x * TILE_WIDTH,y * TILE_HEIGHT,brick_wall);
			else if (g->level_data[counter] == TILE_FLOOR)
				draw_sprite		(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
			else if (g->level_data[counter] == ITEM_MINE)
			{
				draw_sprite		(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr	(x * TILE_WIDTH,y * TILE_HEIGHT,mine_sprite);
			}
			else if (g->level_data[counter] == ITEM_KEY)
			{
				draw_sprite		(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr	(x * TILE_WIDTH,y * TILE_HEIGHT,key_sprite);
			}
			else if (g->level_data[counter] == TILE_DOOR_C)
			{
				draw_sprite   	(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr	(x * TILE_WIDTH,y * TILE_HEIGHT,door_c_sprite);
			}
			else if (g->level_data[counter] == TILE_DOOR_O)
			{
				draw_sprite		(x * TILE_WIDTH,y * TILE_HEIGHT,floor_sprite);
				draw_sprite_tr	(x * TILE_WIDTH,y * TILE_HEIGHT,door_o_sprite);
			}
			else if (g->level_data[counter] == TILE_EXIT)
				draw_sprite		(x * TILE_WIDTH,y * TILE_HEIGHT,exit_sprite);
				
			counter = counter++;
			x = x++;
        }
		y = y++;
		x = 0;
    }
}

void render_actors(struct GameData* g)
{
	uint8_t* sprite;
	int i;
	
	i = 0;
	while (i < g->actor_count)
	{
		
		switch (g->Actors[i].type)
		{
			case ACTOR_PLAYER:	sprite = player_sprite;	break;
			case ACTOR_GUARD:	sprite = guard_sprite;	break;
			//case ACTOR_GRAVE:	sprite = grave_sprite;	break;
			//case ACTOR_EXPLO:	sprite = expl_sprite;	break;
			default:			sprite = error_sprite;	break;
		}
		
		if (sprite == error_sprite)
		{
			i++;
			continue;
		}
		
		draw_sprite_tr(g->Actors[i].x * TILE_WIDTH,
					   g->Actors[i].y * TILE_HEIGHT,
				       sprite);
					   
		i++;
	}
	
	i = 0;
	while (i < g->actor_count)
	{
		
		switch (g->Actors[i].type)
		{
			//case ACTOR_PLAYER:	sprite = player_sprite;	break;
			//case ACTOR_GUARD:	sprite = guard_sprite;	break;
			case ACTOR_GRAVE:	sprite = grave_sprite;	break;
			case ACTOR_EXPLO:	sprite = expl_sprite;	break;
			default:			sprite = error_sprite;	break;
		}
		
		if (sprite == error_sprite)
		{
			i++;
			continue;
		}
		
		draw_sprite_tr(g->Actors[i].x * TILE_WIDTH,
					   g->Actors[i].y * TILE_HEIGHT,
				       sprite);
					   
		i++;
	}
}

void render_menu()
{
	clrscr();
	draw_sprite(0, 0, exit_sprite); // change to menu gfx or text later
	draw_sprite(0, 8, exit_sprite);
	draw_sprite(0, 16, exit_sprite);
	draw_sprite(0, 24, exit_sprite);
	draw_sprite(8, 8, exit_sprite);
	draw_sprite(16, 16, exit_sprite);
	draw_sprite(24, 8, exit_sprite);
	draw_sprite(32, 0, exit_sprite);
	draw_sprite(32, 8, exit_sprite);
	draw_sprite(32, 16, exit_sprite);
	draw_sprite(32, 24, exit_sprite);
}

void render_end()
{
	clrscr();
	printf("the end lol");
}

void render(struct GameData* g)
{
	// in case playing, just died, or exited
	// draw play field and objects
	if (g->game_state == GAME_INGAME ||
		g->game_state == GAME_OVER ||
		g->game_state == GAME_WIN)
	{
		render_maze(g);
		render_actors(g);
	}
	// if menu ...
	else if (g->game_state == GAME_MENU)
	{
		render_menu();
	}
	// if end ...
	else if (g->game_state == GAME_END)
	{
		render_end();
	}
}

void init_gamedata(struct GameData* g)
{
	/* init game data */
	memset(g, 0, sizeof(struct GameData));
}

void level_loader(struct GameData* g)
{
	FILE* level_file;
	char buffer[100];
	char filename[13];
	char c;
	int i = 0;
	
	/* build filename and load it */
	sprintf(filename, "level%d.txt", g->level_num);
	level_file = fopen(filename, "r");
    
	if (level_file == NULL)
	{
		printf("Unable to open file: %s\n", filename);
		printf("Please check the file actually exists\n");
		exit(EXIT_FAILURE);
	}
	
	g->actor_count = 0;
	g->keys_acquired = 0;
	
	while ((c = fgetc(level_file)) != EOF)
	{
		if (c == '$')
		{
			fscanf(level_file, "%s", buffer);
			if (strcmp(buffer, "leveldim") == 0)
			{
				fscanf(level_file, "%d %d", &g->level_width, &g->level_height);
			}
			else if (strcmp(buffer, "player") == 0)
			{
				fscanf(level_file, "%d %d %d %d",
					&g->Actors[0].x,
					&g->Actors[0].y,
					&g->Actors[0].x_vel,
					&g->Actors[0].y_vel
				);
				
				g->Actors[0].type = ACTOR_PLAYER;
				g->actor_count++;
			}
			else if (strcmp(buffer, "guard") == 0)
			{
				if (g->actor_count < MAX_ACTORS)
				{
					fscanf(level_file, "%d %d %d %d",
						&g->Actors[g->actor_count].x,
						&g->Actors[g->actor_count].y,
						&g->Actors[g->actor_count].x_vel,
						&g->Actors[g->actor_count].y_vel
					);
						
					g->Actors[g->actor_count].type = ACTOR_GUARD;
					g->actor_count++;
				}
			}
			else if (strcmp(buffer, "tiledata") == 0)
			{
				while ((c = fgetc(level_file)) != EOF)
				{
					if (c != '\n')
					{
						g->level_data[i] = c;
						i++;
					}
					else
						putchar('\n');
				}
			}
		}
	}
	fclose(level_file);
}

void init(struct GameData* g)
{		
	/* init gfx */
	set_mode(VGA_256_COLOR_MODE);
	load_graphics();

	/* init gamedata */
	init_gamedata(g);
	g->level_num = 1;
	g->game_running = 1;
	g->game_state = GAME_MENU;
	
	/* load level file */
	//level_loader(g);
	
	/* init keyboard */
	init_keyboard();
}

void quit()
{
	//deinit_keyboard();
	set_mode(TEXT_MODE);
}

int main()
{
	/* local variables all inside GameData struct called g */
	struct GameData g;
	
	/* initialize everything */
	init(&g);

	/* run game, get keyboard state first */
	while (g.game_running == 1)
	{
		process_input(&g);
		game_logic(&g);
		clear_keys();
		render(&g);
		//play_audio(&g);
		delay(100);
	}
	
	/* quit */
	quit();
	
	return 0;
}

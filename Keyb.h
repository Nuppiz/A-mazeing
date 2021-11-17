#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define NUM_SCAN_QUE        256

#define KEY_PRESSED         1 /* 1st bit */
#define KEY_HIT             2 /* 2nd bit */
#define KEY_REL             4 /* 3rd bit */

#define KEY_ESC             1
#define KEY_SPACE           57
#define KEY_UP              72
#define KEY_LEFT            75
#define KEY_RIGHT           77
#define KEY_DOWN            80
#define KEY_CHEAT_K         46
#define KEY_CHEAT_L         38
#define KEY_CHEAT_D         37
#define KEY_CHEAT_S         31
#define KEY_Q               16
#define KEY_BKSPC           14

#define KEY_PRESSED         1 /* 1st bit */
#define KEY_HIT             2 /* 2nd bit */
#define KEY_REL             4 /* 3rd bit */
#define KEY_HIT_N_REL       6 /* 2nd and 3rd bits */
#define WAS_HIT(k)          (kb_array[k] & KEY_HIT)
#define WAS_REL(k)          (kb_array[k] & KEY_REL)
#define WAS_MOVED(k)        (kb_array[k] & KEY_HIT_N_REL)
#define IS_PRESSED(k)       (kb_array[k] & KEY_PRESSED)
 
#define X_AXIS 1
#define Y_AXIS 2

void interrupt far keyhandler();
void init_keyboard();
void deinit_keyboard();
void control_menu();
void control_end();
void control_ingame();
void get_keyboard();
void clear_keys();
void process_input();

#endif
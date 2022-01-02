#ifndef SOUNDS_H
#define SOUNDS_H

#define PIT_FREQ            0x1234DD // programmable interveral timer (PIT) frequency for PC speaker

void init_speaker();
void close_speaker();
void play_note();
void note_loop_up();
void note_loop_down();
void play_song();
void test_song();
void end_song();
void sound_gameover();
void sound_death();
void sound_key();
void sound_door_c();
void sound_door_o();
void sound_typing();

typedef struct
{
    uint16_t frequency;
    uint16_t length;
} note;

typedef struct
{
    note* notes;
    uint16_t num_notes;
} note_sequence;

#endif /* SOUNDS_H */

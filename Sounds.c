#include "Shared.h"
#include "Music.h"

#define PIT_FREQ            0x1234DD /* programmable interveral timer (PIT) frequency for PC speaker */

int16_t note_i = 0;
uint8_t song_i = 0;
uint16_t notes[11] = {277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};

note_sequence* current_sequence;

extern uint32_t timer;
uint32_t last_note = 0;
char song_name [11];

void init_music()
{
    /* initialize music files */
    mario_music.size = MarioUW_size;
    Doom_music.size = Doom_size;
    SW_music.size = SW_size;
    Odetojoy_music.size = Odetojoy_size;
}

void init_speaker()
{
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
}

void close_speaker()
{
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}

void play_note(int freq, int note_length)
{
    int counter;

    init_speaker();

    counter = (PIT_FREQ / freq); // calculate frequency
    outportb(0x42, counter & 0xff); // LSB
    outportb(0x42, counter >> 8); // MSB
    delay(note_length); // wait for a bit

    close_speaker();
}

void sequence_note(uint16_t freq)
{
    uint16_t counter;

    counter = (PIT_FREQ / freq); // calculate frequency
    outportb(0x42, counter & 0xff); // LSB
    outportb(0x42, counter >> 8); // MSB
}

void note_loop_up(int note_length, int min_freq, int max_freq)
{
    int freq;
    int counter;

    init_speaker();

    for (freq = min_freq; freq <= max_freq; freq++)
    {
        counter = (PIT_FREQ / freq); // calculate frequency
        outportb(0x42, counter & 0xff); // LSB
        outportb(0x42, counter >> 8); // MSB
        delay(note_length); // wait for a bit
    }

    close_speaker();
}

void note_loop_down(int note_length, int max_freq, int min_freq)
{
    int freq;
    int counter;

    init_speaker();

    for (freq = max_freq; freq >= min_freq; freq--)
    {
        counter = (PIT_FREQ / freq); // calculate frequency
        outportb(0x42, counter & 0xff); // LSB
        outportb(0x42, counter >> 8); // MSB
        delay(note_length); // wait for a bit
    }

    close_speaker();
}

void loop_song()
{
    init_speaker();
    for (note_i = 0; note_i < 10; note_i++);
    {
        play_note(notes[note_i], AUDIO_INTERVAL);
    }
    for (note_i = 10; note_i > -1; note_i--);
    {
        play_note(notes[note_i], AUDIO_INTERVAL);
    }
    close_speaker();
}

void music_track_select(struct GameData* g)
{
    if(g->game_state == GAME_INGAME)
    {
        switch (g->level_num)
        {
            case 1:   current_sequence = MUSIC_LVL1;  break;
            case 2:   current_sequence = MUSIC_LVL2;  break;
            case 3:   current_sequence = MUSIC_LVL3;  break;
            default:  current_sequence = MUSIC_LVL1;  break;
        }
    }
    else if (g->game_state == GAME_END)
        current_sequence = MUSIC_WIN;
}

void play_sequence()
{
    if (current_sequence != NULL)
    {
        if (last_note + current_sequence->duration[note_i] * 15 < timer)
        {
            init_speaker();
            if (note_i >= current_sequence->size)
            {
                note_i = 0;
                if (current_sequence->loop == FALSE)
                {
                    current_sequence = NULL;
                    close_speaker();
                }
            }
            else
            {
                last_note = timer;
                if (current_sequence->notes[note_i] != 0)
                    sequence_note(current_sequence->notes[note_i]);
                else
                    close_speaker();
                note_i++;
            }
        }
    }
}

void end_song()
{
    init_speaker();
    note_loop_up(2, 10, 1000);
    close_speaker();
}

void sound_gameover()
{
    init_speaker();
    play_note(NOTE_C5, 360 * 2);
    play_note(NOTE_G4, 360 * 2);
    play_note(NOTE_E4, 240 * 2);
    play_note(NOTE_A4, 180 * 2);
    play_note(NOTE_B4, 180 * 2);
    play_note(NOTE_A4, 180 * 2);
    play_note(NOTE_GS4, 180 * 2);
    play_note(NOTE_AS4, 180 * 2);
    play_note(NOTE_GS4, 180 * 2);
    play_note(NOTE_G4, 120 * 2);
    play_note(NOTE_D4, 120 * 2);
    play_note(NOTE_E4, 720 * 2);
    close_speaker();
}

void sound_death()
{
    init_speaker();
    note_loop_up(1, 100, 500);
    note_loop_down(1, 500, 100);
    close_speaker();
}

void sound_key()
{
    init_speaker();
    play_note(400, 25);
    close_speaker();
}

void sound_door_c()
{
    init_speaker();
    play_note(100, 25);
    close_speaker();
}

void sound_door_o()
{
    init_speaker();
    play_note(600, 25);
    close_speaker();
}

void sound_typing()
{    
    init_speaker();
    play_note(250, 10);
    close_speaker();
}
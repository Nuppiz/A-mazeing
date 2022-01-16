#include "Shared.h"
#include "Note_seq.h"

#define PIT_FREQ            0x1234DD /* programmable interveral timer (PIT) frequency for PC speaker */

int16_t note_i = 0;
int16_t song_i = 0;
uint16_t notes[11] = {277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};

note_sequence* current_sequence;
note_sequence* prev_sequence;

extern struct Options opt;
extern uint32_t timer;
uint32_t last_note = 0;
uint32_t last_effect = 0;
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

void switch_to_effect(uint8_t effect)
{
        if (current_sequence->type == SEQ_MUSIC) // prevent overwriting the music track
            prev_sequence = current_sequence;

        switch (effect)
        {
            case 1:   current_sequence = &explosion;  break;
            case 2:   current_sequence = &pickup;  break;
            case 3:   current_sequence = &door_open;  break;
            case 4:   current_sequence = &door_shut;  break;
            default:  current_sequence = prev_sequence;  break;
        }
        note_i = 0;
}

void play_sequence(struct Options* opt)
{
    if (current_sequence != NULL)
    {
        if (current_sequence->type == SEQ_MUSIC && opt->music_on == TRUE)
        {
            if (last_note + current_sequence->duration[song_i] * 15 < timer)
            {
                init_speaker();
                if (song_i >= current_sequence->size)
                {
                    song_i = 0;
                    if (current_sequence->loop == FALSE)
                    {
                        current_sequence = NULL;
                        close_speaker();
                    }
                }
                else
                {
                    last_note = timer;
                    if (current_sequence->notes[song_i] != 0)
                        sequence_note(current_sequence->notes[song_i]);
                    else
                        close_speaker();
                    song_i++;
                }
            }
        }
        else if (current_sequence->type == SEQ_EFFECT && opt->sfx_on == TRUE)
        {
            if (last_effect + current_sequence->duration[note_i] * 15 < timer)
            {
                init_speaker();
                if (note_i >= current_sequence->size)
                {
                    note_i = 0;
                    close_speaker();
                    current_sequence = prev_sequence;
                }
                else
                {
                    last_effect = timer;
                    if (current_sequence->notes[note_i] != 0)
                    {
                        sequence_note(current_sequence->notes[note_i]);
                    }
                    else
                        close_speaker();
                    note_i++;
                }
            }
        }
    }
}

/*void play_effect(uint8_t effect)
{
    int seq_i = 0;

    note_sequence* sequence;

    if (effect == 1)
        sequence = &explosion;

    if (sequence != NULL)
    {
        if (last_effect + sequence->duration[seq_i] < timer)
        {
            init_speaker();
            if (seq_i >= sequence->size)
            {
                seq_i = 0;
                if (sequence->loop == FALSE)
                {
                    sequence = NULL;
                    close_speaker();
                }
            }
            else
            {
                last_effect = timer;
                if (sequence->notes[seq_i] != 0)
                {
                    if (sequence->pitch_change != 0)
                    {
                        if (last_pitch_change + PITCH_INTERVAL < timer)
                        {
                            sequence_note(sequence->notes[seq_i] + sequence->pitch_change);
                            last_pitch_change = timer;
                        }
                        else
                        {
                            sequence_note(sequence->notes[seq_i]);
                        }
                    }
                    else
                    {
                        sequence_note(sequence->notes[seq_i]);
                    }
                }
                else
                    close_speaker();
            }
        }
    }
}*/

void end_song()
{
    init_speaker();
    note_loop_up(2, 10, 1000);
    close_speaker();
}

void sound_gameover()
{
    init_speaker();
    play_note(NOTE_C5, 540);
    play_note(NOTE_G4, 540);
    play_note(NOTE_E4, 360);
    play_note(NOTE_A4, 270);
    play_note(NOTE_B4, 270);
    play_note(NOTE_A4, 270);
    play_note(NOTE_GS4, 270);
    play_note(NOTE_AS4, 270);
    play_note(NOTE_GS4, 270);
    play_note(NOTE_G4, 180);
    play_note(NOTE_D4, 180);
    play_note(NOTE_E4, 1000);
    close_speaker();
}

void sound_typing()
{    
    init_speaker();
    play_note(250, 10);
    close_speaker();
}
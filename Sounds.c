#include "Shared.h"
#include "Sounds.h"

int8_t note_i;
uint8_t song_i;
uint16_t notes[11] = {277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};

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

void play_song()
{

    init_speaker();
    play_note(notes[note_i], 25);
    if (song_i < 10)
    {
        note_i++;
        song_i++;
    }
    else if (song_i >= 10 && song_i < 20)
    {
        note_i--;
        song_i++;
    }
    if (song_i >= 20)
        song_i = 0;
    close_speaker();
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
    play_note(600, 200);
    close_speaker();
    delay(250);
    init_speaker();
    play_note(400, 200);
    close_speaker();
    delay(250);
    init_speaker();
    play_note(300, 300);
    close_speaker();
    delay(100);
    init_speaker();
    play_note(200, 200);
    play_note(250, 200);
    play_note(200, 200);
    play_note(225, 350);
    play_note(250, 350);
    play_note(200, 350);
    play_note(150, 1000);
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
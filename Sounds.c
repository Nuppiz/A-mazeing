#include "Shared.h"
#include "Sounds.h"

uint16_t notes[11] = {277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};

void play_song()
{
    int i, counter;
    
    // init speaker
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    // loop through some notes, up...
    for (i = 10; i <= 4000; i++)
    {
        counter = (PIT_FREQ / i); // calculate frequency
        outportb(0x42, counter & 0xff); // LSB
        outportb(0x42, counter >> 8); // MSB
        delay(2); // wait for a bit
    }
    // down...
    for (i = 4000; i >= 10; i--)
    {
        counter = (PIT_FREQ / i); // calculate frequency
        outportb(0x42, counter & 0xff); // LSB
        outportb(0x42, counter >> 8); // MSB
        delay(2); // wait for a bit
    }
    // turn off the speaker
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}

void end_song()
{
    int i, counter;
    
    // init speaker
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    // loop through some notes, up...
    for (i = 10; i <= 1000; i++)
    {
        counter = (PIT_FREQ / i); // calculate frequency
        outportb(0x42, counter & 0xff); // LSB
        outportb(0x42, counter >> 8); // MSB
        delay(2); // wait for a bit
    }
    // turn off the speaker
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}

void sound_gameover()
{
    int counter;

    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    counter = (PIT_FREQ / 600);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(300);
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
    delay(100);
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    counter = (PIT_FREQ / 400);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(300);
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
    delay(100);
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    counter = (PIT_FREQ / 300);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(300);
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
    delay(100);
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    counter = (PIT_FREQ / 200);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(400);
    counter = (PIT_FREQ / 250);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(400);
    counter = (PIT_FREQ / 200);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(400);
    counter = (PIT_FREQ / 225);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(400);
    counter = (PIT_FREQ / 250);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(400);
    counter = (PIT_FREQ / 200);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(400);
    counter = (PIT_FREQ / 150);
    outportb(0x42, counter & 0xff);
    outportb(0x42, counter >> 8);
    delay(1000);
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}

void sound_death()
{
    int i, counter;
    
    // init speaker
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    // loop through some notes, up...
    for (i = 200; i <= 500; i++)
    {
        counter = (PIT_FREQ / i); // calculate frequency
        outportb(0x42, counter & 0xff); // LSB
        outportb(0x42, counter >> 8); // MSB
        delay(2); // wait for a bit
    }
    // turn off the speaker
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}

void sound_key()
{
    int i, counter;
    
    // init speaker
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);
    counter = (PIT_FREQ / 400); // calculate frequency
    outportb(0x42, counter & 0xff); // LSB
    outportb(0x42, counter >> 8); // MSB
    delay(25); // wait for a bit
    // turn off the speaker
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}

void sound_typing()
{
    int counter;
    
    // init speaker
    outportb(0x61, inportb(0x61) | 3);
    outportb(0x43, 0xB6);

    counter = (PIT_FREQ / 250); // calculate frequency
    outportb(0x42, counter & 0xff); // LSB
    outportb(0x42, counter >> 8); // MSB
    delay(10); // wait for a bit
        
    // turn off the speaker
    outportb(0x61, inportb(0x61) & (~3));
    outportb(0x43, 0x00);
}
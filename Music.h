#ifndef MUSIC_H
#define MUSIC_H

#include "Pitches.h"

#define MUSIC_LVL1    &Doom_music
#define MUSIC_LVL2    &mario_music
#define MUSIC_LVL3    &SW_music
#define MUSIC_WIN     &Odetojoy_music

//##############**"Mario underworld" **##############//
int MarioUW_note[] = {

    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_AS3, NOTE_AS4, 0, 0,
    NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_AS3, NOTE_AS4, 0, 0,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4, NOTE_DS3, NOTE_DS4, 0, 0,
    NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4, NOTE_DS3, NOTE_DS4, 0, 0,
    NOTE_DS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_GS3, NOTE_G3,
    NOTE_CS4, NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4, NOTE_GS4,
    NOTE_DS4, NOTE_B3, NOTE_AS3, NOTE_A3, NOTE_GS3, 0, 0, 0
};

int MarioUW_duration[] = {

    0, 12, 12, 12, 12, 12, 12, 16, 10,
    12, 12, 12, 12, 12, 12, 16, 10,
    12, 12, 12, 12, 12, 12, 16, 10,
    12, 12, 12, 12, 12, 12, 16, 10,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 18, 18, 18, 18, 18, 18, 10,
    10, 10, 10, 10, 10, 3, 3, 3

};

int Doom_note[] = {

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //1
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //5
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //9
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //13
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_FS3, NOTE_D3, NOTE_B2, NOTE_A3, NOTE_FS3, NOTE_B2, NOTE_D3, NOTE_FS3, NOTE_A3, NOTE_FS3, NOTE_D3, NOTE_B2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //17
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //21
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_B3, NOTE_G4, NOTE_B4,

    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2, //25
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3,

    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2, //29
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_A3, NOTE_F3, NOTE_D3, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_A3, NOTE_F3, NOTE_D3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //33
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //37
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_CS3, NOTE_CS3, NOTE_CS4, NOTE_CS3, NOTE_CS3, NOTE_B3, NOTE_CS3, NOTE_CS3, //41
    NOTE_A3, NOTE_CS3, NOTE_CS3, NOTE_G3, NOTE_CS3, NOTE_CS3, NOTE_GS3, NOTE_A3,
    NOTE_B2, NOTE_B2, NOTE_B3, NOTE_B2, NOTE_B2, NOTE_A3, NOTE_B2, NOTE_B2,
    NOTE_G3, NOTE_B2, NOTE_B2, NOTE_F3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //45
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_B3, NOTE_G3, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_E4, NOTE_B3, NOTE_G4, NOTE_B4,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //49
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //53
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_FS3, NOTE_DS3, NOTE_B2, NOTE_FS3, NOTE_DS3, NOTE_B2, NOTE_G3, NOTE_D3, NOTE_B2, NOTE_DS4, NOTE_DS3, NOTE_B2,

    // -/-

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //57
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //61
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_E4, NOTE_B3, NOTE_G3, NOTE_G4, NOTE_E4, NOTE_G3, NOTE_B3, NOTE_D4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_G3,  

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //65
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2, //69
    NOTE_F3, NOTE_A2, NOTE_A2, NOTE_DS3, NOTE_A2, NOTE_A2, NOTE_E3, NOTE_F3,
    NOTE_A2, NOTE_A2, NOTE_A3, NOTE_A2, NOTE_A2, NOTE_G3, NOTE_A2, NOTE_A2,
    NOTE_A3, NOTE_F3, NOTE_D3, NOTE_A3, NOTE_F3, NOTE_D3, NOTE_C4, NOTE_A3, NOTE_F3, NOTE_A3, NOTE_F3, NOTE_D3,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //73
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //77
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //81
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2,

    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2, //73
    NOTE_C3, NOTE_E2, NOTE_E2, NOTE_AS2, NOTE_E2, NOTE_E2, NOTE_B2, NOTE_C3,
    NOTE_E2, NOTE_E2, NOTE_E3, NOTE_E2, NOTE_E2, NOTE_D3, NOTE_E2, NOTE_E2,
    NOTE_B3, NOTE_G3, NOTE_E3, NOTE_B2, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_B3, NOTE_G3, NOTE_B3, NOTE_G3, NOTE_E3,

};

int Doom_duration[] ={

    0, 8, 8, 8, 8, 8, 8, 8, 8, //1
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //5
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //9
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //13
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    8, 8, 8, 8, 8, 8, 8, 8, //17
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //21
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    8, 8, 8, 8, 8, 8, 8, 8, //25
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //29
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    8, 8, 8, 8, 8, 8, 8, 8, //33
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //37
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //41
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //45
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    8, 8, 8, 8, 8, 8, 8, 8, //49
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //53
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    // -/-

    8, 8, 8, 8, 8, 8, 8, 8, //57
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //61
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,  

    8, 8, 8, 8, 8, 8, 8, 8, //65
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //69
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

    8, 8, 8, 8, 8, 8, 8, 8, //73
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //77
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //81
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 48,

    8, 8, 8, 8, 8, 8, 8, 8, //85
    8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,

};

int SW_note[] = {

    NOTE_AS2, NOTE_AS2, NOTE_AS2,//1
    NOTE_F3, NOTE_C4,
    NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F4, NOTE_C4,  
    NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F4, NOTE_C4,  
    NOTE_AS3, NOTE_A3, NOTE_AS3, NOTE_G3, NOTE_C3, NOTE_C3, NOTE_C3,
    NOTE_F3, NOTE_C4,
    NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F4, NOTE_C4,  
    
    NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F4, NOTE_C4, //8  
    NOTE_AS3, NOTE_A3, NOTE_AS3, NOTE_G3, NOTE_C3, NOTE_C3, 
    NOTE_D3, NOTE_D3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F3,
    NOTE_F3, NOTE_G3, NOTE_A3, NOTE_G3, NOTE_D3, NOTE_E3,NOTE_C3, NOTE_C3,
    NOTE_D3, NOTE_D3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F3,
    
    NOTE_C4, NOTE_G3, NOTE_G3, 0, NOTE_C3,//12
    NOTE_D3, NOTE_D3, NOTE_AS3, NOTE_A3, NOTE_G3, NOTE_F3,
    NOTE_F3, NOTE_G3, NOTE_A3, NOTE_G3, NOTE_D3, NOTE_E3,NOTE_C4, NOTE_C4,
    NOTE_F4, NOTE_DS4, NOTE_CS4, NOTE_C4, NOTE_AS3, NOTE_GS3, NOTE_G3, NOTE_F3,
    NOTE_C4
  
};

int SW_duration[] = {

    0, 8, 8, 8,//1
    32, 32,
    8, 8, 8, 32, 16,  
    8, 8, 8, 32, 16,  
    8, 8, 8, 32, 8, 8, 8,
    32, 32,
    8, 8, 8, 32, 16,  
    
    8, 8, 8, 32, 16, //8 
    8, 8, 8, 32, 12, 4, 
    24, 8, 8, 8, 8, 8,
    8, 8, 8, 16, 8, 16, 12, 4,
    24, 8, 8, 8, 8, 8,
    
    16, 8, 32, 8, 8, //13
    24, 8, 8, 8, 8, 8,
    8, 8, 8, 16, 8, 16, 12, 4,
    16, 8, 16, 8, 16, 8, 16, 8,
    64
};

int Odetojoy_note[] = {
  
  NOTE_E4,  NOTE_E4,  NOTE_F4,  NOTE_G4,//1
  NOTE_G4,  NOTE_F4,  NOTE_E4,  NOTE_D4,
  NOTE_C4,  NOTE_C4,  NOTE_D4,  NOTE_E4,
  NOTE_E4, NOTE_D4,  NOTE_D4,

  NOTE_E4,  NOTE_E4,  NOTE_F4,  NOTE_G4,//4
  NOTE_G4,  NOTE_F4,  NOTE_E4,  NOTE_D4,
  NOTE_C4,  NOTE_C4,  NOTE_D4,  NOTE_E4,
  NOTE_D4,  NOTE_C4,  NOTE_C4,

  NOTE_D4,  NOTE_D4,  NOTE_E4,  NOTE_C4,//8
  NOTE_D4,  NOTE_E4,  NOTE_F4,  NOTE_E4, NOTE_C4,
  NOTE_D4,  NOTE_E4,  NOTE_F4,  NOTE_E4, NOTE_D4,
  NOTE_C4,  NOTE_D4,  NOTE_G3,

  NOTE_E4,  NOTE_E4,  NOTE_F4,  NOTE_G4,//12
  NOTE_G4,  NOTE_F4,  NOTE_E4,  NOTE_D4,
  NOTE_C4,  NOTE_C4,  NOTE_D4,  NOTE_E4,
  NOTE_D4,  NOTE_C4,  NOTE_C4
  
};

int Odetojoy_duration[] = {
 
    16, 16, 16, 16,//1
    16, 16, 16, 16,
    16, 16, 16, 16,
    24, 8, 32,

    16, 16, 16, 16,//4
    16, 16, 16, 16,
    16, 16, 16, 16,
    24, 8, 32,

    16, 16, 16, 16,//8
    16, 8, 8, 16, 16,
    16, 8, 8, 16, 16,
    16, 16, 32,

    16, 16, 16, 16,//12
    16, 16, 16, 16,
    16, 16, 16, 16,
    24, 8, 32
 
};

int MarioUW_size = sizeof(MarioUW_note)/sizeof(MarioUW_note[0]);
int Doom_size = sizeof(Doom_note)/sizeof(Doom_note[0]);
int SW_size = sizeof(SW_note)/sizeof(SW_note[0]);
int Odetojoy_size = sizeof(Odetojoy_note)/sizeof(Odetojoy_note[0]);

typedef struct
{
    int* notes;
    int* duration;
    int size;
    uint8_t loop;
} note_sequence;

note_sequence mario_music =
{
    MarioUW_note,
    MarioUW_duration,
    0,
    TRUE
};

note_sequence Doom_music =
{
    Doom_note,
    Doom_duration,
    0,
    TRUE
};

note_sequence SW_music =
{
    SW_note,
    SW_duration,
    0,
    TRUE
};

note_sequence Odetojoy_music =
{
    Odetojoy_note,
    Odetojoy_duration,
    0,
    TRUE
};

#endif /* MUSIC_H */
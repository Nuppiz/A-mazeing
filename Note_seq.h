#ifndef NOTE_SEQ_H
#define NOTE_SEQ_H

#include "Pitches.h"

#define MUSIC_LVL1    &Doom_music
#define MUSIC_LVL2    &mario_music
#define MUSIC_LVL3    &SW_music
#define MUSIC_WIN     &Odetojoy_music

#define SEQ_MUSIC    0
#define SEQ_EFFECT   1

#define PITCH_INTERVAL  10

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

    0, 147, 147, 147, 147, 147, 147, 196, 123,
    147, 147, 147, 147, 147, 147, 196, 123,
    147, 147, 147, 147, 147, 147, 196, 123,
    147, 147, 147, 147, 147, 147, 196, 123,
    74, 74, 74, 74, 74, 74, 74, 74,
    74, 221, 221, 221, 221, 221, 221, 123,
    123, 123, 123, 123, 123, 37, 37, 37

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

    0, 98, 98, 98, 98, 98, 98, 98, 98, //1
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //5
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //9
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //13
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

    98, 98, 98, 98, 98, 98, 98, 98, //17
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //21
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

    98, 98, 98, 98, 98, 98, 98, 98, //25
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //29
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

    98, 98, 98, 98, 98, 98, 98, 98, //33
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //37
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //41
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //45
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

    98, 98, 98, 98, 98, 98, 98, 98, //49
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //53
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

    // -/-

    98, 98, 98, 98, 98, 98, 98, 98, //57
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //61
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,  

    98, 98, 98, 98, 98, 98, 98, 98, //65
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //69
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

    98, 98, 98, 98, 98, 98, 98, 98, //73
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //77
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //81
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 500,

    98, 98, 98, 98, 98, 98, 98, 98, //85
    98, 98, 98, 98, 98, 98, 98, 98,
    98, 98, 98, 98, 98, 98, 98, 98,
    72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72, 72,

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

    0, 140, 140, 140,//1
    392, 392,
    140, 140, 140, 392, 196,  
    140, 140, 140, 392, 196,  
    140, 140, 140, 392, 140, 140, 140,
    392, 392,
    140, 140, 140, 392, 196,  
    
    140, 140, 140, 392, 196, //8 
    140, 140, 140, 392, 147, 98, 
    294, 140, 140, 140, 140, 140,
    140, 140, 140, 196, 140, 196, 147, 98,
    294, 140, 140, 140, 140, 140,
    
    196, 140, 392, 140, 140, //13
    294, 140, 140, 140, 140, 140,
    140, 140, 140, 196, 140, 196, 147, 98,
    196, 140, 196, 140, 196, 140, 196, 140,
    784
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
 
    0, 196, 196, 196, 196,//1
    196, 196, 196, 196,
    196, 196, 196, 196,
    294, 98, 392,

    196, 196, 196, 196,//4
    196, 196, 196, 196,
    196, 196, 196, 196,
    294, 98, 392,

    196, 196, 196, 196,//8
    196, 98, 98, 196, 196,
    196, 98, 98, 196, 196,
    196, 196, 392,

    196, 196, 196, 196,//12
    196, 196, 196, 196,
    196, 196, 196, 196,
    294, 98, 392
 
};

int Explosion_note[] = {
  NOTE_GS6, NOTE_A6, NOTE_G7, NOTE_GS7, NOTE_GS7, NOTE_G7, NOTE_A6, NOTE_GS6
};

int Explosion_duration[] = {
    0, 4, 4, 4, 4, 4, 4, 4, 4
};

int Pickup_note[] = {
  NOTE_C5, NOTE_G4, NOTE_C6, 0, NOTE_C6
};

int Pickup_duration[] = {
    0, 8, 8, 4, 4, 4
};

int Door_open_note[] = {
  NOTE_E5, NOTE_C5, NOTE_C6
};

int Door_open_duration[] = {
    0, 8, 8, 4
};

int Door_shut_note[] = {
  NOTE_B1, NOTE_A1
};

int Door_shut_duration[] = {
    0, 4, 4
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
    int16_t pitch_change;
    uint8_t loop;
    uint8_t type;
} note_sequence;

note_sequence mario_music =
{
    MarioUW_note,
    MarioUW_duration,
    0,
    0,
    TRUE,
    SEQ_MUSIC
};

note_sequence Doom_music =
{
    Doom_note,
    Doom_duration,
    0,
    0,
    TRUE,
    SEQ_MUSIC
};

note_sequence SW_music =
{
    SW_note,
    SW_duration,
    0,
    0,
    TRUE,
    SEQ_MUSIC
};

note_sequence Odetojoy_music =
{
    Odetojoy_note,
    Odetojoy_duration,
    0,
    0,
    FALSE,
    SEQ_MUSIC
};

note_sequence explosion =
{
    Explosion_note,
    Explosion_duration,
    8,
    10,
    FALSE,
    SEQ_EFFECT
};

note_sequence pickup =
{
    Pickup_note,
    Pickup_duration,
    5,
    10,
    FALSE,
    SEQ_EFFECT
};

note_sequence door_open =
{
    Door_open_note,
    Door_open_duration,
    3,
    10,
    FALSE,
    SEQ_EFFECT
};

note_sequence door_shut =
{
    Door_shut_note,
    Door_shut_duration,
    1,
    10,
    FALSE,
    SEQ_EFFECT
};

#endif /* NOTE_SEQ_H */

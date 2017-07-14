#pragma once

#include <SDL_mixer.h>

typedef struct {
	Mix_Chunk* source;
} AU_Sound;

//Load a wav file in as a sound clip
AU_Sound au_sound_load(char* path);
//Set the volume from a value from 0 to 128
void au_sound_set_volume(AU_Sound, int);
//Play a sound a given number of times (0 for loop) 
//Returns the sound channel ID, use for other 
//Once a sound finishes, the id is invalid and may be used for other sounds
int au_sound_play(AU_Sound, int, int);
//Pause a playing sound
void au_sound_pause(int);
//Play a paused sound
void au_sound_resume(int);
//Stop a playing sound
void au_sound_stop(int);
//Fade out a playing sound
void au_sound_fadeout(int, int);
//Checks if a sound is playing
void au_sound_is_playing(int);
//Checks if a sound is paused
void au_sound_is_paused(int);
//Destroy a sound clip
void au_sound_destroy(AU_Sound);

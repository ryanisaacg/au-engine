#pragma once

#include <SDL_mixer.h>

typedef struct {
	Mix_Music* source;
} AU_Music;

//Load music from a file
AU_Music au_music_load(char* path);
//Set the voluem of music
void au_music_set_volume(int);
//Play a music track
void au_music_play(AU_Music, int);
//Pause a music track
void au_music_pause();
//Resume a music track
void au_music_resume();
//Stop the music
void au_music_stop();
//Fade out music
void au_music_fadeout(int);
//Destroy a music track
void au_music_destroy(AU_Music);

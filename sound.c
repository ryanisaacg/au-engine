#include "sound.h"

AU_Sound au_sound_load(char* path) {
	return (AU_Sound) { Mix_LoadWAV(path) };
}

void au_sound_set_volume(AU_Sound sound, int volume) {
	Mix_VolumeChunk(sound.source, volume);
}

int au_sound_play(AU_Sound sound, int times) {
	int loops = times - 1;
	return Mix_PlayChannel(-1, sound.source, loops);
}

void au_sound_pause(int id) {
	Mix_Pause(id);
}

void au_sound_resume(int id) {
	Mix_Resume(id);
}

void au_sound_stop(int id) {
	Mix_HaltChannel(id);
}

void au_sound_fadeout(int id, int ms) {
	Mix_FadeOutChannel(id, ms);
}

bool au_sound_is_playing(int id) {
	return Mix_Playing(id);
}

bool au_sound_is_paused(int id) {
	return Mix_Paused(id);
}

void au_sound_destroy(AU_Sound snd) {
	Mix_FreeChunk(snd.source);
}

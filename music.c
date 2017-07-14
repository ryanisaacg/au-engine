#include "music.h"

AU_Music au_music_load(char* path) {
	return (AU_Music) { Mix_LoadMUS(path) };
}

void au_music_set_volume(int vol) {
	Mix_VolumeMusic(vol);
}

void au_music_play(AU_Music msc, int times) {
	int loops = times - 1;
	Mix_PlayMusic(msc.source, loops);
}

void au_music_pause() {
	Mix_PauseMusic();
}

void au_music_resume() {
	Mix_ResumeMusic();
}

void au_music_stop() {
	Mix_HaltMusic();
}

void au_music_fadeout(int ms) {
	Mix_FadeOutMusic(ms);
}

void au_music_destroy(AU_Music msc) {
	Mix_FreeMusic(msc.source);
}

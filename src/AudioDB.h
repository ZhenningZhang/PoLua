#pragma once

#include <string>
#include <unordered_map>

#include "SDL_mixer.h"
#include "AudioHelper.h"

class AudioDB
{
public:
	static inline std::unordered_map<std::string, Mix_Chunk*> loaded_audio;
	static inline std::unordered_map<std::string, bool> playing_audio;

	static void LoadAudio(std::string audio_name);

	static void Play(int channel, std::string clip_name, bool does_loop);
	static void Halt(int channel);
	static void SetVolume(int channel, int volume);

	AudioDB() {
		SDL_Init(SDL_INIT_AUDIO);
		AudioHelper::Mix_OpenAudio498(44100, MIX_DEFAULT_FORMAT, 1, 2048);
		AudioHelper::Mix_AllocateChannels498(50); 
	}
};


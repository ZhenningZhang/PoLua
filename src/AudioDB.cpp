#include "AudioDB.h"

#include <filesystem>

void AudioDB::LoadAudio(std::string audio_name)
{
	if (loaded_audio.find(audio_name) != loaded_audio.end())
		return;

	// Sometimes std::string, sometimes std::filesystem::path
	std::string audio_path = "resources/audio/" + audio_name;

	audio_path += std::filesystem::exists(audio_path + ".wav") ? ".wav" : ".ogg";

	if (!std::filesystem::exists(audio_path)) {
		std::cout << "error: failed to play audio clip " + audio_name;
		exit(0);
	}

	loaded_audio[audio_name] = AudioHelper::Mix_LoadWAV498(audio_path.c_str());
}

void AudioDB::Play(int channel, std::string clip_name, bool does_loop)
{
	LoadAudio(clip_name);

	if (does_loop)
		AudioHelper::Mix_PlayChannel498(channel, loaded_audio[clip_name], -1);
	else
		AudioHelper::Mix_PlayChannel498(channel, loaded_audio[clip_name], 0);
}

void AudioDB::Halt(int channel)
{
	AudioHelper::Mix_HaltChannel498(channel);
}

void AudioDB::SetVolume(int channel, int volume)
{
	AudioHelper::Mix_Volume498(channel, volume);
}

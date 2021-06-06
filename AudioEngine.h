#pragma once
#include "Sound.h"


class AudioEngine {

public:
	AudioEngine() {}
	void update();

	void playSound(const std::string& soundName, const float volume = 100.0f, const float pitch = 1.0f, const bool repeat = false);
	void playMusic(const std::string& musicName, const float volume = 100.0f, const float pitch = 1.0f, const bool repeat = true );
	sf::SoundSource::Status getStatusOfSound(const std::string& soundName);


	std::shared_ptr<sf::Music> currentSong = std::shared_ptr<sf::Music>(nullptr);
	//std::vector<Sound> currentSounds = std::vector<Sound>();
	std::map<std::string, Sound> currentSounds = std::map<std::string, Sound>();
};


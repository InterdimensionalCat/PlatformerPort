#include "stdafx.h"
#include "AudioEngine.h"
#include "AudioRegistry.h"


void AudioEngine::update() {

	//currentSounds.erase(std::remove_if(currentSounds.begin(), currentSounds.end(),
	//	[](auto& elt) {
	//		return elt.getStatus() != sf::SoundSource::Status::Playing;
	//	}), currentSounds.end());

	std::erase_if(currentSounds, [](const auto& elt) {
		auto const& [key, value] = elt;
		return value.getStatus() != sf::SoundSource::Status::Playing;
		}
	);
}

sf::SoundSource::Status AudioEngine::getStatusOfSound(const std::string& soundName) {
	auto found = currentSounds.find(soundName);
	if (found != currentSounds.end()) {
		return found->second.getStatus();
	}
	else {
		return sf::SoundSource::Status::Stopped;
	}
}

void AudioEngine::playSound(const std::string& soundName, const float volume, const float pitch, const bool repeat) {
	currentSounds.emplace(soundName, Sound(soundName));
	//currentSounds.emplace_back(Sound(soundName));
	//auto& soundToPlay = currentSounds.at(currentSounds.size() - 1);
	Sound& soundToPlay = currentSounds.find(soundName)->second;
	soundToPlay.setVolume(volume);
	soundToPlay.setPitch(pitch);
	soundToPlay.setLoop(repeat);
	soundToPlay.play();
}

void AudioEngine::playMusic(const std::string& musicName, const float volume, const float pitch, const bool repeat) {
	if (currentSong.get() != nullptr) {
		currentSong->stop();
	}

	currentSong = getMusicFromName(musicName);

	currentSong->setVolume(volume);
	currentSong->setPitch(pitch);
	currentSong->setLoop(repeat);
	currentSong->play();
}
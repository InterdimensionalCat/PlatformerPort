#include "stdafx.h"
#include "Sound.h"
#include "AudioRegistry.h"


Sound::Sound(const std::string& buffername) {
	buffer = std::shared_ptr<sf::SoundBuffer>(getBufferFromName(buffername));
	sound.setBuffer(*buffer);
}

void Sound::setPitch(const float pitch) {
	sound.setPitch(pitch);
}

void Sound::setVolume(const float volume) {
	sound.setVolume(volume);
}

void Sound::play() {
	sound.play();
}

sf::SoundSource::Status Sound::getStatus() const {
	return sound.getStatus();
}

void Sound::setLoop(const bool loop) {
	sound.setLoop(loop);
}

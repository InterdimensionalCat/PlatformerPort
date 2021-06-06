#pragma once


class AudioManager;


//creates a copy of the buffer
std::shared_ptr<sf::SoundBuffer> getBufferFromName(const std::string& buffername);
std::shared_ptr<sf::Music> getMusicFromName(const std::string& musicname);
void clearAudioRegistry();
void RegisterAudio();


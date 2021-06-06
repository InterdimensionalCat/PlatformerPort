#pragma once

class Sound
{
public:
	Sound(const std::string& soundname);
	void setPitch(const float pitch);
	void setVolume(const float volume);
	void play();
	void setLoop(const bool loop);
	sf::SoundSource::Status getStatus() const;

private:
	std::shared_ptr<sf::SoundBuffer> buffer;
	sf::Sound sound;
};


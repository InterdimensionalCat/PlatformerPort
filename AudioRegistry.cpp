#include "stdafx.h"
#include "AudioRegistry.h"

static std::map<std::string, std::shared_ptr<sf::SoundBuffer>> bufferMap = std::map<std::string, std::shared_ptr<sf::SoundBuffer>>();
static std::map<std::string, std::shared_ptr<sf::Music>> musicMap = std::map<std::string, std::shared_ptr<sf::Music>>();


std::shared_ptr<sf::SoundBuffer> getBufferFromName(const std::string& buffername) {
	auto found = bufferMap.find(buffername);
	if (found != bufferMap.end()) {
		std::shared_ptr<sf::SoundBuffer> ptr = std::make_shared<sf::SoundBuffer>();
		*ptr = *found->second;
		return std::shared_ptr<sf::SoundBuffer>(ptr);
	}
	else {
		throw std::exception();
		return found->second;
	}
}

std::shared_ptr<sf::Music> getMusicFromName(const std::string& musicname) {
	auto found = musicMap.find(musicname);
	if (found != musicMap.end()) {
		return found->second;
	}
	else {
		throw std::exception();
		return found->second;
	}
}

void clearAudioRegistry() {
	bufferMap.clear();
	musicMap.clear();
}


void registerSound(const std::string& soundname) {

	fs::path filepath(fs::current_path());
	filepath /= "resources";
	filepath /= "sounds";
	filepath /= soundname;
	filepath += ".wav";

	std::shared_ptr<sf::SoundBuffer> buffer = std::make_shared<sf::SoundBuffer>();
	if (!buffer->loadFromFile(filepath.string())) {
		throw std::exception();
	}

	bufferMap.emplace(soundname, buffer);

	Logger::get() << "Sound file " << soundname << " loaded\n";
}

void registerMusic(const std::string& musicname) {

	fs::path filepath(fs::current_path());
	filepath /= "resources";
	filepath /= "music";
	filepath /= musicname;
	filepath += ".wav";

	std::shared_ptr<sf::Music> music = std::make_shared<sf::Music>();
	if (!music->openFromFile(filepath.string())) {
		throw std::exception();
	}

	musicMap.emplace(musicname, music);

	Logger::get() << "Music file " << musicname << " loaded\n";
}

void RegisterAudio() {
	std::vector<std::string> audioFileNames{
		"PlayerDash1" , "PlayerDash2" , "PlayerDash3" ,
		"PlayerDash4", "PlayerDead", "PlayerFall1",
		"PlayerJump1", "PlayerJump2", "BoopDeath",
		"BoopDeath2", "PlayerWin"
	};

	for (auto& str : audioFileNames) {
		registerSound(str);
	}

	registerMusic("FlatZone");
}

#pragma once
class LevelData
{
public:
	LevelData(const std::string& levelname, const std::string& tilesetname) : levelname(levelname), tilesetname(tilesetname) {}
	std::string levelname;
	std::string tilesetname;
};


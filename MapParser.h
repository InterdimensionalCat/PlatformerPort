#pragma once

class Scene;

class MapParser {
public:
	void parseMap(const std::string& mapName, Scene& scene);
private:
	void loadActors(tmx::ObjectGroup& layer, Scene& scene);
	void loadParallax(tmx::Map& map, Scene& scene);
};



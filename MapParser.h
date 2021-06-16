#pragma once

class Scene;

class MapParser {
public:
	static void parseMap(const std::string& mapName, Scene& scene);
private:
	static void loadActors(tson::Layer& layer, Scene& scene);
	static void loadParallax(tson::Map& map, Scene& scene);
};



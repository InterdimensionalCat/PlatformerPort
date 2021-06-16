#pragma once
#include "Scene.h"

class ActorData {
public:
	ActorData(const json& mapData, Scene* scene) : mapData(mapData), scene(scene) {


		Logger::get() << mapData << "\n";

		//if (actorname != "") {
		//	std::filesystem::path staticJson(std::filesystem::current_path());
		//	staticJson /= "resources";
		//	staticJson /= "actor";
		//	staticJson /= "static";
		//	staticJson /= actorname;
		//	staticJson += ".json";

		//	std::ifstream st(staticJson.string().c_str());
		//	if (st.is_open()) {
		//		staticData = std::make_unique<json>();
		//		st >> *staticData;
		//	}
		//}


		//if (variantname != "") {
		//	std::filesystem::path dynamicJson(std::filesystem::current_path());
		//	dynamicJson /= "resources";
		//	dynamicJson /= "actor";
		//	dynamicJson /= "dynamic";
		//	dynamicJson /= variantname;
		//	dynamicJson += ".json";

		//	std::ifstream dyn(dynamicJson.string().c_str());
		//	if (dyn.is_open()) {
		//		dynamicData = std::make_unique<json>();
		//		dyn >> *dynamicData.value();
		//	}
		//}
	}


	//WARNING: NO TWO KEYS CAN HAVE THE SAME NAME IN AN ACTOR DATA OBJECT, FINDER WILL STOP AFTER THE FIRST FOUND INSTANCE
	template<typename SearchType>
	SearchType findKey(const std::string& keyval) const {
		std::optional<SearchType> foundVal;

		foundVal = findKey<SearchType>(mapData, keyval);

		//if (!foundVal.has_value()) {
		//	foundVal = findKey<SearchType>(*staticData, keyval);
		//}

		//if (!foundVal.has_value() && dynamicData.has_value()) {
		//	foundVal = findKey<SearchType>(*dynamicData.value(), keyval);
		//}

		return foundVal.value();
	}

	Scene* scene;

private:
	template<typename SearchType>
	std::optional<SearchType> findKey(const json& j, const std::string& keyval) const {
		std::optional<SearchType> returnval{};

		for (auto& [key, val] : j.items()) {
			if (val.is_object()) {
				returnval = findKey<SearchType>(val, keyval);
			}
			else {
				if (keyval == key) {
					returnval = val.get<SearchType>();
				}
			}

			if (returnval.has_value()) break;
		}

		return returnval;
	}

	//json staticData;
	//json dynamicData;
	json mapData;
};


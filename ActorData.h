#pragma once
#include "Scene.h"

void inline addPropertyToJson(const tmx::Property& property, json& val, const std::string& name) {
	switch (property.getType()) {
	case tmx::Property::Type::Boolean:
		val[name] = property.getBoolValue();
		break;
	case tmx::Property::Type::Float:
		val[name] = property.getFloatValue();
		break;
	case tmx::Property::Type::Int:
		val[name] = property.getIntValue();
		break;
	case tmx::Property::Type::String:
		val[name] = property.getStringValue();
		break;
	default:
		val[name] = property.getStringValue();
	}
}

void inline createPropertyHeirarchy(std::string propname, const tmx::Property& property, json& val) {
	auto slash = propname.find("/");

	if (slash == std::string::npos) {
		addPropertyToJson(property, val, propname);
		return;
	}
	else {
		auto tiername = propname.substr(0, slash);
		propname = propname.substr(slash + 1, propname.size());
		if (val.find(tiername) == val.end()) {
			val[tiername] = json();
		}
		createPropertyHeirarchy(propname, property, val[tiername]);
	}
}

void inline objToJson(const tmx::Object& obj, std::shared_ptr<json> mapData) {

	auto& properties = obj.getProperties();

	auto& x = obj.getPosition().x;
	auto& y = obj.getPosition().y;

	(*mapData)["Transform"]["x"] = toMeters(x);
	(*mapData)["Transform"]["y"] = toMeters(y) - 1.0f;

	for (auto& property : properties) {
		std::string propname = property.getName();
		createPropertyHeirarchy(propname, property, *mapData);
	}
}


class ActorData {
public:

	ActorData(const std::string& actorname, const std::string& variantname) {

		std::shared_ptr<json> actorjson;
		std::shared_ptr<json> variantjson;

		fs::path filepath(fs::current_path());
		filepath /= "resources";
		filepath /= "actors";
		filepath /= actorname;
		filepath += ".json";

		actorjson = std::make_shared<json>();
		std::ifstream actorfile(filepath);

		if (!actorfile.is_open()) {
#ifdef _DEBUG
			throw std::exception();
#endif
		}

		actorfile >> (*actorjson);

		if (variantname != "") {
			filepath = fs::current_path();
			filepath /= "resources";
			filepath /= "actors";
			filepath /= actorname + "_" + variantname;
			filepath += ".json";

			variantjson = std::make_shared<json>();
			std::ifstream variantfile(filepath);

			if (!variantfile.is_open()) {
#ifdef _DEBUG
				throw std::exception();
#endif
			}

			variantfile >> (*variantjson);
		}

		data = std::make_shared<json>();
		data->update(*actorjson);
		if (variantjson.get() != nullptr) {
			data->update(*variantjson);
		}
	}

	ActorData(const std::string& actorname, const std::string& variantname, const tmx::Object& mapdata) {
		std::shared_ptr<json> actorjson;
		std::shared_ptr<json> variantjson;
		std::shared_ptr<json> mapjson;

		fs::path filepath(fs::current_path());
		filepath /= "resources";
		filepath /= "actors";
		filepath /= actorname;
		filepath += ".json";

		actorjson = std::make_shared<json>();
		std::ifstream actorfile(filepath);

		if (!actorfile.is_open()) {
#ifdef _DEBUG
			throw std::exception();
#endif
		}

		actorfile >> (*actorjson);

		if (variantname != "") {
			filepath = fs::current_path();
			filepath /= "resources";
			filepath /= "actors";
			filepath /= actorname + "_" + variantname;
			filepath += ".json";

			variantjson = std::make_shared<json>();
			std::ifstream variantfile(filepath);

			if (!variantfile.is_open()) {
#ifdef _DEBUG
				throw std::exception();
#endif
			}

			variantfile >> (*variantjson);
		}

		mapjson = std::make_shared<json>();
		objToJson(mapdata, mapjson);


		data = std::make_shared<json>();
		data->update(*actorjson);
		if (variantjson.get() != nullptr) {
			data->update(*variantjson);
		}
		data->update(*mapjson);
	}

	const std::shared_ptr<json> get() const {
		return data;
	}

private:
	std::shared_ptr<json> data;
};


#include "stdafx.h"
#include "Components.h"

typedef std::function<void(Scene*, std::shared_ptr<ActorEntry>, std::shared_ptr<json>)> Func;

std::vector<Func> generateJsonFunctions = std::vector<Func>();
std::vector<Func> generateFromJsonFunctions = std::vector<Func>();

void registerComponentsFunction() {
	for (auto& compfunc : RegisterComponentsClass::registerComponents) {
		compfunc();
	}

	RegisterComponentsClass::registerComponents.clear();
}


ActionState StateFromStr(const std::string& str) {
	if (str == "GroundStill") {
		return  ActionState::GroundStill;
	}

	if (str == "GroundMove") {
		return  ActionState::GroundMove;
	}

	if (str == "GroundTurn") {
		return  ActionState::GroundTurn;
	}

	return ActionState::Airborne;
}

std::string StateToStr(const ActionState& state) {
	switch (state) {
	case ActionState::GroundStill:
		return "GroundStill";
	case ActionState::GroundMove:
		return "GroundMove";
	case ActionState::GroundTurn:
		return "GroundTurn";
	case ActionState::Airborne:
		return "Airborne";
	}

	return "Airborne";
}

//PlatformType PtypeFromStr(const std::string& str) {
//	if (str == "Vertical") {
//		return  PlatformType::Vertical;
//	}
//
//	if (str == "Horizontal") {
//		return PlatformType::Horizontal;
//	}
//
//	if (str == "Falling") {
//		return PlatformType::Falling;
//	}
//}
//std::string PtypeToStr(const PlatformType& type) {
//	switch (type) {
//	case PlatformType::Vertical:
//		return "Vertical";
//	}
//}

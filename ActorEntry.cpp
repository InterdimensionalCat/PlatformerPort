#include "stdafx.h"
#include "ActorEntry.h"
#include "ActorData.h"

ActorEntry::ActorEntry(const size_t index) : index(index), id(ActorID()), version((unsigned long long)0l), mask(std::vector<bool>()) {}

void ActorEntry::createNewEntry(const std::string& typeName, const std::string& variantName, 
	bool respawnable) {

	ActorEntry::respawnable = respawnable;
	setActive(true);

	id = ActorID(typeName, variantName);
	version += 1;

	data = std::make_shared<ActorData>(typeName, variantName);
}

void ActorEntry::createNewEntry(const std::string& typeName, const std::string& variantName, 
	const tmx::Object& obj, bool respawnable) {

	ActorEntry::respawnable = respawnable;
	setActive(true);

	id = ActorID(typeName, variantName);
	version += 1;

	data = std::make_shared<ActorData>(typeName, variantName, obj);
}

size_t ActorEntry::getIndex() const {
	return index;
}

unsigned long long ActorEntry::getVersion() const {
	return version;
}

const ActorID& ActorEntry::getActorId() const {
	return id;
}

bool ActorEntry::isIdValid() const {
	return id.isIdValid();
}

bool ActorEntry::hasComponent(const int componentId) const {
	if (componentId < mask.size()) {
		return mask.at(componentId);
	}
	else {
		return false;
	}
}

void ActorEntry::setComponent(const int componentId) {
	if (componentId < mask.size()) {
		mask.at(componentId) = true;
	}
	else {
		mask.resize(componentId + 1);
		mask.at(componentId) = true;
	}
}

void ActorEntry::removeComponent(const int componentId) {
	if (componentId < mask.size()) {
		mask.at(componentId) = false;
	}
	else {
		mask.resize(componentId + 1);
		mask.at(componentId) = false;
	}
}

void ActorEntry::resetEntry() {
	id = ActorID();
	mask.clear();
	data.reset();
	setActive(false);
}
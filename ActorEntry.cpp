#include "stdafx.h"
#include "ActorEntry.h"


ActorEntry::ActorEntry(const size_t index) : index(index), id(ActorID()), version((unsigned long long)0l), mask(ComponentMask()) {}

void ActorEntry::createNewEntry(const std::string& typeName, const std::string& variantName) {
	id = ActorID(typeName, variantName);
	version += 1;
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
	return mask.test(componentId);
}

void ActorEntry::setComponent(const int componentId) {
	mask.set(componentId);
}

void ActorEntry::removeComponent(const int componentId) {

}

void ActorEntry::resetEntry() {
	id = ActorID();
	mask.reset();
}
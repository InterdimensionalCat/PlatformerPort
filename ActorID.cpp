#include "stdafx.h"
#include "ActorID.h"

ActorID::ActorID() : typeName(""), variantName("") {}

bool ActorID::isIdValid() const {
	return !(typeName == "" && variantName == "");
}

bool ActorID::operator==(const ActorID& rhs) const {
	return compare(rhs) == 0;
}
bool ActorID::operator!=(const ActorID& rhs) const {
	return compare(rhs) != 0;
}
bool ActorID::operator<(const ActorID& rhs) const {
	return compare(rhs) < 0;
}
bool ActorID::operator>(const ActorID& rhs) const {
	return compare(rhs) > 0;
}
bool ActorID::operator<=(const ActorID& rhs) const {
	return compare(rhs) <= 0;
}
bool ActorID::operator>=(const ActorID& rhs) const {
	return compare(rhs) >= 0;
}
int ActorID::compare(const ActorID& other) const {
	auto typecmp = typeName.compare(other.typeName);
	if (typecmp == 0) {
		return variantName.compare(other.variantName);
	}
	else {
		return typecmp;
	}
}


ActorID::ActorID(const std::string& typeName, const std::string& variantName) : typeName(typeName), variantName(variantName) {}

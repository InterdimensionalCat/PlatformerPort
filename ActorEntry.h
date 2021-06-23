#pragma once
#include "ActorID.h"

constexpr int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;


class ActorEntry
{
public:

	ActorEntry(const size_t index);
	void createNewEntry(const std::string& typeName, const std::string& variantName);
	void resetEntry();

	size_t getIndex() const;
	unsigned long long getVersion() const;
	const ActorID& getActorId() const;
	bool isIdValid() const;

	bool hasComponent(const int componentId) const;
	void setComponent(const int componentId);
	void removeComponent(const int componentId);
private:

	//TODO keep ActorData for the current entry
	//mark entities as respawnable or nonrespawnable,
	//so that respawnable entities can respawn from their retained entitydata

	ActorID id;
	unsigned long long version;
	const size_t index;

	ComponentMask mask;
};


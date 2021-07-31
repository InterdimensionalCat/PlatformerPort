#pragma once
#include "ActorID.h"


class ActorData;

class ActorEntry
{
public:

	ActorEntry(const size_t index);

	~ActorEntry() {
		id = ActorID();
		mask.clear();
		data.reset();
	}

	void createNewEntry(const std::string& typeName, const std::string& variantName, 
		bool respawnable = false);
	void createNewEntry(const std::string& typeName, const std::string& variantName, 
		const tmx::Object& obj, bool respawnable = false);
	void resetEntry();

	size_t getIndex() const;
	unsigned long long getVersion() const;
	const ActorID& getActorId() const;
	bool isIdValid() const;

	bool hasComponent(const int componentId) const;
	void setComponent(const int componentId);
	void removeComponent(const int componentId);

	const std::shared_ptr<ActorData> getData() const {
		return data;
	}

	const bool isRespawnable() const {
		return respawnable;
	}

	const bool isActive() const {
		return active;
	}

	void setActive(bool isactive) {
		active = isactive;
	}

private:

	//TODO keep ActorData for the current entry
	//mark entities as respawnable or nonrespawnable,
	//so that respawnable entities can respawn from their retained entitydata

	ActorID id;
	unsigned long long version;
	const size_t index;

	std::vector<bool> mask;
	std::shared_ptr<ActorData> data;
	bool respawnable = false;
	bool active = true;
};


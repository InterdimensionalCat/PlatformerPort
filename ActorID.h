#pragma once
class ActorID
{
public:
	ActorID(const ActorID&) = default;
	ActorID& operator=(const ActorID&) = default;

	bool isIdValid() const;

	bool operator==(const ActorID & rhs) const;
	bool operator!=(const ActorID & rhs) const;
	bool operator<(const ActorID & rhs) const;
	bool operator>(const ActorID & rhs) const;
	bool operator<=(const ActorID & rhs) const;
	bool operator>=(const ActorID & rhs) const;
private:
	friend class ActorEntry;

	int compare(const ActorID& other) const;
	ActorID(const std::string& typeName, const std::string& variantName);
	ActorID();
	std::string typeName;
	std::string variantName;
};


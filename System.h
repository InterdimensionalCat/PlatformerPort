#pragma once
#include "Scene.h"
#include "Components.h"

#ifndef _System
#define _System(classname, systypename) public:\
uint32_t getID() const override {\
    uint32_t id = System::getIdStatic<classname>();\
    nameidmap[getName()] = id;\
    return id;\
}\
static uint32_t staticId() {\
    uint32_t id = System::getIdStatic<classname>();\
    nameidmap[getNameStatic()] = id;\
    return id;\
}\
SystemType getType() const override {\
   return systypename;\
}\
std::string getName() const override {\
    return #classname;\
}\
static std::string getNameStatic() {\
    return #classname;\
}\
	classname() {}\
protected:\
	std::shared_ptr<System> create(Scene* scene) const override {\
    return std::static_pointer_cast<System>(std::make_shared<classname>(scene));\
}\
    std::string getSysTypeName() const override {\
        return #systypename;\
    }\
private:\
	class StartupObj {\
	public:\
	StartupObj() {\
		System::systemPrototypes[classname::staticId()] = \
		std::static_pointer_cast<System>(std::make_shared<classname>());\
        std::cout << "system " << #classname << " added at value " << (int)classname::staticId() << "\n";\
	    }\
    };\
inline static StartupObj startup;
#endif

#ifndef _TYPE_NAME
#define _TYPE_NAME(type) #type
#endif


enum class SystemType {
	PreUpdate = 0,
	PreInput,
	Input,
	PostInput,
	PreAi,
	Ai,
	PostAi,
	PreLogic,
	Logic,
	PostLogic,
	PrePhysics,
	Physics,
	PostPhysics,
	PostUpdate,
	PreGraphics,
	Graphics,
	PostGraphics,
	Count
};

class System {
public:
	static std::shared_ptr<System> createSystem(Scene* scene, std::string name) {
		return systemPrototypes.at(nameidmap.at(name))->create(scene);
	}
	virtual void excecute() = 0;
	virtual SystemType getType() const = 0;
	virtual uint32_t getID() const = 0;
	virtual std::string getName() const = 0;

	virtual std::string toString() const = 0;

	static inline std::map<uint32_t, std::shared_ptr<System>> systemPrototypes;
	static inline std::map<std::string, uint32_t> nameidmap;

	template<typename T>
	static uint32_t getIdStatic() {
		static uint32_t staticid = staticidcounter++;
		return staticid;
	}
	System() {}
	virtual ~System() {}
protected:
	virtual std::string getSysTypeName() const = 0;
	virtual std::shared_ptr<System> create(Scene* scene) const = 0;
	System(Scene* scene) : scene(scene) {}
	Scene* scene;
private:
	static inline uint32_t staticidcounter = 0;
};

template<typename... ComponentTypes>
class SystemTrivial : public System {
public:
	SystemTrivial(Scene* scene) : System(scene), 
		view(std::make_shared<SceneView<ComponentTypes...>>(scene)) {}

	virtual void excecute() {
		for (auto actor : *view) {
			excecutionFunction(actor);
		}
	}
protected:
	SystemTrivial() {}
	virtual void excecutionFunction(std::shared_ptr<ActorEntry> entry) = 0;

	virtual std::string toString() const override {
		std::string types[] = { "", (typeid(ComponentTypes).name())... ,"" };
		std::string printval = getName()
			+ ":(SystemTrivial<";

		for (auto str : types) {
			std::string::size_type i = str.find("struct");
			if (i != std::string::npos) {
				str.erase(i, 6);
			}

			printval += str;
		}

		printval += " >)->Type="
			+ getSysTypeName();
		return printval;
	}

	std::shared_ptr<SceneView<ComponentTypes...>> view;
};


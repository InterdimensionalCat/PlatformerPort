#pragma once
#include "Texture.h"
#include "TileCollisionEvent.h"
#include "Animation.h"
#include "Scene.h"
#include "MapBoundCollisionEvent.h"
#include <typeinfo>
#include "StringableEnum.h"



class Input;
class Animation;

class RegisterComponentsClass {
public:
	inline static std::vector<std::function<void()>> registerComponents;
};

void registerComponentsFunction();

typedef std::function<void(Scene*, std::shared_ptr<ActorEntry>, std::shared_ptr<json>)> Func;

extern std::vector<Func> generateJsonFunctions;
extern std::vector<Func> generateFromJsonFunctions;

template<typename T>
void inline createJsonOf(Scene* scene, std::shared_ptr<ActorEntry> entry, std::shared_ptr<json> file) {
	auto comp = scene->getComponent<T>(entry);
	if (comp != nullptr) {
		comp->toJson(file);
	}
}

template<typename T>
void inline createComponentFromJson(Scene* scene, std::shared_ptr<ActorEntry> entry, std::shared_ptr<json> file) {
	if (file->find(T::getNameStatic()) != file->end()) {
		auto comp = scene->assignComponent<T>(entry);
		comp->fromJson(file);
	}
}

template<typename T>
void inline writeJsonArg(std::shared_ptr<json>& file, const std::string& name, const std::string& argname, const T& arg) {
	(*file)[name][argname] = arg;
}

template<typename T>
void inline readJsonArg(const std::shared_ptr<json>& file, const std::string& name, const std::string& argname, T& arg) {
	arg = (*file)[name][argname];
}

template<typename... Args>
void inline genericToJson(std::shared_ptr<json>& file, const std::string& name, const std::string argname, const Args&... args) {

	std::stringstream ss(argname);
	std::vector<std::string> result;

	while (ss.good())
	{
		std::string substr;
		getline(ss, substr, ',');

		substr.erase(std::remove_if(substr.begin(), substr.end(),
			[](char& c) {
				return std::isspace<char>(c, std::locale::classic());
			}),
			substr.end());

		result.push_back(substr);
	}

	int i = 0;
	int _[] = { 0, (writeJsonArg<Args>(file, name, result.at(i++), args), 0)... };
	(void)_;
}

template<typename... Args>
void inline genericFromJson(const std::shared_ptr<json>& file, const std::string& name, const std::string& argname, Args&... args) {

	std::stringstream ss(argname);
	std::vector<std::string> result;

	while (ss.good())
	{
		std::string substr;
		getline(ss, substr, ',');

		substr.erase(std::remove_if(substr.begin(), substr.end(),
			[](char& c) {
				return std::isspace<char>(c, std::locale::classic());
			}),
			substr.end());

		result.push_back(substr);
	}

	int i = 0;
	int _[] = { 0, (readJsonArg<Args>(file, name, result.at(i++), args), 0)...  };
	(void)_;
}

#ifndef _JsonTrivial
#define _JsonTrivial(...)  void toJson(std::shared_ptr<json> file) const {\
genericToJson(file, getName(),   #__VA_ARGS__,   __VA_ARGS__);\
}\
void fromJson(const std::shared_ptr<json>& file) {\
genericFromJson(file, getName(), #__VA_ARGS__, __VA_ARGS__);\
}
#endif

#ifndef _JsonEnum
#define _JsonEnum(enumtype, enumval) \
  void toJson(std::shared_ptr<json> file) const {\
    (*file)[getName()][#enumval] = enumtype##ToString(enumval);\
}\
void fromJson(const std::shared_ptr<json>& file) {\
    enumval = enumtype##FromString((*file)[getName()][#enumval]); \
}
#endif

//components must be structs and call this macro where x is the type name
#ifndef _Component
#define _Component(x) std::string getName() const {\
    return #x;\
}\
static std::string getNameStatic() {\
	return #x;\
}\
private:\
class StartupObj {\
public:\
	StartupObj() {\
       RegisterComponentsClass::registerComponents.push_back(x::addJsonGenerator);\
	}\
};\
friend class StartupObj;\
static void addJsonGenerator() {\
		Func function = createJsonOf<x>;\
		Func fromjson = createComponentFromJson<x>;\
		if (!added) {\
				generateJsonFunctions.push_back(function);\
				generateFromJsonFunctions.push_back(fromjson);\
				added = true;\
		}\
}\
inline static StartupObj startup;\
inline static bool added = false;
#endif

#ifndef _Component_Trivial
#define _Component_Trivial(x, ...) _JsonTrivial(__VA_ARGS__) _Component(x)
#endif

#ifndef _Component_Enum
#define _Component_Enum(x, enumtype, enumval) _JsonEnum(enumtype, enumval) _Component(x)
#endif

_CREATE_STRINGABLE_ENUM(ActionState, GroundStill, GroundMove, GroundTurn, Airborne);
_CREATE_STRINGABLE_ENUM(PlatformType, Vertical, Horizontal, Falling, Still);

enum class ConstrainedMovementEvent {
	OOB,
	OOBLeft,
	OOBRight,
	OOBTop,
	OOBBot
};

ActionState StateFromStr(const std::string& str);
std::string StateToStr(const ActionState& state);

struct PlayerFlag {
	bool pflag = true;

	_Component_Trivial(PlayerFlag, pflag)
};

struct EnemyFlag {
	std::string enemyname = "";

	_Component_Trivial(EnemyFlag, enemyname)
};

struct StateController {
	ActionState state;
	_Component_Enum(StateController, ActionState, state)
};

struct AnimStateMap {
	std::shared_ptr<std::map<ActionState, AnimationEntry>> animations;
	ActionState prevstate;

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["prevstate"] = StateToStr(prevstate);
		std::vector<json> arr;
		for (auto [state, val] : *animations) {
			std::shared_ptr<json> obj = std::make_shared<json>();
			val.toJson(obj);
			std::string jstr = "{ \"key\": \"" + StateToStr(state) + "\", \"value\": " + obj->dump() + "}";
			arr.push_back(json(
				json::parse(jstr)
			));
		}

		(*file)[getName()]["animations"] = arr;
	}

	void fromJson(const std::shared_ptr<json>& file) {
		prevstate = StateFromStr((*file)[getName()]["prevstate"]);
		animations = std::make_shared<std::map<ActionState, AnimationEntry>>();
		std::vector<json> entries = (*file)[getName()]["animations"];
		for (auto& entry : entries) {
			std::string key = entry["key"];
			AnimationEntry animentry;
			animentry.fromJson(entry["value"]);
			animations->emplace(StateFromStr(key), animentry);
		}
	}

	_Component(AnimStateMap)
};

struct InputController {
	std::shared_ptr<Input> input;

	//read/write a name that is a key into a map of input devices?


	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["input"] = input->getName();
	}

	void fromJson(const std::shared_ptr<json>& file) {
		input = Input::createInputDevice((*file)[getName()]["input"]);
	}

	_Component(InputController)
};

struct Animatable {
	std::shared_ptr<Animation> animation;

	void toJson(std::shared_ptr<json> file) const {
		std::shared_ptr<json> animEntryJson = std::make_shared<json>();
		animation->getAnimEntry().toJson(animEntryJson);
		(*file)[getName()]["animation"] = *animEntryJson;
	}

	void fromJson(const std::shared_ptr<json>& file) {
		AnimationEntry entry;
		std::shared_ptr<json> animEntryJson = std::make_shared<json>();
		*animEntryJson = (*file)[getName()]["animation"];
		entry.fromJson(animEntryJson);
		animation = std::make_shared<Animation>(entry);
	}


	//_Component(Animatable)

	std::string getName() const {
		return ((std::string)typeid(Animatable).name()).erase(0, 7);
	}
	static std::string getNameStatic() {
		return ((std::string)typeid(Animatable).name()).erase(0, 7);
	}
private:
	class StartupObj {

	public:
		StartupObj() {
			RegisterComponentsClass::registerComponents.push_back(Animatable::addJsonGenerator);
		}
	};
	friend class StartupObj;
	static void addJsonGenerator() {

		Func function = createJsonOf<Animatable>;
		Func fromjson = createComponentFromJson<Animatable>;
		if (!added) {

			generateJsonFunctions.push_back(function);
			generateFromJsonFunctions.push_back(fromjson);
			added = true;
		}
	}
	inline static StartupObj startup;
	inline static bool added = false;
};

struct Transform {
	float x;
	float y;

	_Component_Trivial(Transform, x, y)
};

struct Velocity {
	float x;
	float y;

	_Component_Trivial(Velocity, x, y)
};

struct BoundedVelocity {
	float maxVelX;
	float maxVelY;

	_Component_Trivial(BoundedVelocity, maxVelX, maxVelY)
};

struct PhysicsProperties {
	float friction;
	float gravity;

	_Component_Trivial(PhysicsProperties, friction, gravity)
};

struct Jumpable {
	float jumpForce;
	float jumpReleaseMod;
	float jumpFloat;
	bool  jumpedflag = false;

	_Component_Trivial(Jumpable, jumpForce, jumpReleaseMod, jumpFloat)
};

struct Turnable {
	float turnThreshold;

	_Component_Trivial(Turnable, turnThreshold)
};

struct AirborneState {
	int airborneTimer;
	int maxAirborneTimer;

	_Component_Trivial(AirborneState, airborneTimer, maxAirborneTimer)
};

struct HorzMovable {
	float accelX;

	_Component_Trivial(HorzMovable, accelX)
};

struct CircularMovable {
	float accel;
	float angleX;
	float angleY;

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["accel"] = accel;
		(*file)[getName()]["angleX"] = angleX;
		(*file)[getName()]["angleY"] = angleY;
	}

	void fromJson(const std::shared_ptr<json>& file) {
		accel = (*file)[getName()]["accel"];
		angleX = (*file)[getName()]["angleX"];
		angleY = (*file)[getName()]["angleY"];
	}

	//_Component_Trivial(CircularMovable, accel, angleX, angleY);
	_Component(CircularMovable);
};

struct StateAccelerationValues {
	float groundAccel;
	float airAccel;
	float stillAccel;
	float turnAccel;

	_Component_Trivial(StateAccelerationValues, groundAccel, airAccel, stillAccel, turnAccel)
};

struct StateFrictionValues {
	float groundFrc;
	float airFrc;
	float stillFrc;
	float turnFrc;

	_Component_Trivial(StateFrictionValues, groundFrc, airFrc, stillFrc, turnFrc)
};

struct ConstrainedMovement {
	float maxDisplacementLeft  = -1.0f;
	float maxDisplacementRight = -1.0f;
	float maxDisplacementUp    = -1.0f;
	float maxDisplacementDown  = -1.0f;

	float originX;
	float originY;

	std::shared_ptr<std::queue<ConstrainedMovementEvent>> events 
		= std::make_shared<std::queue<ConstrainedMovementEvent>>();

	void toJson(std::shared_ptr<json> file) const {

		(*file)[getName()]["maxDisplacementLeft" ] = maxDisplacementLeft;
		(*file)[getName()]["maxDisplacementRight"] = maxDisplacementRight;
		(*file)[getName()]["maxDisplacementUp"   ] = maxDisplacementUp;
		(*file)[getName()]["maxDisplacementDown" ] = maxDisplacementDown;

		(*file)[getName()]["originX"] = originX;
		(*file)[getName()]["originY"] = originY;

		(*file)[getName()]["events"] = nullptr;
	}

	void fromJson(const std::shared_ptr<json>& file) {
		maxDisplacementLeft  = (*file)[getName()]["maxDisplacementLeft" ];
		maxDisplacementRight = (*file)[getName()]["maxDisplacementRight"];
		maxDisplacementUp    = (*file)[getName()]["maxDisplacementUp"   ];
		maxDisplacementDown  = (*file)[getName()]["maxDisplacementDown" ];

		originX = (*file)[getName()]["originX"];
		originY = (*file)[getName()]["originY"];
	}


	_Component(ConstrainedMovement)
};

struct ConstrainedMovementEventListener {
	std::shared_ptr<std::queue<ConstrainedMovementEvent>> events
		= std::make_shared<std::queue<ConstrainedMovementEvent>>();

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["events"] = nullptr;
	}

	void fromJson(const std::shared_ptr<json>& file) {

	}


	_Component(ConstrainedMovementEventListener)
};

struct CircularConstrainedMovement {
	float radius;
	float originX;
	float originY;

	_Component_Trivial(CircularConstrainedMovement, radius, originX, originY);
};

struct JumpOn {
	float jumpTolerencePercent;
	float jumpVelMod;
	float jumpMin;

	_Component_Trivial(JumpOn, jumpTolerencePercent, jumpVelMod, jumpMin)
};

struct SpriteDrawable {

	SpriteDrawable() = default;
	SpriteDrawable(std::shared_ptr<sf::Sprite> spr, std::shared_ptr<Texture> tex) : spr(spr), tex(tex) {
		spr->setTexture(tex->getTexture());
	}

	std::shared_ptr<sf::Sprite> spr;
	std::shared_ptr<Texture> tex;

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["spr"] = nullptr;
		(*file)[getName()]["tex"] = tex->getName();
	}

	void fromJson(const std::shared_ptr<json>& file) {
		spr = std::make_shared<sf::Sprite>();
		tex = std::make_shared<Texture>((*file)[getName()]["tex"]);
		spr->setTexture(tex->getTexture());
	}

	_Component(SpriteDrawable)
};

struct Hitbox {
	sf::FloatRect rect;

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["rect"]["left"  ] = rect.left;
		(*file)[getName()]["rect"]["top"   ] = rect.top;
		(*file)[getName()]["rect"]["width" ] = rect.width;
		(*file)[getName()]["rect"]["height"] = rect.height;
	}

	void fromJson(const std::shared_ptr<json>& file) {
		rect.left   = (*file)[getName()]["rect"]["left"];
		rect.top    = (*file)[getName()]["rect"]["top"];
		rect.width  = (*file)[getName()]["rect"]["width"];
		rect.height = (*file)[getName()]["rect"]["height"];
	}

	_Component(Hitbox)

	//_Component_Trivial(Hitbox, rect)
};

struct HitboxExtension {
	sf::FloatRect rect;

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["rect"]["left"] = rect.left;
		(*file)[getName()]["rect"]["top"] = rect.top;
		(*file)[getName()]["rect"]["width"] = rect.width;
		(*file)[getName()]["rect"]["height"] = rect.height;
	}

	void fromJson(const std::shared_ptr<json>& file) {
		rect.left = (*file)[getName()]["rect"]["left"];
		rect.top = (*file)[getName()]["rect"]["top"];
		rect.width = (*file)[getName()]["rect"]["width"];
		rect.height = (*file)[getName()]["rect"]["height"];
	}

	_Component(HitboxExtension)
};

struct TileCollidable {
	bool tileCollidable = true;

	_Component_Trivial(TileCollidable, tileCollidable)
};

struct PlatformTolerence {
	float up;
	float down;

	_Component_Trivial(PlatformTolerence, up, down);
};

struct TileCollisionEventListener {
	std::shared_ptr<std::deque<TileCollisionEvent>> events = std::make_shared<std::deque<TileCollisionEvent>>();
	~TileCollisionEventListener() {
		events->clear();
	}

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["events"] = nullptr;
	}

	void fromJson(const std::shared_ptr<json>& file) {

	}

	_Component(TileCollisionEventListener)
};

struct BoundByMapBounds {
	bool boundUp   = true;
	bool boundDown = true;
	bool boundLeft = true;
	bool boundRight = true;

	_Component_Trivial(BoundByMapBounds, boundUp, boundDown, boundLeft, boundRight)
};

struct MapBoundCollisionListener {

	std::shared_ptr<std::deque<MapBoundCollisionEvent>> events = std::make_shared<std::deque<MapBoundCollisionEvent>>();
	
	~MapBoundCollisionListener() {
		events->clear();
	}

	void toJson(std::shared_ptr<json> file) const {
		(*file)[getName()]["events"] = nullptr;
	}

	void fromJson(const std::shared_ptr<json>& file) {

	}

	_Component(MapBoundCollisionListener)
};

struct ActorCollidable {
	std::string collisiontype;

	_Component_Trivial(ActorCollidable, collisiontype);
};

struct PlatformTypeable {
	PlatformType type;

	_Component_Enum(PlatformTypeable, PlatformType, type);
};

struct FallingPlatform {
	bool falling = false;
	float grv = toMeters(0.5f);
	_Component_Trivial(FallingPlatform, falling, grv);
};


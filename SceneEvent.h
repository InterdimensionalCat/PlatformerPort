#pragma once

class Scene;

class SceneEvent
{
public:
	virtual void excecute(Scene* scene) = 0;
};

#ifndef _Scene_Event
#define _Scene_Event(x) class x : public SceneEvent {\
public:\
	void excecute(Scene* scene) override;\
};
#endif

_Scene_Event(ResetLevelEvent)


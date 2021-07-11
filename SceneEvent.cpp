#include "stdafx.h"
#include "SceneEvent.h"
#include "Scene.h"


void ResetLevelEvent::excecute(Scene* scene) {
	scene->loadLevel(scene->currentlevelname);
}
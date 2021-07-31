#include "stdafx.h"
#include "SceneEvent.h"
#include "Scene.h"


void ResetLevelEvent::excecute(Scene* scene) {
	scene->loadLevel(scene->currentlevelname);
}

void DespawnActorEvent::excecute(Scene* scene) {
	scene->despawnActor(actortodespawn);
}

void ChangeLevelEvent::excecute(Scene* scene) {
	scene->changeLevel();
}
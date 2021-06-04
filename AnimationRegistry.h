#pragma once

class Animation;

void RegisterAnimations();

//CREATES A COPY OF THE ANIMATION
Animation getAnimFromName(const std::string& name);
void clearAnimRegistry();

#pragma once

//standard includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <exception>
#include <memory>
#include <string>
#include <filesystem>
#include <variant>
#include <map>
#include <vector>
#include <array>
#include <functional>
#include <queue>
#include <optional>
#include "json.hpp"
#include "tileson.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

//windows specifc macros/setup
#ifdef _WIN32

///////////////MEMORY LEAK CHECKING/////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/*
 on windows debug mode, new keyword is replaced with a debug-new that
 provides more info about the object that was created(file, line#, etc)
 to better diagnose memory leaks. Since I mostly use smart pointers nowadays
 not super helpful anymore
*/
//#ifdef _DEBUG
//#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#else
//#define new new
//#endif

///////////////////////////////////////////////////////////

#endif

//dependancy includes
#include <SFML\Graphics.hpp>


//project includes
#include "Logger.h"
#include "Settings.h"


float toPixels(const float meters);
float toMeters(const float pixels);
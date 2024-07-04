#pragma once

// **************************** Library ****************************
#ifdef _DEBUG
#pragma comment(lib, "../_Libraries/ThirdParty/Assimp/lib/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "../_Libraries/ThirdParty/Assimp/lib/assimp-vc143-mt.lib")
#endif
#pragma comment(lib, "../_Libraries/ImGui.lib")
#pragma comment(lib, "../_Libraries/ToyRendererEngine.lib")

// **************************** Headers ****************************
#include <Windows.h>
#include "Platform/BaseWindow.h"

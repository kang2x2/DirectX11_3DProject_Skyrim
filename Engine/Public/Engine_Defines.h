#pragma once

#pragma warning (disable : 4251)
#pragma warning (disable : 26812)
#include <d3d11.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <random>

#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#include <Effect11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/ScreenGrab.h>

#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/Effects.h>

#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>

#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

using namespace DirectX;

#include <iostream>
#include <vector> /* 배열 */
#include <list> /* 더블링크드리스트 */
#include <map> /* 이진트리 */
#include <unordered_map> /* 해시테이블 */
#include <algorithm>

// imgui 문자열 관련
#include <string>
#include <tchar.h>
#include <atlstr.h>
#include <locale>
#include <codecvt>

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"

#include "fmod.h"
#include "fmod.hpp"
#include <io.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

using namespace Engine;
using namespace std;

enum CHANNELID {
	CHANNEL_BGM, 
	CHANNEL_PLAYER, CHANNEL_PLAYER_RUN,
	CHANNEL_MONSTER1, CHANNEL_MONSTER1_RUN, CHANNEL_MONSTER1_ATK,
	CHANNEL_MONSTER2, CHANNEL_MONSTER2_RUN, CHANNEL_MONSTER2_ATK,
	CHANNEL_MONSTER3, CHANNEL_MONSTER3_RUN, CHANNEL_MONSTER3_ATK,
	CHANNEL_MONSTER4, CHANNEL_MONSTER4_RUN, CHANNEL_MONSTER4_ATK,
	CHANNEL_NATURAL,
	CHANNEL_ATK, CHANNEL_HIT, CHANNEL_GUARD,
	CHANNEL_END
};
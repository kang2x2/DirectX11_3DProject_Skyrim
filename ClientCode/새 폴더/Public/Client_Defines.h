#pragma once

#include <process.h>

namespace Client
{
	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_ZERO, 
		LEVEL_GAMEPLAY,
		LEVEL_LOADING, LEVEL_TOOL, LEVEL_END };

	enum STAGEID { STAGE_WHITERUN, STAGE_DUNGEON, STAGE_CASTLE, STAGE_END };

	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;

}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern Client::LEVELID g_curLevel;
extern Client::STAGEID g_curStage;

extern bool g_bIsWhiteRunInit;
extern bool g_bIsDungeonInit; 
extern bool g_bIsCastleInit;
extern bool g_bIsStaticInit;
extern bool g_bIsPublicInit;

extern bool g_bIsPause;
extern bool g_bIsTalk;
extern bool g_bIsSlow;
extern float g_fGameSpeed;
extern int g_iCurTalkID;

using namespace Client;

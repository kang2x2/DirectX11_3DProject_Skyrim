#include "../Default/framework.h"
#include "MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "SkyrimUI_SceneChange.h"
#include "Inventory.h"

#include "Loading_Camera.h"
#include "LoadingObj_Falmer.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	ShowCursor(false);

	g_bIsWhiteRunInit = false;
	g_bIsDungeonInit = false;
	g_bIsCastleInit = false;
	g_bIsPublicInit = false;

	g_bIsTalk = false;

	g_bIsSlow = false;
	g_fGameSpeed = 1.f;
	g_iCurTalkID = 0;

	/* 게임 초기화 */
	// 1. 장치
	GRAPHIC_DESC GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	// GameInstance 초기화
	if(FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, g_hInst, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	// 컴포넌트 초기화
	if (FAILED(Ready_ProtoType_Components()))
		return E_FAIL;

	// 폰트 초기화
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_Bold"), TEXT("../Bin/Resource/Fonts/Bold.spriteFont"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext, TEXT("Font_ExtraBold"), TEXT("../Bin/Resource/Fonts/ExtraBold.spriteFont"))))
		return E_FAIL;

	// 레벨 생성
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float _fTimeDelta)
{
	m_pGameInstance->Tick(_fTimeDelta);
	m_fTimeAcc += _fTimeDelta;
}

HRESULT CMainApp::Render()
{
	if (g_curLevel == LEVEL_TOOL)
		m_pGameInstance->Clear_BackBuffer_View(_float4(0.02f, 0.02f, 0.02f, 1.f));

	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw_RenderObjects();

	m_pGameInstance->AfterRender();

//#ifdef _DEBUG
	++m_iRenderCount;

	if (1.f <= m_fTimeAcc)
	{
		m_fTimeAcc = 0;
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iRenderCount);

		m_iRenderCount = 0;
	}
	m_pGameInstance->Render_Font(TEXT("Font_ExtraBold"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 0.f, 0.f, 1.f));

//#endif

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID _eLevelID)
{
	if (m_pGameInstance == nullptr)
		return E_FAIL;

	/* 로고 씬을 위한 로딩 작업 수행. */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, _eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_ProtoType_Components()
{
#pragma region Texture
	// Texture
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/Cursor.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/black.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/gray.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WhiteEm"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/white_em.dds"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/Dissolve.png"), 1))))
		return E_FAIL;
	/* Bar */
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EmptyBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/EmptyBar.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/SpBar.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/UI/HpBar.png"), 1))))
		return E_FAIL;
	/* Fade Black */
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FadeBlack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/FadeBlack.dds"), 1))))
		return E_FAIL;
	/* Title */
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Title"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resource/Textures/Skyrim/SkyrimTitle.png"), 1))))
		return E_FAIL;
#pragma endregion

#pragma region Shader
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("ProtoType_Component_Shader_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_Effect.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
#pragma endregion

	// Renderer
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// Transform
	if (FAILED(m_pGameInstance->Add_ProtoType_Component(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	__super::Free();

	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}

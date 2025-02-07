#include "framework.h"
#include "Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "Logo_Camera.h"

#include "SkyrimUI_SceneChange.h"
#include "Skyrim_Cursor.h"

#include "Effect_FadeBlack.h"

CLevel_Logo::CLevel_Logo(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel(_pDevice, _pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlayBGM(TEXT("DragonbornTheme.mp3"), 0.6f);

	Safe_Release(pGameInstance);

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Cursor(TEXT("Layer_Cursor"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_LogoObj(TEXT("Layer_LogoObj"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_LogoText(TEXT("Layer_LogoText"))))
		return E_FAIL;

	CGameInstance::GetInstance()->InitOcTree(m_pDevice, m_pContext, { 0.f, 0.f, 2.f }, { 1.f, 1.f, 1.f });

	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear_BackBuffer_View(_float4(0.02f, 0.02f, 0.02f, 1.f));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::LateTick(_float _fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("Current Level : Logo."));

	if (m_bIsChangeScene)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
		
		g_curStage = (STAGEID)m_iChangeStageIdx;
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, (LEVELID)m_iChangeLevelIdx))))
			return E_FAIL;
		
		m_bIsChangeScene = false;
		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Cursor(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_Cursor"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLogo_Camera::LOGO_CAMERA_DESC LogoCameraDesc;
	ZeroMemory(&LogoCameraDesc, sizeof LogoCameraDesc);

	LogoCameraDesc.vEye = _float4(0.f, 0.f, -1.f, 1.f);
	LogoCameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	LogoCameraDesc.fFovY = XMConvertToRadians(60.f);
	LogoCameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	LogoCameraDesc.fNear = 0.2f;
	LogoCameraDesc.fFar = 300.f;
	LogoCameraDesc.fSpeedPerSec = 15.f;
	LogoCameraDesc.fRotationRadianPerSec = XMConvertToRadians(90.f);
	LogoCameraDesc.fZoomPerSec = 500.f;

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_Camera_Logo"), &LogoCameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	pGameInstance->Light_Clear();
	
	LIGHT_DESC LightDesc;
	
	/* 점 광원 추가 */
	ZeroMemory(&LightDesc, sizeof(LightDesc));
	LightDesc.eLightType = LIGHT_DESC::LIGHT_POINT;
	LightDesc.vLightPos = { -0.3f, -0.1f, -1.5f, 1.f };
	LightDesc.fLightRange = 100.f;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.773f, 0.973f, 0.965f, 1.f);
	
	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const wstring& _strLayerTag)
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_BackGround"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_LogoObj(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_Logo"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_LogoText(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSkyrimUI_SceneChange::UI_TEXT_DESC textDesc;
	textDesc.strText = TEXT("New");
	textDesc.strFontTag = TEXT("Font_Bold");
	textDesc.fX = 1100.f;
	textDesc.fY = 500.f;
	textDesc.fSizeX = 50.f;
	textDesc.fSizeY = 25.f;
	textDesc.vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	textDesc.fFontSize = 0.5f;
	textDesc.iLevelID = LEVEL_GAMEPLAY;
	textDesc.iStageID = STAGE_WHITERUN;

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_UI_Text"), &textDesc)))
		return E_FAIL;

	textDesc.strText = TEXT("Edit");
	textDesc.strFontTag = TEXT("Font_Bold");
	textDesc.fX = 1100.f;
	textDesc.fY = 550.f;
	textDesc.fSizeX = 50.f;
	textDesc.fSizeY = 25.f;
	textDesc.vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	textDesc.fFontSize = 0.5f;
	textDesc.iLevelID = LEVEL_TOOL;	
	textDesc.iStageID = STAGE_END;

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_UI_Text"), &textDesc)))
		return E_FAIL;

	textDesc.strText = TEXT("Data");
	textDesc.strFontTag = TEXT("Font_Bold");
	textDesc.fX = 1100.f;
	textDesc.fY = 600.f;
	textDesc.fSizeX = 50.f;
	textDesc.fSizeY = 25.f;
	textDesc.vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	textDesc.fFontSize = 0.5f;
	textDesc.iLevelID = LEVEL_ZERO;
	textDesc.iStageID = STAGE_END;

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOGO, _strLayerTag, TEXT("ProtoType_GameObject_UI_Text"), &textDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo* CLevel_Logo::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Logo* pInstance = new CLevel_Logo(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();
}

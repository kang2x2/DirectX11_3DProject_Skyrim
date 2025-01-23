#include "framework.h"
#include "Level_Dungeon.h"

#include "Level_Loading.h"

#include <filesystem>
#include <fstream>
#include <commdlg.h>

#include "IMGui_Manager.h"
#include "GameInstance.h"

#include "Player.h"

#include "Effect_FadeBlack.h"
#include "SceneChangeCol.h"

CLevel_Dungeon::CLevel_Dungeon(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel(_pDevice, _pContext)
{
}

HRESULT CLevel_Dungeon::Initialize()
{
	// Test HPBar
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_UI_MonsterHpBar"))))
		return E_FAIL;
	Safe_Release(pGameInstance);

	if (FAILED(Ready_Cursor(TEXT("Layer_Cursor"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Equip(TEXT("Layer_Equip"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Sky(TEXT("Layer_Sky"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
		return E_FAIL;

	if (FAILED(Ready_Level()))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Navigation_Dungeon(TEXT("Layer_Navigation_Dungeon"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Particle(TEXT("Layer_Particle"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Dungeon::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear_BackBuffer_View(_float4(0.02f, 0.02f, 0.02f, 1.f));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::LateTick(_float _fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("Current Level : Dungeon"));

	if (GetKeyState(VK_F11) & 0x8000)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
	
		g_curStage = STAGE_WHITERUN;
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return E_FAIL;
	
		Safe_Release(pGameInstance);
	}
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

void CLevel_Dungeon::AfterRender()
{
}

HRESULT CLevel_Dungeon::Ready_Cursor(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Cursor"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Level()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	pGameInstance->PlayBGM(TEXT("ambt_dungeon_cave_generic_2dlp.wav"), 0.7f);
	
	CSceneChangeCol::tagSceneChangeColDesc tempDesc;
	tempDesc.eDestStage = STAGE_WHITERUN;
	tempDesc.vPos = { 199.f, -12.f, 84.f, 1.f };
	tempDesc.strDestText = TEXT("화이트런");
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_SceneChangeCol"), TEXT("ProtoType_GameObject_SceneChangeCol"), &tempDesc)))
		return E_FAIL;


#pragma region Static
	wstring filePath = TEXT("../Bin/SaveLoad/Dungeon2");

	// 파일을 열기 모드로 열기.
	ifstream fileStream(filePath, ios::binary);
	if (fileStream.is_open()) {
		pGameInstance->Object_FileLoad(fileStream, LEVEL_GAMEPLAY);

		fileStream.close();
		//MessageBox(g_hWnd, L"파일을 성공적으로 불러왔습니다.", L"불러오기 완료", MB_OK);
	}
	else {
		MessageBox(g_hWnd, L"파일을 불러오는 중 오류가 발생했습니다.", L"불러오기 오류", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
#pragma endregion

#pragma region Dynamic

	filePath = TEXT("../Bin/SaveLoad/Dungeon_Monster");
	
	// 파일을 열기 모드로 열기.
	ifstream fileStream2(filePath, ios::binary);
	if (fileStream2.is_open()) {
		pGameInstance->Object_FileLoad(fileStream2, LEVEL_GAMEPLAY);
		
		fileStream2.close();
		//MessageBox(g_hWnd, L"파일을 성공적으로 불러왔습니다.", L"불러오기 완료", MB_OK);
	}
	else {
		MessageBox(g_hWnd, L"파일을 불러오는 중 오류가 발생했습니다.", L"불러오기 오류", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
#pragma endregion

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Light_Clear();

	LIGHT_DESC LightDesc;

	/* 방향성 광원을 추가. */
	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eLightType = LIGHT_DESC::LIGHT_DIRECTIONAL;
	LightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);

	LightDesc.vDiffuse = _float4(0.05f, 0.05f, 0.05f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);
	
	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

#pragma region Light

	wstring filePath = TEXT("../Bin/SaveLoad/Light_Dungeon");
	// 파일을 열기 모드로 열기.
	ifstream fileStream(filePath, ios::binary);
	if (fileStream.is_open()) {
		pGameInstance->Light_FileLoad(fileStream, LEVEL_GAMEPLAY);

		fileStream.close();
		//MessageBox(g_hWnd, L"파일을 성공적으로 불러왔습니다.", L"불러오기 완료", MB_OK);
	}
	else {
		MessageBox(g_hWnd, L"파일을 불러오는 중 오류가 발생했습니다.", L"불러오기 오류", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

#pragma endregion

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Layer_Equip(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Weapon_FalmerAxe"))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Layer_Player(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Player"))))
		return E_FAIL;

	dynamic_cast<CTransform*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"),
		TEXT("Player"))->Get_Component(TEXT("Com_Transform")))
		->Set_State(CTransform::STATE_POSITION, XMVectorSet(51.f, 0.f, 3.f, 1.f));

	//dynamic_cast<CTransform*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"),
	//	TEXT("Player"))->Get_Component(TEXT("Com_Transform")))
	//	->Set_State(CTransform::STATE_POSITION, XMVectorSet(149.f, -12.f, 85.f, 1.f));

	dynamic_cast<CPlayer*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"),
		TEXT("Player")))->Set_CurCell();

	Safe_Release(pGameInstance);

	return S_OK;

}

HRESULT CLevel_Dungeon::Ready_Layer_Inventory(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* Inventory */
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_Inventory"), TEXT("ProtoType_GameObject_UI_Inventory"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Layer_Navigation_Dungeon(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Navigation"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Layer_Sky(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstace);

	if (FAILED(pGameInstace->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Sky"))))
		return E_FAIL;

	Safe_Release(pGameInstace);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Layer_Particle(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Particle_Rect"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Dungeon::Ready_Layer_Effect(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CEffect_FadeBlack::FADE_DESC FadeDesc;
	FadeDesc.bIsFadeIn = true;
	FadeDesc.fFadeTime = 0.25f;
	FadeDesc.eDestStage = STAGE_DUNGEON;
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_FadeBlack"), &FadeDesc)))
	{
		MSG_BOX("Fade 생성 실패.");
		return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_BloodFlare"))))
	{
		MSG_BOX("Blood 생성 실패.");
		return E_FAIL;
	}
	//if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_BloodSpot"))))
	//	return E_FAIL;
	pGameInstance->PlaySoundFile(TEXT("drs_autoload_cave_open_01.wav"), CHANNEL_NATURAL, 1.f);


	// UI
	//if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_UI_MonsterHpBar"))))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Dungeon* CLevel_Dungeon::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_Dungeon* pInstance = new CLevel_Dungeon(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CLevel_Dungeon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Dungeon::Free()
{
	__super::Free();
}

#include "framework.h"
#include "Level_WhiteRun.h"

#include "Level_Loading.h"

#include <filesystem>
#include <fstream>
#include <commdlg.h>

#include "IMGui_Manager.h"
#include "GameInstance.h"

#include "SceneChangeCol.h"
#include "Player.h"
#include "Inventory.h"

#include "Effect_FadeBlack.h"

_uint CLevel_WhiteRun::m_iVisitCount = 0;

CLevel_WhiteRun::CLevel_WhiteRun(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel(_pDevice, _pContext)
{
}

HRESULT CLevel_WhiteRun::Initialize()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->PlaySoundFile(TEXT("Whiterun.mp3"), CHANNEL_BGM, 0.35f);

	Safe_Release(pGameInstance);

	m_iVisitCount += 1;

	// CIMGui_Manager 초기화
	CIMGui_Manager::GetInstance()->Initialize(m_pDevice, m_pContext, LEVEL_GAMEPLAY);

	if (FAILED(Ready_Cursor(TEXT("Layer_Cursor"))))
		return E_FAIL;

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Equip(TEXT("Layer_Equip"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Inventory(TEXT("Layer_Inventory"))))
		return E_FAIL;

	if (FAILED(Ready_Level()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Navigation_WhiteRun(TEXT("Layer_Navigation_WhiteRun"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Particle(TEXT("Layer_Particle"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_WhiteRun::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear_BackBuffer_View(_float4(0.02f, 0.02f, 0.02f, 1.f));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WhiteRun::LateTick(_float _fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("Current Level : WhiteRun"));

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

void CLevel_WhiteRun::AfterRender()
{
	// ImGui 그리기
	// CIMGui_Manager::GetInstance()->Frame();
}

HRESULT CLevel_WhiteRun::Ready_Cursor(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Cursor"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Level()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSceneChangeCol::tagSceneChangeColDesc tempDesc;
	tempDesc.eDestStage = STAGE_DUNGEON;
	tempDesc.vPos = { 3.f, 7.f, 64.f, 1.f };
	tempDesc.strDestText = TEXT("지하수로");
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_SceneChangeCol"), TEXT("ProtoType_GameObject_SceneChangeCol"), &tempDesc)))
		return E_FAIL;

#pragma region Static
	/* 화이트런 */
	wstring filePath = TEXT("../Bin/SaveLoad/Skyrim3");
	//wstring filePath = TEXT("../Bin/SaveLoad/testMap");

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

	filePath = TEXT("../Bin/SaveLoad/Outfit_NPC2");
	//filePath = TEXT("../Bin/SaveLoad/testMonster2");
	//
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

HRESULT CLevel_WhiteRun::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Light_Clear();

	LIGHT_DESC LightDesc;

	/* 방향성 광원을 추가. */
	ZeroMemory(&LightDesc, sizeof LightDesc);
	LightDesc.eLightType = LIGHT_DESC::LIGHT_DIRECTIONAL;
	LightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	
	LightDesc.vDiffuse = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vAmbient = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

#pragma region Light
	wstring filePath = TEXT("../Bin/SaveLoad/Light_WhiteRun");
	// 파일을 열기 모드로 열기.
	ifstream fileStream(filePath, ios::binary);
	if (fileStream.is_open()) {
		// 파일 내용을 읽기.
	
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);
	
		pGameInstance->Light_FileLoad(fileStream, LEVEL_GAMEPLAY);
	
		Safe_Release(pGameInstance);
	
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

HRESULT CLevel_WhiteRun::Ready_Layer_Equip(const wstring& _strLayerTag)
{
	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Player(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Player"))))
		return E_FAIL;

	/* 화이트런 */
	if (m_iVisitCount == 1)
	{
		dynamic_cast<CTransform*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"),
			TEXT("Player"))->Get_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, -1.3f, 12.f, 1.f));
	}
	else if (m_iVisitCount == 2)
	{
		dynamic_cast<CTransform*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"),
			TEXT("Player"))->Get_Component(TEXT("Com_Transform")))
			->Set_State(CTransform::STATE_POSITION, XMVectorSet(40.f, -1.3f, 10.f, 1.f));
	}

	dynamic_cast<CPlayer*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"),
		TEXT("Player")))->Set_CurCell();

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Inventory(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* Inventory */
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Player_Inventory"), TEXT("ProtoType_GameObject_UI_Inventory"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Navigation_WhiteRun(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstace);

	if (FAILED(pGameInstace->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Navigation"))))
		return E_FAIL;

	Safe_Release(pGameInstace);


	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Sky(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstace = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstace);
	
	if (FAILED(pGameInstace->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Sky"))))
		return E_FAIL;
	
	Safe_Release(pGameInstace);

	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Terrain(const wstring& _strLayerTag)
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Terrain"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Particle(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_Particle_Rect"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_WhiteRun::Ready_Layer_Effect(const wstring& _strLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CEffect_FadeBlack::FADE_DESC FadeDesc;
	FadeDesc.bIsFadeIn = true;
	FadeDesc.fFadeTime = 0.25f;
	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Effect"), TEXT("ProtoType_GameObject_FadeBlack"), &FadeDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, _strLayerTag, TEXT("ProtoType_GameObject_BloodFlare"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_WhiteRun* CLevel_WhiteRun::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLevel_WhiteRun* pInstance = new CLevel_WhiteRun(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CLevel_WhiteRun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_WhiteRun::Free()
{
	__super::Free();

	CIMGui_Manager::GetInstance()->DestroyInstance();
}

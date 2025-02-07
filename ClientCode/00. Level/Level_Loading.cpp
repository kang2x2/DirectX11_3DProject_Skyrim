#include "framework.h"
#include "Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"

#include "Level_Zero.h"
#include "CLevel_Tool.h"
#include "Level_Logo.h"
#include "Level_WhiteRun.h"
#include "Level_Dungeon.h"

#include "Loading_Camera.h"
#include "LoadingObj_Falmer.h"
#include "LoadingObj_Skeever.h"

#include "SkyrimUI_SceneChange.h"
#include "Inventory.h"


CLevel_Loading::CLevel_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CLevel(_pDevice, _pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVELID _eNextLevel)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->StopSoundAll();

	Safe_Release(pGameInstance);

	m_eNextLevel = _eNextLevel;

	if (!g_bIsStaticInit)
	{
		if (FAILED(Ready_StaticInit()))
			return E_FAIL;

		g_bIsStaticInit = true;
	}

	if (FAILED(Ready_Light(TEXT("Layer_Light"))))
		return E_FAIL;

	if (FAILED(Ready_LoadingClone(TEXT("Layer_Loading"))))
		return E_FAIL;

	// 다음 레벨에 대한 로딩을 수행
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel);

	if (m_pLoader == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear_BackBuffer_View(_float4(0.02f, 0.02f, 0.02f, 1.f));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::LateTick(_float _fTimeDelta)
{
	wstring		strLoadingText = m_pLoader->Get_LoadingText();

	SetWindowText(g_hWnd, strLoadingText.c_str());

	if (m_pLoader->Get_Finished() == true)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CLevel* pNewLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_ZERO:
			pNewLevel = CLevel_Zero::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_TOOL:
			pNewLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY:
			if (g_curStage == STAGE_WHITERUN)
				pNewLevel = CLevel_WhiteRun::Create(m_pDevice, m_pContext);
			else if (g_curStage == STAGE_DUNGEON)
				pNewLevel = CLevel_Dungeon::Create(m_pDevice, m_pContext);
			break;
		}

		if (pNewLevel == nullptr)
			return E_FAIL;

		if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
			return E_FAIL;

		Safe_Release(pGameInstance);

	}

	return S_OK;
}

HRESULT CLevel_Loading::Ready_StaticInit()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

#pragma region Component

	/* Shader_VtxNonAnimMesh */
	if (FAILED(pGameInstance->Add_ProtoType_Component(LEVEL_LOADING, TEXT("ProtoType_Component_Shader_VtxNonAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_VtxNonAnimMesh.hlsl"), VTX_NONANIMMESH::Elements, VTX_NONANIMMESH::iNumElements))))
		return E_FAIL;

	// Shader_VtxPosTex
	if (FAILED(pGameInstance->Add_ProtoType_Component(LEVEL_LOADING, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	// ViBuffer_Rect
	if (FAILED(pGameInstance->Add_ProtoType_Component(LEVEL_LOADING, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// Loading Obj
	_matrix matInitialize = XMMatrixIdentity();
	matInitialize = XMMatrixScaling(0.0012f, 0.0012f, 0.0012f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(pGameInstance->Add_ProtoType_Component(LEVEL_LOADING, TEXT("ProtoType_Component_Model_LoadingObj_Falmer"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/BinaryFBX/NonAnim/Skyrim_Loading/Loading_Falmer/Loading_Falmer.bin", matInitialize, CModel::TYPE_NONANIM))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_ProtoType_Component(LEVEL_LOADING, TEXT("ProtoType_Component_Model_LoadingObj_Skeever"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resource/BinaryFBX/NonAnim/Skyrim_Loading/Loading_Skeever/Loading_Skeever.bin", matInitialize, CModel::TYPE_NONANIM))))
		return E_FAIL;

#pragma endregion

#pragma region Object

	/* Camera */
	if (FAILED(pGameInstance->Add_ProtoObject(TEXT("ProtoType_GameObject_Camera_Loading"),
		CLoading_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Logo Obj */
	if (FAILED(pGameInstance->Add_ProtoObject(TEXT("ProtoType_GameObject_LoadingObj_Falmer"),
		CLoadingObj_Falmer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_ProtoObject(TEXT("ProtoType_GameObject_LoadingObj_Skeever"),
		CLoadingObj_Skeever::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Logo UI */
	if (FAILED(pGameInstance->Add_ProtoObject(TEXT("ProtoType_GameObject_UI_Text"),
		CSkyrimUI_SceneChange::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Ready_LoadingClone(const wstring& _strLayerTag)
{
	if (m_eNextLevel != LEVEL_LOGO)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CLoading_Camera::LOADING_CAMERA_DESC LoadingCameraDesc;
		ZeroMemory(&LoadingCameraDesc, sizeof LoadingCameraDesc);

		LoadingCameraDesc.vEye = _float4(0.f, 0.f, -1.5f, 1.f);
		LoadingCameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
		LoadingCameraDesc.fFovY = XMConvertToRadians(60.f);
		LoadingCameraDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
		LoadingCameraDesc.fNear = 0.2f;
		LoadingCameraDesc.fFar = 300.f;
		LoadingCameraDesc.fSpeedPerSec = 0.01f;
		LoadingCameraDesc.fRotationRadianPerSec = XMConvertToRadians(90.f);
		LoadingCameraDesc.fMouseSensitive = 0.1f;

		if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOADING, _strLayerTag, TEXT("ProtoType_GameObject_Camera_Loading"), &LoadingCameraDesc)))
			return E_FAIL;


		if (g_curStage == STAGE_WHITERUN)
		{
			CTransform::TRANSFORM_DESC transformDesc;
			transformDesc.fRotationRadianPerSec = XMConvertToRadians(90.f);
			if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOADING, _strLayerTag, TEXT("ProtoType_GameObject_LoadingObj_Falmer"), &transformDesc)))
				return E_FAIL;
		}
		else if (g_curStage == STAGE_DUNGEON)
		{
			CTransform::TRANSFORM_DESC transformDesc;
			transformDesc.fRotationRadianPerSec = XMConvertToRadians(90.f);
			if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOADING, _strLayerTag, TEXT("ProtoType_GameObject_LoadingObj_Skeever"), &transformDesc)))
				return E_FAIL;
		}
		else
		{
			CTransform::TRANSFORM_DESC transformDesc;
			transformDesc.fRotationRadianPerSec = XMConvertToRadians(90.f);
			if (FAILED(pGameInstance->Add_CloneObject(LEVEL_LOADING, _strLayerTag, TEXT("ProtoType_GameObject_LoadingObj_Falmer"), &transformDesc)))
				return E_FAIL;
		}

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Light(const wstring& _strLayerTag)
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

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, LEVELID _eNextLevel)
{
	CLevel_Loading* pInstance = new CLevel_Loading(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_eNextLevel)))
	{
		MSG_BOX("Fail Create : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
 }

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}

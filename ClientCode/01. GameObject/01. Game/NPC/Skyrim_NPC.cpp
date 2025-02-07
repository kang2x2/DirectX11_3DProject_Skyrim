#include "framework.h"
#include "Skyrim_NPC.h"

#include "GameInstance.h"

#include "Player.h"

#include "SkyrimUI_Talk.h"
#include "SkyrimUI_TalkIcon.h"

CSkyrim_NPC::CSkyrim_NPC(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCreatureObject(_pDevice, _pContext)
{
}

CSkyrim_NPC::CSkyrim_NPC(const CSkyrim_NPC& rhs)
	: CCreatureObject(rhs)
{
}

HRESULT CSkyrim_NPC::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrim_NPC::Initialize_Clone(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")));

	Safe_Release(pGameInstance);

	m_bHasMesh = true;
	m_bCreature = true;
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return S_OK;
}

HRESULT CSkyrim_NPC::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	return S_OK;
}

void CSkyrim_NPC::PriorityTick(_float _fTimeDelta)
{
}

void CSkyrim_NPC::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pNavigationCom != nullptr)
	{
		_vector	vPosition = m_pNavigationCom->Set_OnCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}

	Safe_Release(pGameInstance);

	if (g_curLevel != LEVEL_TOOL)
	{
		m_pTalk->Tick(_fTimeDelta);
		m_pTalkIcon->Tick(_fTimeDelta);
	}
}

void CSkyrim_NPC::LateTick(_float _fTimeDelta)
{
	if (g_curLevel != LEVEL_TOOL)
	{
		m_pTalk->LateTick(_fTimeDelta);
		m_pTalkIcon->LateTick(_fTimeDelta);
	}
}

HRESULT CSkyrim_NPC::Render()
{
	// 메시 몇개
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BondMatrices(m_pShaderCom, i, "g_BoneMatrices")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Bind_MaterialTexture(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;


		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

#ifdef _DEBUG
	if (m_pNavigationCom != nullptr)
		m_pNavigationCom->Render();
#endif
	if (g_curLevel != LEVEL_TOOL)
	{
		m_pTalk->Render();
		m_pTalkIcon->Render();
	}

	return S_OK;
}

HRESULT CSkyrim_NPC::Bind_ShaderResource()
{
	_float4x4 matWorld = m_pTransformCom->Get_WorldMatrix_Float4x4();
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &matWorld)))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	// 뷰, 투영 행렬을 던져준다.
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CSkyrim_NPC::Play_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	m_pModelCom->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
}


_bool CSkyrim_NPC::Get_IsAnimationFin()
{
	return m_pModelCom->Get_IsAnimationFin();
}

string CSkyrim_NPC::Get_CurAnimationName()
{
	return m_pModelCom->Get_CurAnimationName();
}

_uint CSkyrim_NPC::Get_CurFrameIndex()
{
	return m_pModelCom->Get_CurFrameIndex();
}

void CSkyrim_NPC::Set_KeyFrame(_uint _iFrame)
{
	m_pModelCom->Set_KeyFrame(_iFrame);
}

void CSkyrim_NPC::Set_AnimationStop(_bool _bIsStop)
{
	m_pModelCom->Set_AnimationStop(_bIsStop);
}

void CSkyrim_NPC::Start_Talk()
{
	m_pTalk->Start_Talk();
}
_uint CSkyrim_NPC::Get_CurTalkID()
{
	return 0;
}
_uint CSkyrim_NPC::Get_CurTextID()
{
	return m_pTalk->Get_CurTextID();
}

void CSkyrim_NPC::Set_VisibleTalkIcon(_bool _bIsVisible)
{
	m_pTalkIcon->Set_VisibleTalkIcon(_bIsVisible);
}

HRESULT CSkyrim_NPC::Ready_Component()
{
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	if (g_curLevel != LEVEL_TOOL)
	{
		/* Com_Navigation */
		CNavigation::DESC_NAVIGATION		NavigationDesc;
		NavigationDesc.iCurIndex = -1;

		if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_WhiteRun"),
			TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NavigationDesc)))
			return E_FAIL;

		m_pNavigationCom->Set_CurCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_vector	vPosition = m_pNavigationCom->Set_OnCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}


	return S_OK;
}

CGameObject* CSkyrim_NPC::Clone(void* _pArg)
{
	return nullptr;
}

void CSkyrim_NPC::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);
}

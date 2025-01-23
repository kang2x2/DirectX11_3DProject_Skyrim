#include "framework.h"
#include "Monster.h"

#include "GameInstance.h"

#include "Player.h"
#include "SkyrimUI_MonsterHpBar.h"

CMonster::CMonster(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCreatureObject(_pDevice, _pContext)
{
}

CMonster::CMonster(const CMonster& rhs)
	: CCreatureObject(rhs)
{
}

HRESULT CMonster::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CMonster::Initialize_Clone(void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")));
	m_pHpBar = dynamic_cast<CSkyrimUI_MonsterHpBar*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("UI_MonsterHpbar")));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CMonster::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	return S_OK;
}

void CMonster::PriorityTick(_float _fTimeDelta)
{
}

void CMonster::Tick(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pNavigationCom != nullptr)
	{
		_vector	vPosition = m_pNavigationCom->Set_OnCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}

	Safe_Release(pGameInstance);
}

void CMonster::LateTick(_float _fTimeDelta)
{
}

HRESULT CMonster::Render()
{
#ifdef _DEBUG
	if (m_pNavigationCom != nullptr)
		m_pNavigationCom->Render();
#endif

	return S_OK;
}

HRESULT CMonster::Bind_ShaderResource()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

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

void CMonster::Play_Animation(_bool _bIsLoop, string _strAnimationName, _uint _iChangeIndex, _bool _bIsReset, _bool _bIsQuickChange)
{
	m_pModelCom->SetUp_Animation(_bIsLoop, _strAnimationName, _iChangeIndex, _bIsReset, _bIsQuickChange);
}


_bool CMonster::Get_IsAnimationFin()
{
	return m_pModelCom->Get_IsAnimationFin();
}

string CMonster::Get_CurAnimationName()
{
	return m_pModelCom->Get_CurAnimationName();
}

_uint CMonster::Get_CurFrameIndex()
{
	return m_pModelCom->Get_CurFrameIndex();
}

void CMonster::Set_AnimationStop(_bool _bIsStop)
{
	m_pModelCom->Set_AnimationStop(_bIsStop);
}

HRESULT CMonster::Ready_Component()
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

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL; 

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	if (g_curLevel != LEVEL_TOOL)
	{
		/* Com_Navigation */
		CNavigation::DESC_NAVIGATION		NavigationDesc;
		NavigationDesc.iCurIndex = -1;
		if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_Dungeon"),
			TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NavigationDesc)))
			return E_FAIL;
		//if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Navigation_WhiteRun"),
		//	TEXT("Com_Navigation"), (CComponent**)&m_pNavigationCom, &NavigationDesc)))
		//	return E_FAIL;

		m_pNavigationCom->Set_CurCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		_vector	vPosition = m_pNavigationCom->Set_OnCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	}


	return S_OK;
}

CGameObject* CMonster::Clone(void* _pArg)
{
	return nullptr;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);
}

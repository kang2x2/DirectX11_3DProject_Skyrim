#include "framework.h"
#include "NPC_Guard.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Player.h"

#include "Guard_Weapon.h"

CNPC_Guard::CNPC_Guard(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrim_NPC(_pDevice, _pContext)
{
}

CNPC_Guard::CNPC_Guard(const CNPC_Guard& rhs)
	: CSkyrim_NPC(rhs)
{
}

HRESULT CNPC_Guard::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CNPC_Guard::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CNPC_Guard::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	__super::Initialize_Clone(pArg);

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	if (FAILED(Ready_Part()))
		return E_FAIL;

	m_strName = TEXT("NPC_Guard");
	m_pTransformCom->Set_Speed(3.f);

	Play_Animation(true, "mt_idle");

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CNPC_Guard::PriorityTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		for (auto& iter : m_vecNpcPart)
		{
			if (iter != nullptr)
				iter->PriorityTick(_fTimeDelta);
		}
	}
}

void CNPC_Guard::Tick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (m_pNavigationCom != nullptr)
		{
			_vector	vPosition = m_pNavigationCom->Set_OnCell(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		}

		Safe_Release(pGameInstance);

		for (auto& iter : m_vecNpcPart)
		{
			if (iter != nullptr)
				iter->Tick(_fTimeDelta);
		}

		_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

		for (size_t i = 0; i < GUARDCAPTAIN_COL_END; ++i)
		{
			if (m_pVecCollider[i] != nullptr)
			{
				m_pVecCollider[i]->Update(matWorld);
			}
		}


		if (g_curLevel != LEVEL_TOOL)
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			for (_int i = 0; i < m_pVecCollider.size(); ++i)
			{
				pGameInstance->Collision_Out(m_pVecCollider[i],
					dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
			}

			Safe_Release(pGameInstance);
		}
	}
}

void CNPC_Guard::LateTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		for (auto& iter : m_vecNpcPart)
		{
			if (iter != nullptr)
				iter->LateTick(_fTimeDelta);
		}
	}

#ifdef _DEBUG

	for (auto& collider : m_pVecCollider)
	{
		/* 콜라이더를 그 때도 오리지널을 그리는 게 아니라 행렬을 곱해놓은 aabb를 그린다. */
		if (collider != nullptr)
			m_pRendererCom->Add_Debug(collider);
	}

#endif 
	if (CGameInstance::GetInstance()->IsIn_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 2.0f))
	{
		m_pModelCom->Play_Animation(_fTimeDelta);
		m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (g_curLevel == LEVEL_GAMEPLAY)
	{
		for (auto& collider : m_pVecCollider)
			collider->IsCollision(dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));

		pGameInstance->Collision_AABBTransition(m_pVecCollider[GUARDCAPTAIN_COL_AABB], dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
	}

	Safe_Release(pGameInstance);
}

HRESULT CNPC_Guard::Render()
{
	for (auto& iter : m_vecNpcPart)
	{
		if (iter != nullptr)
			iter->Render();
	}

	__super::Bind_ShaderResource();
	__super::Render();

	return S_OK;
}

void CNPC_Guard::Set_WeaponSocket(const char* _strBoneName)
{
	dynamic_cast<CGuard_Weapon*>(m_vecNpcPart[PART_WEAPON])->
		Set_SoketBone(m_pModelCom->Get_BonePtr(_strBoneName));
}

void CNPC_Guard::Set_Idle()
{
	Play_Animation(true, "mt_idle");
}

HRESULT CNPC_Guard::Ready_Part()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	/* For. Weapon */
	CGuard_Weapon::WEAPON_DESC WeaponPartDesc;
	WeaponPartDesc.pParent = this;
	WeaponPartDesc.pParentTransform = m_pTransformCom;
	WeaponPartDesc.pSocketBone = m_pModelCom->Get_BonePtr("WeaponSword");
	WeaponPartDesc.matSocketPivot = m_pModelCom->Get_PivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Guard_Weapon"), &WeaponPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecNpcPart.push_back(pPart);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CNPC_Guard::Ready_Component(_uint _iLevel)
{
	if (FAILED(__super::Add_CloneComponent(_iLevel, m_strModelComTag,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	__super::Ready_Component();

#pragma region Collider

	m_pVecCollider.resize(GUARDCAPTAIN_COL_END);

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(0.1f, 0.7f, 0.1f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pVecCollider[GUARDCAPTAIN_COL_AABB], &AABBDesc)))
		return E_FAIL;

	m_pVecCollider[GUARDCAPTAIN_COL_AABB]->Set_OwnerObj(this);

	/* DETECTION */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc = {};
	SphereDesc.fRadius = 5.f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_Detection"), (CComponent**)&m_pVecCollider[GUARDCAPTAIN_COL_DETECTION], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[GUARDCAPTAIN_COL_DETECTION]->Set_OwnerObj(this);

	/* DIALOG */
	SphereDesc.fRadius = 1.f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_Dialog"), (CComponent**)&m_pVecCollider[GUARDCAPTAIN_COL_DIALOG], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[GUARDCAPTAIN_COL_DIALOG]->Set_OwnerObj(this);

#pragma endregion

	return S_OK;
}

HRESULT CNPC_Guard::Ready_State()
{
	m_fRunSpeed = 2.5f;
	m_fWalkSpeed = 1.f;
	m_fHp = 500;
	m_iAtk = 40;

	return S_OK;
}

CNPC_Guard* CNPC_Guard::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CNPC_Guard* pInstance = new CNPC_Guard(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CNPC_Guard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_Guard::Clone(void* _pArg)
{
	CNPC_Guard* pInstance = new CNPC_Guard(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_Guard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_Guard::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CNPC_Guard* pInstance = new CNPC_Guard(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_Guard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_Guard::Free()
{
	__super::Free();

	for (auto& iter : m_vecNpcPart)
	{
		if (iter != nullptr)
			Safe_Release(iter);
	}

	for (auto& iter : m_pVecCollider)
		Safe_Release(iter);
}

#include "framework.h"
#include "Spider.h"

#include "GameInstance.h"

#include "StateManager_Spider.h"

#include "Bone.h"
#include "Player.h"

#include "Spider_Mouth.h"
#include "SkyrimUI_MonsterHpBar.h"

CSpider::CSpider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CSpider::CSpider(const CSpider& rhs)
	: CMonster(rhs)
{
}

HRESULT CSpider::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSpider::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CSpider::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	__super::Initialize_Clone(pArg);

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	if (FAILED(Ready_Part()))
		return E_FAIL;

	m_bHasMesh = true;
	m_bCreature = true;
	m_strName = TEXT("Spider");
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fDissloveTime = 2.5f;

	m_pTransformCom->Set_Speed(3.f);

	Play_Animation(true, "mainidle");

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CSpider::PriorityTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		for (auto& iter : m_vecMonsterPart)
		{
			if (iter != nullptr)
				iter->PriorityTick(_fTimeDelta);
		}
	}
}

void CSpider::Tick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		m_pModelCom->Play_Animation(_fTimeDelta);

		for (auto& iter : m_vecMonsterPart)
		{
			if (iter != nullptr)
				iter->Tick(_fTimeDelta);
		}

		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Update(_fTimeDelta);

		__super::Tick(_fTimeDelta);

		_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

		for (size_t i = 0; i < SPIDER_COL_END; ++i)
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

			if (m_pPlayer->Get_IsAttack())
			{
				if (pGameInstance->Collision_Enter(m_pVecCollider[SPIDER_COL_AABB],
					dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_WEAPON)->Get_Component(TEXT("Com_Collider_OBB")))))
				{
					m_pHpBar->Set_Monster(this);

					pGameInstance->PlaySoundFile(TEXT("wpn_impact_blade_fleshdraugr_03.wav"), CHANNEL_ATK, 1.f);

					pGameInstance->Add_CloneObject(g_curLevel, TEXT("Layer_Effect"), TEXT("ProtoType_GameObject_BloodSpot"));
					m_fHp -= m_pPlayer->GetAtk();
				}
			}

			pGameInstance->Collision_Out(m_pVecCollider[SPIDER_COL_AABB],
				dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_WEAPON)->Get_Component(TEXT("Com_Collider_OBB"))));


			for (_int i = 0; i < m_pVecCollider.size(); ++i)
			{
				pGameInstance->Collision_Out(m_pVecCollider[i],
					dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
			}

			Safe_Release(pGameInstance);
		}

		if (m_bIsDissloving)
		{
			m_fCurDissloveTime += _fTimeDelta;

			if (m_fCurDissloveTime > m_fDissloveTime)
				m_bReadyDead = true;
		}

	}
}

void CSpider::LateTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Late_Update(_fTimeDelta);

		for (auto& iter : m_vecMonsterPart)
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

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>
		(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")));

	if (g_curLevel == LEVEL_GAMEPLAY)
	{
		for (auto& collider : m_pVecCollider)
			collider->IsCollision(dynamic_cast<CCollider*>(pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));

		pGameInstance->Collision_AABBTransition(m_pVecCollider[SPIDER_COL_AABB], dynamic_cast<CCollider*>(pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
	}

	Safe_Release(pGameInstance);
}

HRESULT CSpider::Render()
{
	for (auto& iter : m_vecMonsterPart)
	{
		if (iter != nullptr)
			iter->Render();
	}

	__super::Bind_ShaderResource();

	if (m_bIsDissloving)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 0)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fCurDissloveTime, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveTime", &m_fDissloveTime, sizeof(_float))))
			return E_FAIL;
	}


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

		if (!m_bIsDissloving)
		{
			if (FAILED(m_pShaderCom->Begin(0)))
				return E_FAIL;
		}
		else if (m_bIsDissloving)
		{
			if (FAILED(m_pShaderCom->Begin(1)))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	__super::Render();

	return S_OK;

}

HRESULT CSpider::Set_State(SPIDER_STATE _eState)
{
	m_pStateManager->Set_State(_eState);

	return S_OK;
}

HRESULT CSpider::Ready_Part()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	/* For. Spider_Weapon */
	CSpider_Mouth::MOUTH_DESC MouthPartDesc;
	MouthPartDesc.pParent = this;
	MouthPartDesc.pParentTransform = m_pTransformCom;
	MouthPartDesc.pSocketBone = m_pModelCom->Get_BonePtr("FangL[02]");
	MouthPartDesc.matSocketPivot = m_pModelCom->Get_PivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Spider_Mouth"), &MouthPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecMonsterPart.push_back(pPart);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CSpider::Ready_Component(_uint _iLevel)
{
	if (FAILED(__super::Add_CloneComponent(_iLevel, m_strModelComTag,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	__super::Ready_Component();

#pragma region Collider

	m_pVecCollider.resize(SPIDER_COL_END);

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(0.5f, 1.f, 0.5f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pVecCollider[SPIDER_COL_AABB], &AABBDesc)))
		return E_FAIL;

	m_pVecCollider[SPIDER_COL_AABB]->Set_OwnerObj(this);

	/* DETECTION */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc = {};
	SphereDesc.fRadius = 6.f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_Detection"), (CComponent**)&m_pVecCollider[SPIDER_COL_DETECTION], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[SPIDER_COL_DETECTION]->Set_OwnerObj(this);

	/* MISS DETECTION */
	SphereDesc.fRadius = 9.f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_MissDetection"), (CComponent**)&m_pVecCollider[SPIDER_COL_MISSDETECTION], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[SPIDER_COL_MISSDETECTION]->Set_OwnerObj(this);

	/* RUN ATTACK */
	SphereDesc.fRadius = 1.5f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_AtkRound"), (CComponent**)&m_pVecCollider[SPIDER_COL_ATKROUND], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[SPIDER_COL_ATKROUND]->Set_OwnerObj(this);

#pragma endregion

	return S_OK;
}

HRESULT CSpider::Ready_State()
{
	m_fRunSpeed = 2.5f;
	m_fWalkSpeed = 1.5f;
	m_fHp = 100;
	m_fMaxHp = 100;
	m_iAtk = 5;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pStateManager = CStateManager_Spider::Create(this,
		pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Player"), TEXT("Player")),
		m_pTransformCom, m_pNavigationCom, m_pVecCollider);

	Safe_Release(pGameInstance);

	return S_OK;
}

CSpider* CSpider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CSpider* pInstance = new CSpider(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpider::Clone(void* _pArg)
{
	CSpider* pInstance = new CSpider(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpider::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CSpider* pInstance = new CSpider(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpider::Free()
{
	__super::Free();

	for (auto& iter : m_vecMonsterPart)
	{
		if (iter != nullptr)
			Safe_Release(iter);
	}

	for (auto& iter : m_pVecCollider)
		Safe_Release(iter);

	Safe_Release(m_pStateManager);
}

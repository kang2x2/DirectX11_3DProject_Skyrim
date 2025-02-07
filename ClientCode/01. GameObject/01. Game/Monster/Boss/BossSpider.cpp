#include "framework.h"
#include "BossSpider.h"

#include "GameInstance.h"

#include "StateManager_BossSpider.h"

#include "Bone.h"
#include "Player.h"

#include "BossSpider_Mouth.h"
#include "BossSpider_Left.h"
#include "BossSpider_Right.h"

#include "SkyrimUI_BossHpBar.h"

CBossSpider::CBossSpider(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CMonster(_pDevice, _pContext)
{
}

CBossSpider::CBossSpider(const CBossSpider& rhs)
	: CMonster(rhs)
{
}

HRESULT CBossSpider::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CBossSpider::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CBossSpider::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	__super::Initialize_Clone(pArg);

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	if (FAILED(Ready_Part()))
		return E_FAIL;

	m_strName = TEXT("King Spider");
	m_fDissloveTime = 5.f;
	m_pTransformCom->Set_Speed(6.f);

	Play_Animation(true, "combatidle");

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CBossSpider::PriorityTick(_float _fTimeDelta)
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

void CBossSpider::Tick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		m_pModelCom->Play_Animation(_fTimeDelta * m_fAnimationSpeed);

		for (auto& iter : m_vecMonsterPart)
		{
			if (iter != nullptr)
				iter->Tick(_fTimeDelta);
		}

		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Update(_fTimeDelta);

		// Boss는 최초에는 공중에 있어야 되기에 
		if (m_bIsCombat)
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

		_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

		for (size_t i = 0; i < BOSSSPIDER_COL_END; ++i)
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
				if (pGameInstance->Collision_Enter(m_pVecCollider[BOSSSPIDER_COL_AABB],
					dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_WEAPON)->Get_Component(TEXT("Com_Collider_OBB")))))
				{
					pGameInstance->PlaySoundFile(TEXT("wpn_impact_blade_fleshdraugr_03.wav"), CHANNEL_ATK, 1.f);

					pGameInstance->Add_CloneObject(g_curLevel, TEXT("Layer_Effect"), TEXT("ProtoType_GameObject_BloodSpot"));
					m_fHp -= m_pPlayer->GetAtk();
				}
			}

			pGameInstance->Collision_Out(m_pVecCollider[BOSSSPIDER_COL_AABB],
				dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_WEAPON)->Get_Component(TEXT("Com_Collider_OBB"))));

			for (_int i = 0; i < m_pVecCollider.size(); ++i)
			{
				pGameInstance->Collision_Out(m_pVecCollider[i],
					dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
			}

			Safe_Release(pGameInstance);

			if (m_bIsDissloving)
			{
				m_fCurDissloveTime += _fTimeDelta;

				if (m_fCurDissloveTime > m_fDissloveTime)
				{
					m_bReadyDead = true;
				}
			}

			if (g_curLevel != LEVEL_TOOL)
				m_pHpBar->Tick(m_fHp, _fTimeDelta);
		}
	}
}

void CBossSpider::LateTick(_float _fTimeDelta)
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

	if (g_curLevel == LEVEL_GAMEPLAY)
	{
		for (auto& collider : m_pVecCollider)
			collider->IsCollision(dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));

		pGameInstance->Collision_AABBTransition(dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))), m_pVecCollider[BOSSSPIDER_COL_AABB]);
	}

	Safe_Release(pGameInstance);

	if (g_curLevel != LEVEL_TOOL)
		m_pHpBar->LateTick(_fTimeDelta);
}

HRESULT CBossSpider::Render()
{
	for (auto& iter : m_vecMonsterPart)
	{
		if (iter != nullptr)
			iter->Render();
	}

	__super::Bind_ShaderResource();

	if (g_curLevel != LEVEL_TOOL)
		m_pHpBar->Render();

	__super::Render();

	return S_OK;
}

HRESULT CBossSpider::Set_State(BOSSSPIDER_STATE _eState)
{
	m_pStateManager->Set_State(_eState);

	return S_OK;
}

HRESULT CBossSpider::Ready_Part()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	/* For. BossSpider_Mouth */
	CBossSpider_Mouth::MOUTH_DESC MouthPartDesc;
	MouthPartDesc.pParent = this;
	MouthPartDesc.pParentTransform = m_pTransformCom;
	MouthPartDesc.pSocketBone = m_pModelCom->Get_BonePtr("FangR[02]");
	MouthPartDesc.matSocketPivot = m_pModelCom->Get_PivotMatrix();
	
	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_BossSpider_Mouth"), &MouthPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecMonsterPart.push_back(pPart);

	/* For. BossSpider_Left */
	CBossSpider_Left::LEFT_DESC LeftPartDesc;
	LeftPartDesc.pParent = this;
	LeftPartDesc.pParentTransform = m_pTransformCom;
	LeftPartDesc.pSocketBone = m_pModelCom->Get_BonePtr("ArmLClaw");
	LeftPartDesc.matSocketPivot = m_pModelCom->Get_PivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_BossSpider_Left"), &LeftPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecMonsterPart.push_back(pPart);

	/* For. BossSpider_Right */
	CBossSpider_Right::RIGHT_DESC RightPartDesc;
	RightPartDesc.pParent = this;
	RightPartDesc.pParentTransform = m_pTransformCom;
	RightPartDesc.pSocketBone = m_pModelCom->Get_BonePtr("ArmRClaw");
	RightPartDesc.matSocketPivot = m_pModelCom->Get_PivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_BossSpider_Right"), &RightPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecMonsterPart.push_back(pPart);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CBossSpider::Ready_Component(_uint _iLevel)
{
	if (FAILED(__super::Add_CloneComponent(_iLevel, m_strModelComTag,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	__super::Ready_Component();

#pragma region Collider

	m_pVecCollider.resize(BOSSSPIDER_COL_END);

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(1.7f, 1.3f, 1.7f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pVecCollider[BOSSSPIDER_COL_AABB], &AABBDesc)))
		return E_FAIL;

	m_pVecCollider[BOSSSPIDER_COL_AABB]->Set_OwnerObj(this);

	/* Detection */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc = {};
	SphereDesc.fRadius = 13.f;
	SphereDesc.vCenter = _float3(0.f, -10.f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_Detection"), (CComponent**)&m_pVecCollider[BOSSSPIDER_COL_DETECTION], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[BOSSSPIDER_COL_DETECTION]->Set_OwnerObj(this);

	/* Charge Round */
	SphereDesc.fRadius = 10.f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_ChargeRound"), (CComponent**)&m_pVecCollider[BOSSSPIDER_COL_CHARGEROUND], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[BOSSSPIDER_COL_CHARGEROUND]->Set_OwnerObj(this);

	/* Attack */
	SphereDesc.fRadius = 4.5f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_AtkRound"), (CComponent**)&m_pVecCollider[BOSSSPIDER_COL_ATKROUND], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[BOSSSPIDER_COL_ATKROUND]->Set_OwnerObj(this);


#pragma endregion

	return S_OK;
}

HRESULT CBossSpider::Ready_State()
{
	m_fRunSpeed = 3.5f;
	m_fWalkSpeed = 1.5f;
	m_fHp = 450;
	m_fMaxHp = 450;
	m_iAtk = 10;

	m_tBossDesc.fSprintSpeed = 10.f;
	m_tBossDesc.fChargeSpeed = 7.f;
	m_tBossDesc.fChargeDamage = 30.f;
	m_tBossDesc.fBiteDamage = 10.f;
	m_tBossDesc.fChopDamage = 5.f;
	m_tBossDesc.fNidleDamage = 15.f;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pStateManager = CStateManager_BossSpider::Create(this,
		pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Player"), TEXT("Player")),
		m_pTransformCom, m_pNavigationCom, m_pVecCollider);

	if (g_curLevel != LEVEL_TOOL)
	{
		/* HpBar */
		if (FAILED(pGameInstance->Add_CloneObject(LEVEL_GAMEPLAY, TEXT("Boss_HpBar"), TEXT("ProtoType_GameObject_UI_BossHpBar"))))
			return E_FAIL;

		m_pHpBar = dynamic_cast<CSkyrimUI_BossHpBar*>
			(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Boss_HpBar"), TEXT("UI_BossHpbar")));
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

void CBossSpider::Set_BarShow()
{
	if (g_curLevel != LEVEL_TOOL)
		m_pHpBar->Set_BarShow();
}

CBossSpider* CBossSpider::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBossSpider* pInstance = new CBossSpider(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CBossSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossSpider::Clone(void* _pArg)
{
	CBossSpider* pInstance = new CBossSpider(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CBossSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBossSpider::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CBossSpider* pInstance = new CBossSpider(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CBossSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBossSpider::Free()
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

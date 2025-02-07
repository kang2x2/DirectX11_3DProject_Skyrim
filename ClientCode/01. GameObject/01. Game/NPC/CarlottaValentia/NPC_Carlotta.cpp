#include "framework.h"
#include "NPC_Carlotta.h"

#include "GameInstance.h"

#include "StateManager_FalmerOH.h"

#include "Bone.h"
#include "Player.h"

#include "SkyrimUI_Talk.h"
#include "SkyrimUI_TalkIcon.h"

CNPC_Carlotta::CNPC_Carlotta(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrim_NPC(_pDevice, _pContext)
{
}

CNPC_Carlotta::CNPC_Carlotta(const CNPC_Carlotta& rhs)
	: CSkyrim_NPC(rhs)
{
}

HRESULT CNPC_Carlotta::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CNPC_Carlotta::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CNPC_Carlotta::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	__super::Initialize_Clone(pArg);

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	if (g_curLevel != LEVEL_TOOL)
	{
		if (FAILED(Ready_Talk()))
			return E_FAIL;
	}

	if (FAILED(Ready_Part()))
		return E_FAIL;

	m_strName = TEXT("NPC_Carlotta");
	m_pTransformCom->Set_Speed(3.f);

	Play_Animation(true, "animobjectcounteridleloopright");

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CNPC_Carlotta::PriorityTick(_float _fTimeDelta)
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

void CNPC_Carlotta::Tick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		for (auto& iter : m_vecNpcPart)
		{
			if (iter != nullptr)
				iter->Tick(_fTimeDelta);
		}

		__super::Tick(_fTimeDelta);

		_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

		for (size_t i = 0; i < CARLOTTA_COL_END; ++i)
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

void CNPC_Carlotta::LateTick(_float _fTimeDelta)
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

		// pGameInstance->Collision_AABBTransition(m_pVecCollider[CARLOTTA_COL_AABB], dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
		pGameInstance->Collision_AABBTransition(dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))), m_pVecCollider[CARLOTTA_COL_AABB]);
	}

	Safe_Release(pGameInstance);
}

HRESULT CNPC_Carlotta::Render()
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

HRESULT CNPC_Carlotta::Set_State(CARLOTTA_STATE _eState)
{
	// m_pStateManager->Set_State(_eState);

	return S_OK;
}

void CNPC_Carlotta::Set_WeaponSocket(const char* _strBoneName)
{
	//dynamic_cast<CFalmerOH_Weapon*>(m_vecNpcPart[PART_WEAPON])->Set_SoketBone(
	//	m_pModelCom->Get_BonePtr(_strBoneName));
}

HRESULT CNPC_Carlotta::Ready_Part()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CNPC_Carlotta::Ready_Component(_uint _iLevel)
{
	if (FAILED(__super::Add_CloneComponent(_iLevel, m_strModelComTag,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	__super::Ready_Component();

#pragma region Collider

	m_pVecCollider.resize(CARLOTTA_COL_END);

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(0.8f, 0.7f, 0.8f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pVecCollider[CARLOTTA_COL_AABB], &AABBDesc)))
		return E_FAIL;

	m_pVecCollider[CARLOTTA_COL_AABB]->Set_OwnerObj(this);

	/* DIALOG */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc = {};
	SphereDesc.fRadius = 1.5f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_Detection"), (CComponent**)&m_pVecCollider[CARLOTTA_COL_DIALOG], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[CARLOTTA_COL_DIALOG]->Set_OwnerObj(this);

	/* COL_ACTIVE */
	SphereDesc.fRadius = 2.f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_MissDetection"), (CComponent**)&m_pVecCollider[CARLOTTA_COL_ACTIVE], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[CARLOTTA_COL_ACTIVE]->Set_OwnerObj(this);

#pragma endregion

	return S_OK;
}

HRESULT CNPC_Carlotta::Ready_State()
{
	m_fRunSpeed = 2.5f;
	m_fWalkSpeed = 1.5f;
	m_fHp = 150;
	m_iAtk = 10;

	return S_OK;
}

HRESULT CNPC_Carlotta::Ready_Talk()
{
	CSkyrimUI_Talk::TALK_DESC talkDesc;
	talkDesc.pOwner = this;
	talkDesc.m_iShowDialogIndex = 1;
	talkDesc.m_strOwnerName = TEXT("카를로타");
	talkDesc.m_mapNpcTalkData.insert(make_pair(0, vector<wstring>{
		{ L"음?"},
		{ L"둘러보시오."},
	}));

	CSkyrimUI_Talk::PLAYERTALK_DESC playerTalkDesc;
	playerTalkDesc.m_iShowTextID = 0;
	playerTalkDesc.m_strPlayerTalk = { L"무엇을 팔고 있지?" };
	talkDesc.m_vecPlayerTalkDesc.push_back(playerTalkDesc);

	CSkyrimUI_TalkIcon::TALKICON_DESC IconDesc;
	IconDesc.strTargetName = TEXT("카를로타");

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pTalk = dynamic_cast<CSkyrimUI_Talk*>(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_Talk"), &talkDesc));
	m_pTalkIcon = dynamic_cast<CSkyrimUI_TalkIcon*>(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_TalkIcon"), &IconDesc));

	Safe_Release(pGameInstance);

	return S_OK;
}

CNPC_Carlotta* CNPC_Carlotta::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CNPC_Carlotta* pInstance = new CNPC_Carlotta(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CNPC_Carlotta");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_Carlotta::Clone(void* _pArg)
{
	CNPC_Carlotta* pInstance = new CNPC_Carlotta(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_Carlotta");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_Carlotta::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CNPC_Carlotta* pInstance = new CNPC_Carlotta(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_Carlotta");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_Carlotta::Free()
{
	__super::Free();

	for (auto& iter : m_vecNpcPart)
	{
		if (iter != nullptr)
			Safe_Release(iter);
	}

	for (auto& iter : m_pVecCollider)
		Safe_Release(iter);

	// Safe_Release(m_pStateManager);

}

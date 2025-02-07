#include "framework.h"
#include "NPC_GuardSupply.h"

#include "GameInstance.h"

#include "StateManager_GuardSupply.h"

#include "Bone.h"
#include "Player.h"

#include "SkyrimUI_Talk.h"
#include "SkyrimUI_TalkIcon.h"

CNPC_GuardSupply::CNPC_GuardSupply(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrim_NPC(_pDevice, _pContext)
{
}

CNPC_GuardSupply::CNPC_GuardSupply(const CNPC_GuardSupply& rhs)
	: CSkyrim_NPC(rhs)
{
}

HRESULT CNPC_GuardSupply::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CNPC_GuardSupply::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CNPC_GuardSupply::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
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

	m_strName = TEXT("NPC_GuardSupply");
	m_pTransformCom->Set_Speed(3.f);

	Play_Animation(true, "mt_idle");

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CNPC_GuardSupply::PriorityTick(_float _fTimeDelta)
{
}

void CNPC_GuardSupply::Tick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		__super::Tick(_fTimeDelta);

		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Update(_fTimeDelta);

		__super::Tick(_fTimeDelta);

		_matrix matWorld = m_pTransformCom->Get_WorldMatrix();

		for (size_t i = 0; i < GUARDSUPPLY_COL_END; ++i)
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

void CNPC_GuardSupply::LateTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		__super::LateTick(_fTimeDelta);

		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Late_Update(_fTimeDelta);
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

		pGameInstance->Collision_AABBTransition(m_pVecCollider[GUARDSUPPLY_COL_AABB], dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
	}

	Safe_Release(pGameInstance);
}

HRESULT CNPC_GuardSupply::Render()
{
	__super::Bind_ShaderResource();
	__super::Render();

	return S_OK;
}

HRESULT CNPC_GuardSupply::Set_State(GUARDSUPPLY_STATE _eState)
{
	m_pStateManager->Set_State(_eState);

	return S_OK;
}

void CNPC_GuardSupply::Set_Idle()
{
	Play_Animation(true, "mt_idle");
}

HRESULT CNPC_GuardSupply::Ready_Component(_uint _iLevel)
{
	if (FAILED(__super::Add_CloneComponent(_iLevel, m_strModelComTag,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	__super::Ready_Component();

#pragma region Collider

	m_pVecCollider.resize(GUARDSUPPLY_COL_END);

	/* AABB */
	CBounding_AABB::BOUNDING_AABB_DESC AABBDesc = {};
	AABBDesc.vExtents = _float3(0.1f, 0.7f, 0.1f);
	AABBDesc.vCenter = _float3(0.f, AABBDesc.vExtents.y, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_AABB"),
		TEXT("Com_Collider_AABB"), (CComponent**)&m_pVecCollider[GUARDSUPPLY_COL_AABB], &AABBDesc)))
		return E_FAIL;

	m_pVecCollider[GUARDSUPPLY_COL_AABB]->Set_OwnerObj(this);

	/* DIALOG */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc = {};
	SphereDesc.fRadius = 1.5f;
	SphereDesc.vCenter = _float3(0.f, 0.5f, 0.f);

	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("ProtoType_Component_Collider_Sphere"),
		TEXT("Com_Collider_Dialog"), (CComponent**)&m_pVecCollider[GUARDSUPPLY_COL_DIALOG], &SphereDesc)))
		return E_FAIL;

	m_pVecCollider[GUARDSUPPLY_COL_DIALOG]->Set_OwnerObj(this);

#pragma endregion

	return S_OK;
}

HRESULT CNPC_GuardSupply::Ready_State()
{
	m_fRunSpeed = 2.5f;
	m_fWalkSpeed = 1.f;
	m_fHp = 500;
	m_iAtk = 40;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pStateManager = CStateManager_GuardSupply::Create(this,
		pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Player"), TEXT("Player")),
		m_pTransformCom, m_pNavigationCom, m_pVecCollider);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CNPC_GuardSupply::Ready_Talk()
{
	CSkyrimUI_Talk::TALK_DESC talkDesc;
	talkDesc.pOwner = this;
	talkDesc.m_iShowDialogIndex = 0;
	talkDesc.m_strOwnerName = TEXT("보급병");
	talkDesc.m_mapNpcTalkData.insert(make_pair(0, vector<wstring>{
		{ L"음?"},
		{ L"장비를 받으러 온건가? 용케 그 꼴로 여기까지 왔군." },
		{ L"자 받으라고." },
	}));

	CSkyrimUI_Talk::PLAYERTALK_DESC playerTalkDesc;
	playerTalkDesc.m_iShowTextID = 0;
	playerTalkDesc.m_strPlayerTalk = { L"장비를 받으러 왔다." };
	talkDesc.m_vecPlayerTalkDesc.push_back(playerTalkDesc);

	CSkyrimUI_TalkIcon::TALKICON_DESC IconDesc;
	IconDesc.strTargetName = TEXT("보급병");

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pTalk = dynamic_cast<CSkyrimUI_Talk*>(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_Talk"), &talkDesc));
	m_pTalkIcon = dynamic_cast<CSkyrimUI_TalkIcon*>(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_TalkIcon"), &IconDesc));

	Safe_Release(pGameInstance);

	return S_OK;
}

CNPC_GuardSupply* CNPC_GuardSupply::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CNPC_GuardSupply* pInstance = new CNPC_GuardSupply(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CNPC_GuardSupply");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_GuardSupply::Clone(void* _pArg)
{
	CNPC_GuardSupply* pInstance = new CNPC_GuardSupply(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_GuardSupply");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_GuardSupply::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CNPC_GuardSupply* pInstance = new CNPC_GuardSupply(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_GuardSupply");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_GuardSupply::Free()
{
	__super::Free();

	for (auto& iter : m_pVecCollider)
		Safe_Release(iter);

	Safe_Release(m_pStateManager);
}

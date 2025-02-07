#include "framework.h"
#include "NPC_GuardCaptain.h"

#include "GameInstance.h"

#include "StateManager_GuardCaptain.h"

#include "Bone.h"
#include "Player.h"

#include "GuardCaptain_Weapon.h"

#include "SkyrimUI_Talk.h"
#include "SkyrimUI_TalkIcon.h"

CNPC_GuardCaptain::CNPC_GuardCaptain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrim_NPC(_pDevice, _pContext)
{
}

CNPC_GuardCaptain::CNPC_GuardCaptain(const CNPC_GuardCaptain& rhs)
	: CSkyrim_NPC(rhs)
{
}

HRESULT CNPC_GuardCaptain::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CNPC_GuardCaptain::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CNPC_GuardCaptain::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
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

	m_strName = TEXT("NPC_GuardCaptain");
	m_pTransformCom->Set_Speed(3.f);

	Play_Animation(true, "mt_idle");

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CNPC_GuardCaptain::PriorityTick(_float _fTimeDelta)
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

void CNPC_GuardCaptain::Tick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		__super::Tick(_fTimeDelta);

		m_pModelCom->Play_Animation(_fTimeDelta);

		for (auto& iter : m_vecNpcPart)
		{
			if (iter != nullptr)
				iter->Tick(_fTimeDelta);
		}

		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Update(_fTimeDelta);

		__super::Tick(_fTimeDelta);

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

void CNPC_GuardCaptain::LateTick(_float _fTimeDelta)
{
	if (!g_bIsPause)
	{
		__super::LateTick(_fTimeDelta);

		if (g_curLevel == LEVEL_GAMEPLAY)
			m_pStateManager->Late_Update(_fTimeDelta);

		for (auto& iter : m_vecNpcPart)
		{
			if (iter != nullptr)
				iter->LateTick(_fTimeDelta);
		}
	}

#ifdef _DEBUG

	for (auto& collider : m_pVecCollider)
	{
		/* 콜라이더를 그릴 때도 오리지널을 그리는 게 아니라 행렬을 곱해놓은 aabb를 그린다. */
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

		pGameInstance->Collision_AABBTransition(m_pVecCollider[GUARDCAPTAIN_COL_AABB], dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB"))));
	}

	Safe_Release(pGameInstance);
}

HRESULT CNPC_GuardCaptain::Render()
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

HRESULT CNPC_GuardCaptain::Set_State(GUARDCAPTAIN_STATE _eState)
{
	m_pStateManager->Set_State(_eState);

	return S_OK;
}

void CNPC_GuardCaptain::Set_WeaponSocket(const char* _strBoneName)
{
	dynamic_cast<CGuardCaptain_Weapon*>(m_vecNpcPart[PART_WEAPON])->
		Set_SoketBone(m_pModelCom->Get_BonePtr(_strBoneName));
}

void CNPC_GuardCaptain::Set_Idle()
{
	Play_Animation(true, "mt_idle");
}

HRESULT CNPC_GuardCaptain::Ready_Part()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	/* For. Weapon */
	CGuardCaptain_Weapon::WEAPON_DESC WeaponPartDesc;
	WeaponPartDesc.pParent = this;
	WeaponPartDesc.pParentTransform = m_pTransformCom;
	WeaponPartDesc.pSocketBone = m_pModelCom->Get_BonePtr("WeaponSword");
	WeaponPartDesc.matSocketPivot = m_pModelCom->Get_PivotMatrix();

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_GuardCaptain_Weapon"), &WeaponPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecNpcPart.push_back(pPart);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CNPC_GuardCaptain::Ready_Component(_uint _iLevel)
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

HRESULT CNPC_GuardCaptain::Ready_State()
{
	m_fRunSpeed = 2.5f;
	m_fWalkSpeed = 1.f;
	m_fHp = 500;
	m_iAtk = 40;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pStateManager = CStateManager_GuardCaptain::Create(this,
		pGameInstance->Find_CloneObject(g_curLevel, TEXT("Layer_Player"), TEXT("Player")),
		m_pTransformCom, m_pNavigationCom, m_pVecCollider);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CNPC_GuardCaptain::Ready_Talk()
{
	CSkyrimUI_Talk::TALK_DESC talkDesc;
	talkDesc.pOwner = this;
	talkDesc.m_iShowDialogIndex = 1;
	talkDesc.m_strOwnerName = TEXT("경비대장");
	talkDesc.m_mapNpcTalkData.insert(make_pair(0, vector<wstring>{
		{ L"멈춰라."},
		{ L"현재 이곳은 대피령이 내려져 모두 피난했다. 여기 온 이유가 뭐냐." },
		{ L"공고? 영주님께서 각지에 뿌린 공고를 말하는건가." },
		{ L"네 놈의 차림새를 보면 정신이 나갔던가 실력에 자신이 있다는건데." },
		{ L"아무리 실력에 자신이 있어도 그 차림으론 무리다." },
		{ L"시장쪽에서 장비를 보급받고 토벌을 시작하도록." },
		{ L"수로를 말끔하게 청소했다면 나에게 돌아와 보고해라."}, 
		{ L"물론 돌아올 거란 기대는 안하지만." },
	}));


	CSkyrimUI_Talk::PLAYERTALK_DESC playerTalkDesc;
	playerTalkDesc.m_iShowTextID = 1;
	playerTalkDesc.m_strPlayerTalk = { L"영주의 공고를 보고 왔다만." };
	talkDesc.m_vecPlayerTalkDesc.push_back(playerTalkDesc);
	playerTalkDesc.m_iShowTextID = 7;
	playerTalkDesc.m_strPlayerTalk = { L"네 놈.. 반드시 돌아오고야 말겠다." };
	talkDesc.m_vecPlayerTalkDesc.push_back(playerTalkDesc);

	CSkyrimUI_TalkIcon::TALKICON_DESC IconDesc;
	IconDesc.strTargetName = TEXT("경비대장");

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pTalk = dynamic_cast<CSkyrimUI_Talk*>(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_Talk"), &talkDesc));
	m_pTalkIcon = dynamic_cast<CSkyrimUI_TalkIcon*>(pGameInstance->Add_InstanceCloneObject(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("ProtoType_GameObject_TalkIcon"), &IconDesc));

	Safe_Release(pGameInstance);

	return S_OK;
}

CNPC_GuardCaptain* CNPC_GuardCaptain::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CNPC_GuardCaptain* pInstance = new CNPC_GuardCaptain(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CNPC_GuardCaptain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_GuardCaptain::Clone(void* _pArg)
{
	CNPC_GuardCaptain* pInstance = new CNPC_GuardCaptain(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_GuardCaptain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_GuardCaptain::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CNPC_GuardCaptain* pInstance = new CNPC_GuardCaptain(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CNPC_GuardCaptain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_GuardCaptain::Free()
{
	__super::Free();

	for (auto& iter : m_vecNpcPart)
	{
		if (iter != nullptr)
			Safe_Release(iter);
	}

	for (auto& iter : m_pVecCollider)
		Safe_Release(iter);

	Safe_Release(m_pStateManager);
}

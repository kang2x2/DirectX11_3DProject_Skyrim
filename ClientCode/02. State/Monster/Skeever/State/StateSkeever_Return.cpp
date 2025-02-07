#include "framework.h"
#include "StateSkeever_Return.h"

#include "GameInstance.h"

#include "Skeever.h"
#include "Player.h"

CStateSkeever_Return::CStateSkeever_Return()
{
}

HRESULT CStateSkeever_Return::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Return::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);
	m_pMonsterTransform->LookAt(m_pMonster->Get_OriginPos());

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CSkeever::SKEEVER_COL_DETECTION], dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB")))))
	{
		m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

		m_pMonster->Set_State(CSkeever::SKEEVER_CHASE);
		m_pMonster->Play_Animation(false, "runforward");
	}

	Safe_Release(pGameInstance);
}

void CStateSkeever_Return::Late_Update(_float _fTimeDelta)
{
	_float4 vPos = {};
	_float4 vOriginPos = {};

	XMStoreFloat4(&vPos, m_pMonsterTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&vOriginPos, m_pMonster->Get_OriginPos());

	if (vPos.x >= vOriginPos.x - 1.f && vPos.x <= vOriginPos.x + 1.f &&
		vPos.z >= vOriginPos.z - 2.f && vPos.z <= vOriginPos.z + 2.f)
	{
		m_pMonster->Set_State(CSkeever::SKEEVER_IDLE);
		m_pMonster->Play_Animation(true, "combatidle");
	}
}

CStateSkeever_Return* CStateSkeever_Return::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Return* pInstance = new CStateSkeever_Return();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Return::Free()
{
}

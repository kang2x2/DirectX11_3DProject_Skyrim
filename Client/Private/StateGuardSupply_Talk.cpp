#include "framework.h"
#include "StateGuardSupply_Talk.h"

#include "GameInstance.h"

#include "NPC_GuardSupply.h"
#include "Player.h"

CStateGuardSupply_Talk::CStateGuardSupply_Talk()
{
}

HRESULT CStateGuardSupply_Talk::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardSupply_Talk::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
}

void CStateGuardSupply_Talk::Late_Update(_float _fTimeDelta)
{
	if (m_pNpc->Get_CurFrameIndex() >= 160)
	{
		m_pNpc->Set_State(CNPC_GuardSupply::GUARDSUPPLY_IDLE);
		m_pNpc->Play_Animation(true, "mt_idle");
	}
}

CStateGuardSupply_Talk* CStateGuardSupply_Talk::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardSupply_Talk* pInstance = new CStateGuardSupply_Talk();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardSupply_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardSupply_Talk::Free()
{
}

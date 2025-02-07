#include "framework.h"
#include "StateGuardCaptain_Talk.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"
#include "Player.h"

CStateGuardCaptain_Talk::CStateGuardCaptain_Talk()
{
}

HRESULT CStateGuardCaptain_Talk::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardCaptain_Talk::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
}

void CStateGuardCaptain_Talk::Late_Update(_float _fTimeDelta)
{
	if (m_pNpc->Get_CurFrameIndex() >= 160)
	{
		m_pNpc->Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_IDLE);
		m_pNpc->Play_Animation(true, "mt_idle");
	}
}

CStateGuardCaptain_Talk* CStateGuardCaptain_Talk::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardCaptain_Talk* pInstance = new CStateGuardCaptain_Talk();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardCaptain_Talk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardCaptain_Talk::Free()
{
}

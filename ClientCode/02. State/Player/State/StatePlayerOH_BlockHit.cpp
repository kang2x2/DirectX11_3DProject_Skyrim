#include "framework.h"
#include "StatePlayerOH_BlockHit.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_BlockHit::CStatePlayerOH_BlockHit()
{
}

HRESULT CStatePlayerOH_BlockHit::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_BlockHit::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CurFrameIndex() < 20)
	{
		m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
	}

	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_BlockHit::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_blockhit"))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}

}

void CStatePlayerOH_BlockHit::Key_Input(_float _fTimeDelta)
{
	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_BlockHit* CStatePlayerOH_BlockHit::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_BlockHit* pInstance = new CStatePlayerOH_BlockHit();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_BlockHit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_BlockHit::Free()
{
	__super::Free();
}

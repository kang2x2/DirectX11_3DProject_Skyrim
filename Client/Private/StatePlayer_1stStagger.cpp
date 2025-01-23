#include "framework.h"
#include "StatePlayer_1stStagger.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_1stStagger::CStatePlayer_1stStagger()
{
}

HRESULT CStatePlayer_1stStagger::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayer_1stStagger::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);

	if (m_pPlayer->Get_CurFrameIndex() <= 60 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "staggercamera"))
	{
		m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
	}
}

void CStatePlayer_1stStagger::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "staggercamera"))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayer_1stStagger::Key_Input(_float _fTimeDelta)
{
	__super::Key_Input(_fTimeDelta);
}

CStatePlayer_1stStagger* CStatePlayer_1stStagger::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayer_1stStagger* pInstance = new CStatePlayer_1stStagger();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayer_1stStagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_1stStagger::Free()
{
	__super::Free();
}

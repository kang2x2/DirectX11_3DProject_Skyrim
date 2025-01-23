#include "framework.h"
#include "StatePlayer_3stStagger.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_3stStagger::CStatePlayer_3stStagger()
{
}

HRESULT CStatePlayer_3stStagger::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayer_3stStagger::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);

	if (m_pPlayer->Get_CurFrameIndex() <= 30 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_staggerbacklargest"))
	{
		m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
	}
}

void CStatePlayer_3stStagger::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_staggerbacklargest"))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayer_3stStagger::Key_Input(_float _fTimeDelta)
{
	__super::Key_Input(_fTimeDelta);
}

CStatePlayer_3stStagger* CStatePlayer_3stStagger::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayer_3stStagger* pInstance = new CStatePlayer_3stStagger();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayer_3stStagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_3stStagger::Free()
{
	__super::Free();
}
#include "StatePlayer_3stStagger.h"

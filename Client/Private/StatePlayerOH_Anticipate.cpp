#include "framework.h"
#include "StatePlayerOH_Anticipate.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_Anticipate::CStatePlayerOH_Anticipate()
{
}

HRESULT CStatePlayerOH_Anticipate::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_Anticipate::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_Anticipate::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_blockanticipate"))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayerOH_Anticipate::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_IsCounter())
	{
		if (pGameInstance->Get_DIKeyDown(VK_LBUTTON))
		{
			m_pPlayer->Set_AnimationSpeed(2.f);

			m_pPlayer->Set_State(CPlayer::ONEHAND_BASH);
			m_pPlayer->Play_Animation_All(false, "1hm_runfwdattackleft");
		}
	}

	Safe_Release(pGameInstance);

	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_Anticipate* CStatePlayerOH_Anticipate::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_Anticipate* pInstance = new CStatePlayerOH_Anticipate();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_Anticipate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_Anticipate::Free()
{
	__super::Free();
}

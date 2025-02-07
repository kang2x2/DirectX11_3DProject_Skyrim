#include "framework.h"
#include "StatePlayer_RunLeft.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_RunLeft::CStatePlayer_RunLeft()
{
}

HRESULT CStatePlayer_RunLeft::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayer_RunLeft::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);
}

void CStatePlayer_RunLeft::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayer_RunLeft::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 10 || m_pPlayer->Get_CurFrameIndex() == 20)
	{
		pGameInstance->PlaySoundFile(TEXT("fst_dirt_run_player_03.wav"), CHANNEL_PLAYER_RUN, 0.5f);
	}

	if (pGameInstance->Get_DIKeyPress('A'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
		
		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			m_pPlayerTransform->Go_Left(_fTimeDelta, m_pPlayerNavigation);

		if (pGameInstance->Get_DIKeyPress('W'))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
				m_pPlayer->Play_Animation_All(true, "hm_1stp_run");

			m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_F);
		}

		if (pGameInstance->Get_DIKeyPress('S'))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
				m_pPlayer->Play_Animation_All(true, "hm_1stp_run");
			else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
				m_pPlayer->Play_Animation_All(true, "mt_runbackward");

			m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_B);
		}
	}

	if (pGameInstance->Get_DIKeyUp('A'))
	{
		m_pPlayer->Set_State(CPlayer::UNEQUIP_IDLE);
		m_pPlayer->Play_Animation_All(true, "mt_idle");
	}

	__super::Key_Input(_fTimeDelta);

	Safe_Release(pGameInstance);
}

CStatePlayer_RunLeft* CStatePlayer_RunLeft::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayer_RunLeft* pInstance = new CStatePlayer_RunLeft();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayer_RunLeft");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_RunLeft::Free()
{
}

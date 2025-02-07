#include "framework.h"
#include "StatePlayer_RunBackward.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_RunBackward::CStatePlayer_RunBackward()
{
}

HRESULT CStatePlayer_RunBackward::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayer_RunBackward::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);
}

void CStatePlayer_RunBackward::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayer_RunBackward::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 10 || m_pPlayer->Get_CurFrameIndex() == 20)
	{
		pGameInstance->PlaySoundFile(TEXT("fst_dirt_run_player_03.wav"), CHANNEL_PLAYER_RUN, 0.5f);
	}

	if (pGameInstance->Get_DIKeyPress('S'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());
	
		if (pGameInstance->Get_DIKeyPress('A'))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			{
				Player_SetLook(45.f);
				m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
			}

			else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			{
				m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
				m_pPlayerTransform->Go_Left(_fTimeDelta, m_pPlayerNavigation);
			}

			if (pGameInstance->Get_DIKeyUp('S'))
			{
				Player_SetLook(135.f);

				m_pPlayer->Play_Animation_All(true, "mt_runforward");
				m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_L);
			}
		}

		else if (pGameInstance->Get_DIKeyPress('D'))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			{
				Player_SetLook(-45.f);
				m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
			}
			else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			{
				m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
				m_pPlayerTransform->Go_Right(_fTimeDelta, m_pPlayerNavigation);
			}

			if (pGameInstance->Get_DIKeyUp('S'))
			{
				Player_SetLook(-45.f);

				m_pPlayer->Play_Animation_All(true, "mt_runforward");
				m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_R);
			}

		}
		else
			m_pPlayerTransform->Go_Backward(_fTimeDelta, m_pPlayerNavigation);
	}

	if (pGameInstance->Get_DIKeyUp('S'))
	{
		m_pPlayer->Play_Animation_All(true, "mt_idle");
		m_pPlayer->Set_State(CPlayer::UNEQUIP_IDLE);
	}

	__super::Key_Input(_fTimeDelta);

	Safe_Release(pGameInstance);
}

CStatePlayer_RunBackward* CStatePlayer_RunBackward::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayer_RunBackward* pInstance = new CStatePlayer_RunBackward();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayer_RunBackward");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_RunBackward::Free()
{
}

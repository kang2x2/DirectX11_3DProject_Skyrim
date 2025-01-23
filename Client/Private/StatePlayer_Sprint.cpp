#include "framework.h"
#include "StatePlayer_Sprint.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayer_Sprint::CStatePlayer_Sprint()
{
}

HRESULT CStatePlayer_Sprint::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayer_Sprint::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 10 || m_pPlayer->Get_CurFrameIndex() == 20)
	{
		pGameInstance->PlaySoundFile(TEXT("fst_dirt_sprint_player_01.wav"), CHANNEL_PLAYER_RUN, 0.5f);
	}

	Safe_Release(pGameInstance);

	m_pPlayer->Set_ReadyRecoverySp(false);

	if (m_pPlayer->Get_PlayerSp() > 0.f)
		m_pPlayer->Set_PlayerSp(-0.3f);

	if (m_pPlayer->Get_PlayerSp() <= 0.f)
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			m_pPlayer->Play_Animation_All(true, "1hm_runforward");

		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayer->Play_Animation_All(true, "mt_runforward");

		m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_F);
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
	}

	Key_Input(_fTimeDelta);
}

void CStatePlayer_Sprint::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayer_Sprint::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyPress('W'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

		if (pGameInstance->Get_DIKeyUp(VK_LCONTROL))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
				m_pPlayer->Play_Animation_All(true, "1hm_runforward");

			else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
				m_pPlayer->Play_Animation_All(true, "mt_runforward");

			m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_F);
			m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
		}

		if (pGameInstance->Get_DIKeyPress('A'))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			{
				Player_SetLook(-45.f);
				m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
			}
			else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			{
				m_pPlayerTransform->Go_Left(_fTimeDelta, m_pPlayerNavigation);
				m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
			}

			if (pGameInstance->Get_DIKeyUp('W'))
			{
				if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
					Player_SetLook(-90.f);

				m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_L);
			}

		}

		else if (pGameInstance->Get_DIKeyPress('D'))
		{
			if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			{
				Player_SetLook(45.f);
				m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
			}
			else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			{
				m_pPlayerTransform->Go_Right(_fTimeDelta, m_pPlayerNavigation);
				m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
			}

			if (pGameInstance->Get_DIKeyUp('W'))
			{
				if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
					Player_SetLook(45.f);

				m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_R);
			}

		}

		else
			m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
	}

	if (pGameInstance->Get_DIKeyUp('W'))
	{
		m_pPlayer->Set_State(CPlayer::UNEQUIP_IDLE);
		m_pPlayer->Play_Animation_All(true, "mt_idle");
	}

	if (pGameInstance->Get_DIKeyUp(VK_LCONTROL))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			m_pPlayer->Play_Animation_All(true, "1hm_runforward");

		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayer->Play_Animation_All(true, "mt_runforward");

		m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_F);
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
	}

	__super::Key_Input(_fTimeDelta);

	Safe_Release(pGameInstance);
}

CStatePlayer_Sprint* CStatePlayer_Sprint::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayer_Sprint* pInstance = new CStatePlayer_Sprint();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayer_Sprint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_Sprint::Free()
{
}

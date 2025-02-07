#include "framework.h"
#include "StatePlayerOH_Sprint.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_Sprint::CStatePlayerOH_Sprint()
{
}

HRESULT CStatePlayerOH_Sprint::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_Sprint::Update(_float _fTimeDelta)
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
		m_pPlayer->Play_Animation_All(true, "1hm_runforward");

		m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_F);
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
	}

	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_Sprint::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayerOH_Sprint::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyPress('W'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

		if (pGameInstance->Get_DIKeyUp(VK_LCONTROL) &&
			m_pPlayer->Get_PlayerSp() > 0.f)
		{
			m_pPlayer->Play_Animation_All(true, "1hm_runforward");

			m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_F);
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
				m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
				m_pPlayerTransform->Go_Left(_fTimeDelta, m_pPlayerNavigation);
			}

			if (pGameInstance->Get_DIKeyUp('W'))
			{
				if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
					Player_SetLook(-90.f);

				m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_L);
			}

			/* 공격 */
			if (pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
			{
				m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

				m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKL);
				m_pPlayer->Play_Animation_All(false, "1hm_walkfwdattackleft");
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
				m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
				m_pPlayerTransform->Go_Right(_fTimeDelta, m_pPlayerNavigation);
			}

			if (pGameInstance->Get_DIKeyUp('W'))
			{
				if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
					Player_SetLook(45.f);

				m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_R);
			}

			/* 공격 */
			if (pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
			{
				m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

				m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKL);
				m_pPlayer->Play_Animation_All(false, "1hm_walkfwdattackleft");
			}
		}

		/* 공격 */
		else if (pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
		{
			m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

			m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKL);
			m_pPlayer->Play_Animation_All(false, "1hm_walkfwdattackleft");
		}

		else if (pGameInstance->Get_DIKeyDown('V') && m_pPlayer->Get_PlayerSp() > 0.f)
		{
			m_pPlayer->Set_IsAttack(true);

			m_pPlayer->Set_PlayerSp(-20.f);

			m_pPlayer->Set_State(CPlayer::ONEHAND_RUNPOWERATTACK);
			m_pPlayer->Play_Animation_All(false, "1hm_attackpowerforwardsprint");
		}

		else
			m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
	}

	if (pGameInstance->Get_DIKeyUp('W'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}

	if (pGameInstance->Get_DIKeyUp(VK_LCONTROL) || 
		!pGameInstance->Get_DIKeyPress('W'))
	{
		m_pPlayer->Play_Animation_All(true, "1hm_runforward");

		m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_F);
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());
	}


	Safe_Release(pGameInstance);

	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_Sprint* CStatePlayerOH_Sprint::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_Sprint* pInstance = new CStatePlayerOH_Sprint();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_Sprint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_Sprint::Free()
{
}

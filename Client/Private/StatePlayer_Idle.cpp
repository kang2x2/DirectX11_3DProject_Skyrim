#include "framework.h"
#include "StatePlayer_Idle.h"

#include "GameInstance.h"

#include "Player.h"


CStatePlayer_Idle::CStatePlayer_Idle()
{
}

HRESULT CStatePlayer_Idle::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayer_Idle::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);
}

void CStatePlayer_Idle::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayer_Idle::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* ÀÌµ¿ */
	if (pGameInstance->Get_DIKeyPress('W'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			m_pPlayer->Play_Animation_All(true, "1hm_runforward");

		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayer->Play_Animation_All(true, "mt_runforward");

		m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_F);
	}

	if (pGameInstance->Get_DIKeyPress('S'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			m_pPlayer->Play_Animation_All(true, "1hm_runbackward");

		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayer->Play_Animation_All(true, "mt_runbackward");

		m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_B);
	}

	if (pGameInstance->Get_DIKeyPress('A'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
		{
			m_pPlayer->Play_Animation_All(true, "1hm_runforward");
		}
		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		{
			Player_SetLook(-90.f);
			m_pPlayer->Play_Animation_All(true, "mt_runforward");
		}

		m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_L);
	}

	if (pGameInstance->Get_DIKeyPress('D'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
		{
			m_pPlayer->Play_Animation_All(true, "1hm_runforward");
		}
		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		{
			Player_SetLook(90.f);
			m_pPlayer->Play_Animation_All(true, "mt_runforward");
		}

		m_pPlayer->Set_State(CPlayer::UNEQUIP_RUN_R);
	}

	if (pGameInstance->Get_DIKeyDown(VK_LBUTTON))
	{
		pGameInstance->PlaySoundFile(TEXT("wpn_blade1hand_draw_01.wav"), CHANNEL_PLAYER, 1.f);

		m_pPlayer->Set_PlayerEquipState(CPlayer::EQUIP_ONEHAND);
		m_pPlayer->Set_State(CPlayer::ONEHAND_EQUIP);
		m_pPlayer->Play_Animation_All(false, "1hm_equip");
	}

	__super::Key_Input(_fTimeDelta);

	Safe_Release(pGameInstance);
}

CStatePlayer_Idle* CStatePlayer_Idle::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayer_Idle* pInstance = new CStatePlayer_Idle();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayer_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayer_Idle::Free()
{
}

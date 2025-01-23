#include "framework.h"
#include "StatePlayerOH_Idle.h"

#include "GameInstance.h"

#include "Player.h"


CStatePlayerOH_Idle::CStatePlayerOH_Idle()
{
}

HRESULT CStatePlayerOH_Idle::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_Idle::Update(_float _fTimeDelta)
{
	if(m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(m_pPlayer->Get_PlayerCamLook());

	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_Idle::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayerOH_Idle::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* ÀÌµ¿ */
	if (pGameInstance->Get_DIKeyPress('W'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_F);
		m_pPlayer->Play_Animation_All(true, "1hm_runforward");
	}

	if (pGameInstance->Get_DIKeyPress('S'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_B);
		m_pPlayer->Play_Animation_All(true, "1hm_runbackward");
	}

	if (pGameInstance->Get_DIKeyPress('A'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			Player_SetLook(-90.f);

		m_pPlayer->Play_Animation_All(true, "1hm_runforward");
		m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_L);
		
	}

	if (pGameInstance->Get_DIKeyPress('D'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			Player_SetLook(90.f);

		m_pPlayer->Play_Animation_All(true, "1hm_runforward");
		m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_R);

	}

	if (pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON) &&
		strcmp(m_pPlayer->Get_NextAnimationName().c_str(), "1hm_attackleft"))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_LATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackleft");
	}

	if (pGameInstance->Get_DIKeyDown('R'))
	{
		pGameInstance->PlaySoundFile(TEXT("wpn_blade1hand_sheathe.wav"), CHANNEL_PLAYER, 1.f);

		m_pPlayer->Set_PlayerEquipState(CPlayer::EQUIP_UNEQUIP);

		m_pPlayer->Set_State(CPlayer::ONEHAND_UNEQUIP);
		m_pPlayer->Play_Animation_All(false, "1hm_unequip");
	}

	if (pGameInstance->Get_DIKeyDown(VK_RBUTTON))
	{
		pGameInstance->PlaySoundFile(TEXT("npc_human_combat_shield_block_01.wav"), CHANNEL_PLAYER, 1.f);

		m_pPlayer->Set_IsReadyCounter(true);
		m_pPlayer->Set_State(CPlayer::ONEHAND_BLOCK);
		m_pPlayer->Play_Animation_All(true, "1hm_blockidle");
	}

	if (pGameInstance->Get_DIKeyDown('V') && m_pPlayer->Get_PlayerSp() > 0.f)
	{
		m_pPlayer->Set_PlayerSp(-20.f);

		m_pPlayer->Set_State(CPlayer::ONEHAND_PATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackpower");
	}

	__super::Key_Input(_fTimeDelta);

	Safe_Release(pGameInstance);
}

CStatePlayerOH_Idle* CStatePlayerOH_Idle::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_Idle* pInstance = new CStatePlayerOH_Idle();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_Idle::Free()
{
}

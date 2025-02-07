#include "framework.h"
#include "StatePlayerOH_RunRight.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_RunRight::CStatePlayerOH_RunRight()
{
}

HRESULT CStatePlayerOH_RunRight::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_RunRight::Update(_float _fTimeDelta)
{
	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_RunRight::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayerOH_RunRight::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 10 || m_pPlayer->Get_CurFrameIndex() == 20)
	{
		pGameInstance->PlaySoundFile(TEXT("fst_dirt_run_player_03.wav"), CHANNEL_PLAYER_RUN, 0.5f);
	}

	if (pGameInstance->Get_DIKeyPress('D'))
	{
		if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
			m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);
		
		else if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
			m_pPlayerTransform->Go_Right(_fTimeDelta, m_pPlayerNavigation);

		if (pGameInstance->Get_DIKeyPress('W'))
			m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_F);

		if (pGameInstance->Get_DIKeyPress('S'))
		{
			m_pPlayer->Play_Animation_All(true, "1hm_runbackward");
			m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_B);
		}

		/* АјАн */
		if (pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
		{
			m_pPlayerTransform->SetLook(m_pPlayer->Get_PlayerCamLook());
			m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());
		
			m_pPlayer->Set_State(CPlayer::ONEHAND_RWATTACKL);
			m_pPlayer->Play_Animation_All(false, "1hm_walkrightattackleft");
		}
	}

	if (pGameInstance->Get_DIKeyUp('D'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}

	Safe_Release(pGameInstance);

	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_RunRight* CStatePlayerOH_RunRight::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_RunRight* pInstance = new CStatePlayerOH_RunRight();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_RunRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_RunRight::Free()
{
}

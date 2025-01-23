#include "framework.h"
#include "StatePlayerOH_Bash.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_Bash::CStatePlayerOH_Bash()
{
}

HRESULT CStatePlayerOH_Bash::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_Bash::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(m_pPlayer->Get_PlayerCamLook());

	m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 12)
		pGameInstance->PlaySoundFile(TEXT("fx_swing_blade_medium_03.wav"), CHANNEL_PLAYER, 1.f);

	Safe_Release(pGameInstance);


	if (m_pPlayer->Get_CurFrameIndex() >= 12 && m_pPlayer->Get_CurFrameIndex() <= 20 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_runfwdattackleft"))
	{
		m_pPlayer->Set_IsAttack(true);
	}
	else
		m_pPlayer->Set_IsAttack(false);

	Key_Input(_fTimeDelta);

}

void CStatePlayerOH_Bash::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin())
	{
		m_pPlayer->Set_AnimationSpeed(1.5f);
		m_pPlayer->Set_IsAttack(false);

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayerOH_Bash::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 20 && 
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_runfwdattackleft") &&
		pGameInstance->Get_DIKeyDown(VK_LBUTTON))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_RATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackright");
	}

	Safe_Release(pGameInstance);


	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_Bash* CStatePlayerOH_Bash::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_Bash* pInstance = new CStatePlayerOH_Bash();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_Bash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_Bash::Free()
{
}

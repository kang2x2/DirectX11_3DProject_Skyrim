#include "framework.h"
#include "StatePlayerOH_LAttack.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_LAttack::CStatePlayerOH_LAttack()
{
}

HRESULT CStatePlayerOH_LAttack::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_LAttack::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
		m_pPlayer->CheckHit_Onehand(13, 15);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if(m_pPlayer->Get_CurFrameIndex() == 12)
		pGameInstance->PlaySoundFile(TEXT("fx_swing_blade_medium_03.wav"), CHANNEL_PLAYER, 1.f);

	Safe_Release(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 12 && m_pPlayer->Get_CurFrameIndex() <= 20 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackleft"))
	{
		m_pPlayer->Set_IsAttack(true);
	}
	else
		m_pPlayer->Set_IsAttack(false);

	Key_Input(_fTimeDelta);

	__super::Key_Input(_fTimeDelta);
}

void CStatePlayerOH_LAttack::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin())
	{
		m_pPlayer->Set_IsAttack(false);

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayerOH_LAttack::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 25 &&
		strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackright") &&
		pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_RATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackright");
	}

	else if (m_pPlayer->Get_CurFrameIndex() >= 25 && m_pPlayer->Get_PlayerSp() > 0.f &&
		strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackright") &&
		pGameInstance->Get_DIKeyDown('V'))
	{
		m_pPlayer->Set_PlayerSp(-20.f);

		m_pPlayer->Set_State(CPlayer::ONEHAND_PATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackpower");
	}

	else if (pGameInstance->Get_DIKeyPress('A'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_LWATTACKL);
		m_pPlayer->Play_Animation_All(false, "1hm_walkleftattackleft");
	}
	else if (pGameInstance->Get_DIKeyPress('D'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_RWATTACKL);
		m_pPlayer->Play_Animation_All(false, "1hm_walkrightattackleft", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('W'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKL);
		m_pPlayer->Play_Animation_All(false, "1hm_walkfwdattackleft", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('S'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_BWATTACKL);
		m_pPlayer->Play_Animation_All(false, "1hm_walkbwdattackleft", m_pPlayer->Get_CurFrameIndex());
	}

	Safe_Release(pGameInstance);

}

CStatePlayerOH_LAttack* CStatePlayerOH_LAttack::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_LAttack* pInstance = new CStatePlayerOH_LAttack();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_LAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_LAttack::Free()
{
}

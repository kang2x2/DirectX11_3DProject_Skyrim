#include "framework.h"
#include "StatePlayerOH_RAttack.h"

#include "GameInstance.h"

#include "Player.h"
#include "Player_Weapon.h"

CStatePlayerOH_RAttack::CStatePlayerOH_RAttack()
{
}

HRESULT CStatePlayerOH_RAttack::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_RAttack::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_1ST)
		m_pPlayer->CheckHit_Onehand(14, 16);
	
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 12)
		pGameInstance->PlaySoundFile(TEXT("fx_swing_blade_medium_03.wav"), CHANNEL_PLAYER, 1.f);

	Safe_Release(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 14 && m_pPlayer->Get_CurFrameIndex() <= 20 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackright"))
	{
		m_pPlayer->Set_IsAttack(true);
	}
	else
		m_pPlayer->Set_IsAttack(false);


	Key_Input(_fTimeDelta);

	__super::Key_Input(_fTimeDelta);
}

void CStatePlayerOH_RAttack::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin())
	{
		m_pPlayer->Set_IsAttack(false);

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayerOH_RAttack::Key_Input(_float _fTimeDelta)
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 22 &&
		strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackleft") &&
		pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_LATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackleft");
	}

	else if (m_pPlayer->Get_CurFrameIndex() >= 22 && m_pPlayer->Get_PlayerSp() > 0.f &&
		strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackleft") &&
		pGameInstance->Get_DIKeyDown('V'))
	{
		m_pPlayer->Set_PlayerSp(-20.f);

		m_pPlayer->Set_State(CPlayer::ONEHAND_PATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackpower");
	}

	else if (pGameInstance->Get_DIKeyPress('A'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_LWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkleftattackright", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('D'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_RWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkrtattackright", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('W'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkfwdattackright", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('S'))
	{
		m_pPlayerTransform->Set_Speed(m_pPlayer->GetWalkSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_BWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkbwdattackright", m_pPlayer->Get_CurFrameIndex());
	}

	Safe_Release(pGameInstance);

}

CStatePlayerOH_RAttack* CStatePlayerOH_RAttack::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_RAttack* pInstance = new CStatePlayerOH_RAttack();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_RAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_RAttack::Free()
{
}

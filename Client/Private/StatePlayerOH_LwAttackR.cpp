#include "framework.h"
#include "StatePlayerOH_LwAttackR.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_LwAttackR::CStatePlayerOH_LwAttackR()
{
}

HRESULT CStatePlayerOH_LwAttackR::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_LwAttackR::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 12)
		pGameInstance->PlaySoundFile(TEXT("fx_swing_blade_medium_03.wav"), CHANNEL_PLAYER, 1.f);

	Safe_Release(pGameInstance);


	if (m_pPlayer->Get_CurFrameIndex() >= 15 && m_pPlayer->Get_CurFrameIndex() <= 18 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_walkleftattackright"))
	{
		m_pPlayer->Set_IsAttack(true);
	}
	else
		m_pPlayer->Set_IsAttack(false);

	Key_Input(_fTimeDelta);

	__super::Key_Input(_fTimeDelta);
}

void CStatePlayerOH_LwAttackR::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() && 
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_walkleftattackright"))
	{
		m_pPlayer->Set_IsAttack(false);

		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");

		//if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		//	Player_SetLook(-90.f);
		//
		//m_pPlayer->Set_State(CPlayer::ONEHAND_RUN_L);
		//m_pPlayer->Play_Animation_All(true, "1hm_runforward");
	}
}

void CStatePlayerOH_LwAttackR::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 20 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_walkleftattackright") &&
		pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_LWATTACKL);
		m_pPlayer->Play_Animation_All(false, "1hm_walkleftattackleft");
	}

	else if (m_pPlayer->Get_CurFrameIndex() >= 20 && m_pPlayer->Get_PlayerSp() > 0.f &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_walkleftattackright") &&
		pGameInstance->Get_DIKeyDown('v'))
	{
		m_pPlayer->Set_PlayerSp(-20.f);

		m_pPlayer->Set_State(CPlayer::ONEHAND_PATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackpower");
	}


	if (pGameInstance->Get_DIKeyPress('A'))
	{
		m_pPlayerTransform->Go_Left(_fTimeDelta, m_pPlayerNavigation);
	}
	else if (pGameInstance->Get_DIKeyPress('D'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_RWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkrtattackright", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('W'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkfwdattackright", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('S'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_BWATTACKR);
		m_pPlayer->Play_Animation_All(false, "1hm_walkbwdattackright", m_pPlayer->Get_CurFrameIndex());
	}

	Safe_Release(pGameInstance);
}

CStatePlayerOH_LwAttackR* CStatePlayerOH_LwAttackR::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_LwAttackR* pInstance = new CStatePlayerOH_LwAttackR();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_LwAttackR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_LwAttackR::Free()
{
}

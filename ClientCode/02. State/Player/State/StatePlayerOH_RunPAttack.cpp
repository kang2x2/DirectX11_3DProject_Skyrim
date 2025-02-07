#include "framework.h"
#include "StatePlayerOH_RunPAttack.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_RunPAttack::CStatePlayerOH_RunPAttack()
{
}

HRESULT CStatePlayerOH_RunPAttack::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_RunPAttack::Update(_float _fTimeDelta)
{
	m_pPlayer->Set_ReadyRecoverySp(false);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 16)
		pGameInstance->PlaySoundFile(TEXT("fx_swing_blade_medium_03.wav"), CHANNEL_PLAYER, 1.f);

	Safe_Release(pGameInstance);

	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());
	
	m_pPlayerTransform->Go_Foward(_fTimeDelta, m_pPlayerNavigation);

	if (m_pPlayer->Get_CurFrameIndex() >= 15 && m_pPlayer->Get_CurFrameIndex() <= 22 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackpowerforwardsprint.001"))
	{
		m_pPlayer->Set_IsAttack(true);
	}
	else
		m_pPlayer->Set_IsAttack(false);

	__super::Key_Input(_fTimeDelta);
}

void CStatePlayerOH_RunPAttack::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsAnimationFin() && 
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackpowerforwardsprint"))
	{
		m_pPlayer->Set_IsAttack(false);

		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

CStatePlayerOH_RunPAttack* CStatePlayerOH_RunPAttack::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_RunPAttack* pInstance = new CStatePlayerOH_RunPAttack();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_RunPAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_RunPAttack::Free()
{
}

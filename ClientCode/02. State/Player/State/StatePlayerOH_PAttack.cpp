#include "framework.h"
#include "StatePlayerOH_PAttack.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_PAttack::CStatePlayerOH_PAttack()
{
}

HRESULT CStatePlayerOH_PAttack::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_PAttack::Update(_float _fTimeDelta)
{
	m_pPlayer->Set_ReadyRecoverySp(false);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() == 16)
		pGameInstance->PlaySoundFile(TEXT("fx_swing_blade_medium_03.wav"), CHANNEL_PLAYER, 1.f);

	Safe_Release(pGameInstance);


	if (m_pPlayer->Get_CurFrameIndex() >= 18 && m_pPlayer->Get_CurFrameIndex() <= 24 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackpower"))
	{
		m_pPlayer->Set_IsAttack(true);
	}
	else
		m_pPlayer->Set_IsAttack(false);


	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_PAttack::Late_Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

	if (m_pPlayer->Get_IsAnimationFin())
	{
		m_pPlayer->Set_IsAttack(false);

		m_pPlayerTransform->Set_Speed(m_pPlayer->GetRunSpeed());

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}
}

void CStatePlayerOH_PAttack::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pPlayer->Get_CurFrameIndex() >= 35 &&
		!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_attackpower") &&
		pGameInstance->Get_DIMouseDown(CInput_Device::MKS_LBUTTON))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_LATTACK);
		m_pPlayer->Play_Animation_All(false, "1hm_attackleft");
	}

	Safe_Release(pGameInstance);


	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_PAttack* CStatePlayerOH_PAttack::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_PAttack* pInstance = new CStatePlayerOH_PAttack();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_PAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_PAttack::Free()
{
}

#include "framework.h"
#include "StatePlayerOH_LwBlock.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_LwBlock::CStatePlayerOH_LwBlock()
{
}

HRESULT CStatePlayerOH_LwBlock::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_LwBlock::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_LwBlock::Late_Update()
{
	if (m_pPlayer->Get_IsAnimationFin() &&
	!strcmp(m_pPlayer->Get_CurAnimationName().c_str(), "1hm_equip"))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation(true, "1hm_idle");
	}
}

void CStatePlayerOH_LwBlock::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (pGameInstance->Get_DIKeyPress('A'))
	{
		m_pPlayerTransform->Go_Left(_fTimeDelta, m_pPlayerNavigation);
	}

	else if (pGameInstance->Get_DIKeyPress('D'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_RWBLOCK);
		m_pPlayer->Play_Animation(false, "1hm_walkleftattackleft", m_pPlayer->Get_CurFrameIndex());
	}

	else if (pGameInstance->Get_DIKeyPress('W'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_FWATTACKL);
		m_pPlayer->Play_Animation(false, "1hm_walkfwdattackleft", m_pPlayer->Get_CurFrameIndex());
	}
	else if (pGameInstance->Get_DIKeyPress('S'))
	{
		m_pPlayer->Set_State(CPlayer::ONEHAND_BWATTACKL);
		m_pPlayer->Play_Animation(false, "1hm_walkbwdattackleft", m_pPlayer->Get_CurFrameIndex());
	}


	Safe_Release(pGameInstance);

	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_LwBlock* CStatePlayerOH_LwBlock::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_LwBlock* pInstance = new CStatePlayerOH_LwBlock();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_LwBlock");
		Safe_Release(pInstance);

	}
	return pInstance;
}

void CStatePlayerOH_LwBlock::Free()
{
	__super::Free();
}

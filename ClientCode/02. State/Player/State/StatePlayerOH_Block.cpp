#include "framework.h"
#include "StatePlayerOH_Block.h"

#include "GameInstance.h"

#include "Player.h"

CStatePlayerOH_Block::CStatePlayerOH_Block()
{
}

HRESULT CStatePlayerOH_Block::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	__super::Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation);

	return S_OK;
}

void CStatePlayerOH_Block::Update(_float _fTimeDelta)
{
	if (m_pPlayer->Get_IsReadyCounter())
	{
		m_fCounterTime += _fTimeDelta;
	}

	if (m_pPlayer->Get_IsReadyCounter() && m_fCounterTime > 0.5f)
	{
		m_pPlayer->Set_IsReadyCounter(false);
		m_fCounterTime = 0.f;
	}

	if (m_pPlayer->Get_CamMode() == CPlayer::CAM_3ST)
		m_pPlayerTransform->SetLook(dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerCamLook());

	Key_Input(_fTimeDelta);
}

void CStatePlayerOH_Block::Late_Update(_float _fTimeDelta)
{
}

void CStatePlayerOH_Block::Key_Input(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyUp(VK_RBUTTON))
	{
		pGameInstance->PlaySoundFile(TEXT("npc_human_combat_shield_block_release_02.wav"), CHANNEL_PLAYER, 1.f);

		m_pPlayer->Set_IsReadyCounter(false);
		m_fCounterTime = 0.f;

		m_pPlayer->Set_State(CPlayer::ONEHAND_IDLE);
		m_pPlayer->Play_Animation_All(true, "1hm_idle");
	}

	if (pGameInstance->Get_DIKeyDown('Q'))
	{
		m_pPlayer->Create_Spark();
	}

	Safe_Release(pGameInstance);

	__super::Key_Input(_fTimeDelta);
}

CStatePlayerOH_Block* CStatePlayerOH_Block::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	CStatePlayerOH_Block* pInstance = new CStatePlayerOH_Block();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStatePlayerOH_Block");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatePlayerOH_Block::Free()
{
	__super::Free();
}

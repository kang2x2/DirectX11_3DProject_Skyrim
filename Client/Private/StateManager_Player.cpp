#include "framework.h"
#include "StateManager_Player.h"

#include "State_Player.h"

#include "StatePlayer_Idle.h"
#include "StatePlayer_RunFoward.h"
#include "StatePlayer_RunBackward.h"
#include "StatePlayer_RunLeft.h"
#include "StatePlayer_RunRight.h"
#include "StatePlayer_Sprint.h"

#include "StatePlayerOH_Idle.h"
#include "StatePlayerOH_Equip.h"
#include "StatePlayerOH_UnEquip.h"
#include "StatePlayerOH_RunFoward.h"
#include "StatePlayerOH_RunBackward.h"
#include "StatePlayerOH_RunLeft.h"
#include "StatePlayerOH_RunRight.h"
#include "StatePlayerOH_Sprint.h"

#include "StatePlayerOH_LAttack.h"
#include "StatePlayerOH_RAttack.h"
#include "StatePlayerOH_PAttack.h"

#include "StatePlayerOH_LwAttackL.h"
#include "StatePlayerOH_LwAttackR.h"

#include "StatePlayerOH_RwAttackL.h"
#include "StatePlayerOH_RwAttackR.h"

#include "StatePlayerOH_FwAttackL.h"
#include "StatePlayerOH_FwAttackR.h"

#include "StatePlayerOH_BwAttackL.h"
#include "StatePlayerOH_BwAttackR.h"

#include "StatePlayerOH_RunPAttack.h"

#include "StatePlayerOH_Bash.h"

#include "StatePlayerOH_Block.h"
#include "StatePlayerOH_Anticipate.h"
#include "StatePlayerOH_BlockHit.h"

#include "StatePlayer_1stStagger.h"
#include "StatePlayer_3stStagger.h"

CStateManager_Player::CStateManager_Player()
{
}

HRESULT CStateManager_Player::Initialize(CGameObject* _pPlayer, CTransform* _pPlayerTransform, CNavigation* _pPlayerNavigation)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

#pragma region EnEquip
	/* Idle */
	CState_Player* pState = CStatePlayer_Idle::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Run */
	pState = CStatePlayer_RunFoward::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayer_RunBackward::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayer_RunLeft::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayer_RunRight::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Sprint */
	pState = CStatePlayer_Sprint::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

#pragma endregion

#pragma region OneHand
	/* Idle */
	pState = CStatePlayerOH_Idle::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* equip */
	pState = CStatePlayerOH_Equip::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayerOH_UnEquip::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Run */
	pState = CStatePlayerOH_RunFoward::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayerOH_RunBackward::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayerOH_RunLeft::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayerOH_RunRight::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Sprint */
	pState = CStatePlayerOH_Sprint::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Attack */
	pState = CStatePlayerOH_LAttack::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayerOH_RAttack::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	pState = CStatePlayerOH_PAttack::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Move Attack */
	// L
	pState = CStatePlayerOH_LwAttackL::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayerOH_LwAttackR::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	// R
	pState = CStatePlayerOH_RwAttackL::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayerOH_RwAttackR::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	// F
	pState = CStatePlayerOH_FwAttackL::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayerOH_FwAttackR::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	// B
	pState = CStatePlayerOH_BwAttackL::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayerOH_BwAttackR::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	// F P
	pState = CStatePlayerOH_RunPAttack::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	// Bash
	pState = CStatePlayerOH_Bash::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

	/* Block */
	pState = CStatePlayerOH_Block::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayerOH_Anticipate::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayerOH_BlockHit::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);

#pragma endregion

#pragma region Stagger
	pState = CStatePlayer_1stStagger::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
	pState = CStatePlayer_3stStagger::Create(_pPlayer, _pPlayerTransform, _pPlayerNavigation);
	m_vecPlayerState.push_back(pState);
#pragma endregion

	
	m_pCurState = m_vecPlayerState.front();

	return S_OK;
}

CState_Player* CStateManager_Player::Get_State(CPlayer::PLAYERSTATE _eState)
{
	return m_vecPlayerState[_eState];
}

HRESULT CStateManager_Player::Set_State(CPlayer::PLAYERSTATE _eState)
{
	m_pCurState = m_vecPlayerState[_eState];

	return S_OK;
}

void CStateManager_Player::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_Player::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_Player* CStateManager_Player::Create(CGameObject* _pPlayer, CTransform* _pPlayerTransform, class CNavigation* _pPlayerNavigation)
{
	CStateManager_Player* pInstance = new CStateManager_Player();

	if (FAILED(pInstance->Initialize(_pPlayer, _pPlayerTransform, _pPlayerNavigation)))
	{
		MSG_BOX("Fail Create : CStateManager_Player");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_Player::Free()
{
	__super::Free();

	for (auto& iter : m_vecPlayerState)
	{
		Safe_Release(iter);
	}
}

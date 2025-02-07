#include "framework.h"
#include "StateManager_FalmerOH.h"

#include "State_FalmerOH.h"

#include "StateFalmerOH_Idle.h"

#include "StateFalmerOH_Equip.h"
#include "StateFalmerOH_UnEquip.h"

#include "StateFalmerOH_Warning.h"

#include "StateFalmerOH_Chase.h"

#include "StateFalmerOH_Detection.h"

#include "StateFalmerOH_Return.h"

#include "StateFalmerOH_Attack.h"
#include "StateFalmerOH_Attack2.h"
#include "StateFalmerOH_PAttack.h"
#include "StateFalmerOH_Charge.h"

#include "StateFalmerOH_StaggerL.h"
#include "StateFalmerOH_StaggerL2.h"
#include "StateFalmerOH_StaggerH.h"
#include "StateFalmerOH_StaggerH2.h"

#include "StateFalmerOH_Dead.h"

CStateManager_FalmerOH::CStateManager_FalmerOH()
{
}

HRESULT CStateManager_FalmerOH::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pMonster == nullptr)
		return E_FAIL;

	/* Idle */
	CState_FalmerOH* pState = CStateFalmerOH_Idle::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Equip */
	pState = CStateFalmerOH_Equip::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_UnEquip::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Warning */
	pState = CStateFalmerOH_Warning::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Detection*/
	pState = CStateFalmerOH_Detection::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Return */
	pState = CStateFalmerOH_Return::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Chase */
	pState = CStateFalmerOH_Chase::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Attack */
	pState = CStateFalmerOH_Attack::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_Attack2::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_PAttack::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_Charge::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Stagger */
	pState = CStateFalmerOH_StaggerL::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_StaggerL2::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_StaggerH::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	pState = CStateFalmerOH_StaggerH2::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Dead */
	pState = CStateFalmerOH_Dead::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	m_pCurState = m_vecMonsterState.front();

	return S_OK;

}

CState_FalmerOH* CStateManager_FalmerOH::Get_State(CFalmer_OneHand::FALMEROH_STATE _eState)
{
	return m_vecMonsterState[_eState];
}

HRESULT CStateManager_FalmerOH::Set_State(CFalmer_OneHand::FALMEROH_STATE _eState)
{
	m_pCurState = nullptr;
	m_pCurState = m_vecMonsterState[_eState];

	return S_OK;
}

void CStateManager_FalmerOH::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_FalmerOH::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_FalmerOH* CStateManager_FalmerOH::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateManager_FalmerOH* pInstance = new CStateManager_FalmerOH();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateManager_FalmerOH");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_FalmerOH::Free()
{
	__super::Free();

	for (auto& iter : m_vecMonsterState)
	{
		Safe_Release(iter);
	}
}


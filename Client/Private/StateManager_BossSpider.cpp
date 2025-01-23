#include "framework.h"
#include "StateManager_BossSpider.h"

#include "StateBossSpider_Idle.h"
#include "StateBossSpider_Fall.h"
#include "StateBossSpider_Planding.h"
#include "StateBossSpider_Warning.h"

#include "StateBossSpider_Chase.h"
#include "StateBossSpider_Left.h"
#include "StateBossSpider_Backward.h"
#include "StateBossSpider_Sprint.h"
#include "StateBossSpider_Turn.h"
#include "StateBossSpider_BerserkSprint.h"
#include "StateBossSpider_BerserkCharge.h"
#include "StateBossSpider_BerserkChop.h"

#include "StateBossSpider_OneChop.h"
#include "StateBossSpider_DoubleChop.h"
#include "StateBossSpider_Bite.h"
#include "StateBossSpider_Charge.h"

#include "StateBossSpider_StaggerOC.h"
#include "StateBossSpider_StaggerDC.h"
#include "StateBossSpider_StaggerB.h"
#include "StateBossSpider_StaggerC.h"

#include "StateBossSpider_GroggyIntro.h"
#include "StateBossSpider_Groggy.h"
#include "StateBossSpider_GroggyOut.h"

#include "StateBossSpider_Dead.h"
#include "StateBossSpider_Release.h"

CStateManager_BossSpider::CStateManager_BossSpider()
{
}

HRESULT CStateManager_BossSpider::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pMonster == nullptr)
		return E_FAIL;

	/* Idle */
	CState_BossSpider* pState = CStateBossSpider_Idle::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Fall */
	pState = CStateBossSpider_Fall::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Planding */
	pState = CStateBossSpider_Planding::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Warning */
	pState = CStateBossSpider_Warning::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Chase */
	pState = CStateBossSpider_Chase::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Left */
	pState = CStateBossSpider_Left::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Backward */
	pState = CStateBossSpider_Backward::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Sprint */
	pState = CStateBossSpider_Sprint::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Turn */
	pState = CStateBossSpider_Turn::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Berserk Sprint */
	pState = CStateBossSpider_BerserkSprint::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Berserk Charge */
	pState = CStateBossSpider_BerserkCharge::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Berserk Chop */
	pState = CStateBossSpider_BerserkChop::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* OneChop */
	pState = CStateBossSpider_OneChop::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* DoubleChop */
	pState = CStateBossSpider_DoubleChop::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Bite */
	pState = CStateBossSpider_Bite::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Charge */
	pState = CStateBossSpider_Charge::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Stagger_OneChop */
	pState = CStateBossSpider_StaggerOC::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Stagger_DoubleChop */
	pState = CStateBossSpider_StaggerDC::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Stagger_Bite */
	pState = CStateBossSpider_StaggerB::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Stagger_Charge */
	pState = CStateBossSpider_StaggerC::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	/* Groggy Intro */
	pState = CStateBossSpider_GroggyIntro::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Groggy */
	pState = CStateBossSpider_Groggy::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Groggy Out */
	pState = CStateBossSpider_GroggyOut::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);


	/* Dead */
	pState = CStateBossSpider_Dead::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);
	/* Release */
	pState = CStateBossSpider_Release::Create(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);
	m_vecMonsterState.push_back(pState);

	m_pCurState = m_vecMonsterState.front();

	return S_OK;
}

CState_BossSpider* CStateManager_BossSpider::Get_State(CBossSpider::BOSSSPIDER_STATE _eState)
{
	return m_vecMonsterState[_eState];
}

HRESULT CStateManager_BossSpider::Set_State(CBossSpider::BOSSSPIDER_STATE _eState)
{
	m_pCurState = nullptr;
	m_pCurState = m_vecMonsterState[_eState];

	return S_OK;
}

void CStateManager_BossSpider::Update(_float _fTimeDelta)
{
	m_pCurState->Update(_fTimeDelta);
}

void CStateManager_BossSpider::Late_Update(_float _fTimeDelta)
{
	m_pCurState->Late_Update(_fTimeDelta);
}

CStateManager_BossSpider* CStateManager_BossSpider::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateManager_BossSpider* pInstance = new CStateManager_BossSpider();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateManager_BossSpider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateManager_BossSpider::Free()
{
	__super::Free();

	for (auto& iter : m_vecMonsterState)
	{
		Safe_Release(iter);
	}
}

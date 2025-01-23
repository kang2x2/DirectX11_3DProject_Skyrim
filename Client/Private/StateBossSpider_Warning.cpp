#include "framework.h"
#include "StateBossSpider_Warning.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Warning::CStateBossSpider_Warning()
{
}

HRESULT CStateBossSpider_Warning::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Warning::Update(_float _fTimeDelta)
{
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "attack_bitehigh"))
	{
		m_pMonster->Set_AnimationSpeed(2.f);
		m_pMonsterTransform->Set_Speed(m_pMonster->Get_BossDesc().fSprintSpeed);

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_SPRINT);
		m_pMonster->Play_Animation(true, "forward_run");
	}
}

void CStateBossSpider_Warning::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Warning* CStateBossSpider_Warning::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Warning* pInstance = new CStateBossSpider_Warning();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Warning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Warning::Free()
{
}

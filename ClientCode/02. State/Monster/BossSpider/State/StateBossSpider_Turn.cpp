#include "framework.h"
#include "StateBossSpider_Turn.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Turn::CStateBossSpider_Turn()
{
}

HRESULT CStateBossSpider_Turn::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Turn::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_fLeftTime += _fTimeDelta;

	m_pMonsterTransform->Go_Left(_fTimeDelta, m_pMonsterNavigation);
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_fLeftTime > 6.f)
	{
		m_fLeftTime = 0.f;

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_BERSERKSPRINT);
		m_pMonster->Play_Animation(true, "forward_run");
	}
}

void CStateBossSpider_Turn::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Turn* CStateBossSpider_Turn::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Turn* pInstance = new CStateBossSpider_Turn();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Turn::Free()
{
}

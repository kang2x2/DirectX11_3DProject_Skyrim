#include "framework.h"
#include "StateBossSpider_Groggy.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Groggy::CStateBossSpider_Groggy()
{
}

HRESULT CStateBossSpider_Groggy::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Groggy::Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "bleedout_idle"))
	{
		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_AnimationSpeed(0.8f);

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_GROGGYOUT);
		m_pMonster->Play_Animation(false, "getup");
	}
}

void CStateBossSpider_Groggy::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Groggy* CStateBossSpider_Groggy::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Groggy* pInstance = new CStateBossSpider_Groggy();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Groggy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Groggy::Free()
{
}

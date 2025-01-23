#include "framework.h"
#include "StateBossSpider_GroggyIntro.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_GroggyIntro::CStateBossSpider_GroggyIntro()
{
}

HRESULT CStateBossSpider_GroggyIntro::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_GroggyIntro::Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "recoil_jump"))
	{
		m_pMonster->Set_AnimationSpeed(0.5f);
		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_GROGGY);
		m_pMonster->Play_Animation(false, "bleedout_idle");
	}
}

void CStateBossSpider_GroggyIntro::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_GroggyIntro* CStateBossSpider_GroggyIntro::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_GroggyIntro* pInstance = new CStateBossSpider_GroggyIntro();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_GroggyIntro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_GroggyIntro::Free()
{
}

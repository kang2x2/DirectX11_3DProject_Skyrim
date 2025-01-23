#include "framework.h"
#include "StateFalmerUE_SquatIntro.h"

#include "GameInstance.h"

#include "Falmer_UnEquip.h"

CStateFalmerUE_SquatIntro::CStateFalmerUE_SquatIntro()
{
}

HRESULT CStateFalmerUE_SquatIntro::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerUE_SquatIntro::Update(_float _fTimeDelta)
{
}

void CStateFalmerUE_SquatIntro::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "idlesquatintro"))
	{
		m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_SQUAT);
		m_pMonster->Play_Animation(true, "idlesquatloop");
	}
}

CStateFalmerUE_SquatIntro* CStateFalmerUE_SquatIntro::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerUE_SquatIntro* pInstance = new CStateFalmerUE_SquatIntro();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerUE_SquatIntro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerUE_SquatIntro::Free()
{
}


#include "framework.h"
#include "StateFalmerOH_UnEquip.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_UnEquip::CStateFalmerOH_UnEquip()
{
}

HRESULT CStateFalmerOH_UnEquip::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_UnEquip::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	if (m_pMonster->Get_CurFrameIndex() >= 55 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "1hmunequip"))
	{
		m_pMonster->Set_WeaponSocket("WeaponAxe");
	}
}

void CStateFalmerOH_UnEquip::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin())
	{
		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_RETURN);
		m_pMonster->Play_Animation(true, "mtwalkforward");
	}

}

CStateFalmerOH_UnEquip* CStateFalmerOH_UnEquip::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_UnEquip* pInstance = new CStateFalmerOH_UnEquip();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_UnEquip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_UnEquip::Free()
{
}

#include "framework.h"
#include "StateFalmerUE_Detection.h"

#include "GameInstance.h"

#include "Falmer_UnEquip.h"

CStateFalmerUE_Detection::CStateFalmerUE_Detection()
{
}

HRESULT CStateFalmerUE_Detection::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerUE_Detection::Update(_float _fTimeDelta)
{
}

void CStateFalmerUE_Detection::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "idledetection"))
	{
		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_State(CFalmer_UnEquip::FALMERUE_RETURN);
		m_pMonster->Play_Animation(true, "mtrunforward");
	}
}

CStateFalmerUE_Detection* CStateFalmerUE_Detection::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerUE_Detection* pInstance = new CStateFalmerUE_Detection();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerUE_Detection");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerUE_Detection::Free()
{
}


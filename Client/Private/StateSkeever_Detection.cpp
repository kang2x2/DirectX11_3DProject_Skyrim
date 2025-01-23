#include "framework.h"
#include "StateSkeever_Detection.h"

#include "GameInstance.h"

#include "Skeever.h"

CStateSkeever_Detection::CStateSkeever_Detection()
{
}

HRESULT CStateSkeever_Detection::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateSkeever_Detection::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CStateSkeever_Detection::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "idlecombat1"))
	{
		m_pMonsterTransform->Set_Speed(m_pMonster->GetWalkSpeed());

		m_pMonster->Set_State(CSkeever::SKEEVER_RETURN);
		m_pMonster->Play_Animation(true, "walkforward");
	}

}

CStateSkeever_Detection* CStateSkeever_Detection::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateSkeever_Detection* pInstance = new CStateSkeever_Detection();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateSkeever_Detection");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateSkeever_Detection::Free()
{
}

#include "framework.h"
#include "StateFalmerOH_Warning.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_Warning::CStateFalmerOH_Warning()
{
}

HRESULT CStateFalmerOH_Warning::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_Warning::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
}

void CStateFalmerOH_Warning::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "1hm_aggrowarning1"))
	{
		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_CHASE);
		m_pMonster->Play_Animation(true, "mtrunforward");
	}
}

CStateFalmerOH_Warning* CStateFalmerOH_Warning::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_Warning* pInstance = new CStateFalmerOH_Warning();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_Warning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_Warning::Free()
{
}


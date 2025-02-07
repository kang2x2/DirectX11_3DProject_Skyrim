#include "State_GuardSupply.h"

#include "GameInstance.h"

#include "NPC_GuardSupply.h"

CState_GuardSupply::CState_GuardSupply()
{
}

HRESULT CState_GuardSupply::Initialize(CGameObject* _pNpc, CGameObject* _pPlayer, CTransform* _pNpcTransform, CNavigation* _pNpcNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pNpc = dynamic_cast<CNPC_GuardSupply*>(_pNpc);

	if (FAILED(__super::Initialize(_pPlayer, _pNpcTransform, _pNpcNavigation, _pVecColCom)))
		return E_FAIL;

	return S_OK;
}

void CState_GuardSupply::Update(_float _fTimeDelta)
{
}

void CState_GuardSupply::Late_Update(_float _fTimeDelta)
{
}

void CState_GuardSupply::Free()
{
}

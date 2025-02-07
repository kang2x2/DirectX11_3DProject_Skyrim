#include "State_GuardCaptain.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"

CState_GuardCaptain::CState_GuardCaptain()
{
}

HRESULT CState_GuardCaptain::Initialize(CGameObject* _pNpc, CGameObject* _pPlayer, CTransform* _pNpcTransform, CNavigation* _pNpcNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pNpc = dynamic_cast<CNPC_GuardCaptain*>(_pNpc);

	if (FAILED(__super::Initialize(_pPlayer, _pNpcTransform, _pNpcNavigation, _pVecColCom)))
		return E_FAIL;

	return S_OK;
}

void CState_GuardCaptain::Update(_float _fTimeDelta)
{
}

void CState_GuardCaptain::Late_Update(_float _fTimeDelta)
{
}

void CState_GuardCaptain::Free()
{
}

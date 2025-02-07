#include "State_Npc.h"

#include "GameInstance.h"

#include "Player.h"

CState_Npc::CState_Npc()
{
}

HRESULT CState_Npc::Initialize(CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pNpcTransform = _pMonsterTransform;
	m_pNpcNavigation = _pMonsterNavigation;
	m_pVecCollider = _pVecColCom;

	m_pPlayer = dynamic_cast<CPlayer*>(_pPlayer);
	m_pPlayerTransform = dynamic_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));
	m_pPlayerBodyCollider = dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB")));

	return S_OK;
}

void CState_Npc::Update(_float _fTimeDelta)
{
}

void CState_Npc::Late_Update(_float _fTimeDelta)
{
}

void CState_Npc::Free()
{
}

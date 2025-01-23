#include "State_Monster.h"

#include "GameInstance.h"

#include "Player.h"

CState_Monster::CState_Monster()
{
}

HRESULT CState_Monster::Initialize(CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pMonsterTransform = _pMonsterTransform;
	m_pMonsterNavigation = _pMonsterNavigation;
	m_pVecCollider = _pVecColCom;

	m_pPlayer = dynamic_cast<CPlayer*>(_pPlayer);
	m_pPlayerTransform = dynamic_cast<CTransform*>(m_pPlayer->Get_Component(TEXT("Com_Transform")));
	m_pPlayerBodyCollider = dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB")));
	m_pPlayerWeaponCollider = dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_WEAPON)->Get_Component(TEXT("Com_Collider_OBB")));

	return S_OK;
}

void CState_Monster::Update(_float _fTimeDelta)
{
}

void CState_Monster::Late_Update(_float _fTimeDelta)
{
}

void CState_Monster::Free()
{
}

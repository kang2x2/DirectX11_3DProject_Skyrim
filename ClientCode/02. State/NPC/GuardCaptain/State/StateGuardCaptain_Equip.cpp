#include "framework.h"
#include "StateGuardCaptain_Equip.h"

#include "GameInstance.h"

#include "NPC_GuardCaptain.h"
#include "Player.h"

CStateGuardCaptain_Equip::CStateGuardCaptain_Equip()
{
}

HRESULT CStateGuardCaptain_Equip::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateGuardCaptain_Equip::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pNpcTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_pNpc->Get_CurFrameIndex() >= 11 &&
		!strcmp(m_pNpc->Get_CurAnimationName().c_str(), "1hm_equip"))
	{
		m_pNpc->Set_WeaponSocket("WEAPON");
	}
}

void CStateGuardCaptain_Equip::Late_Update(_float _fTimeDelta)
{
	if (m_pNpc->Get_IsAnimationFin())
	{
		m_pNpcTransform->Set_Speed(m_pNpc->GetWalkSpeed());

		m_pNpc->Set_State(CNPC_GuardCaptain::GUARDCAPTAIN_WALK);
		m_pNpc->Play_Animation(true, "1hm_walkforward");
	}
}

CStateGuardCaptain_Equip* CStateGuardCaptain_Equip::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateGuardCaptain_Equip* pInstance = new CStateGuardCaptain_Equip();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateGuardCaptain_Equip");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateGuardCaptain_Equip::Free()
{
}

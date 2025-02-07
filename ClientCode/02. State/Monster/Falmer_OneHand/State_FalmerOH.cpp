#include "State_FalmerOH.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"

CState_FalmerOH::CState_FalmerOH()
{
}

HRESULT CState_FalmerOH::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pMonster = dynamic_cast<CFalmer_OneHand*>(_pMonster);
	m_pWeaponCollider = dynamic_cast<CCollider*>(m_pMonster->Get_Part(CFalmer_OneHand::PART_WEAPON)->Get_Component(TEXT("Com_Collider_OBB")));

	if (FAILED(__super::Initialize(_pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
		return E_FAIL;

	return S_OK;
}

void CState_FalmerOH::Update(_float _fTimeDelta)
{
	if (m_pMonster->GetHp() <= 0.f)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("npc_falmer_death_03.wav"), CHANNEL_MONSTER2, 1.f);

		Safe_Release(pGameInstance);

		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_DEAD);
		m_pMonster->Play_Animation(false, "bleedoutintro");
	}
}

void CState_FalmerOH::Late_Update(_float _fTimeDelta)
{
}

void CState_FalmerOH::Free()
{
}

#include "framework.h"
#include "State_Spider.h"

#include "GameInstance.h"

#include "Spider.h"

CState_Spider::CState_Spider()
{
}

HRESULT CState_Spider::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pMonster = dynamic_cast<CSpider*>(_pMonster);
	m_pMouthCollider = dynamic_cast<CCollider*>(m_pMonster->Get_Part(CSpider::PART_MOUTH)->Get_Component(TEXT("Com_Collider_OBB")));

	if (FAILED(__super::Initialize(_pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
		return E_FAIL;

	return S_OK;
}

void CState_Spider::Update(_float _fTimeDelta)
{
	m_fSpitCoolTime += _fTimeDelta;

	if (m_fSpitCoolTime > 15.f)
	{
		m_pMonster->Set_State(CSpider::SPIDER_BACKWARD);
		m_pMonster->Play_Animation(true, "backward_run");
		m_fSpitCoolTime = 25.f - m_fSpitCoolTime;
	}

	if (m_pMonster->GetHp() <= 0.f)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_death_02.wav"), CHANNEL_MONSTER4, 1.f);

		Safe_Release(pGameInstance);

		m_pMonster->Set_State(CSpider::SPIDER_DEAD);
		m_pMonster->Play_Animation(false, "death");
	}
}

void CState_Spider::Late_Update(_float _fTimeDelta)
{
}

_bool CState_Spider::State_Waiting(_float _fWaitingTime, _bool _bIsLookAt, _float _fTimeDelta)
{
	if (_bIsLookAt)
		m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_fWaitingTime += _fTimeDelta;
	if (m_fWaitingTime >= _fWaitingTime)
	{
		m_fWaitingTime = _fWaitingTime - m_fWaitingTime;
		return true;
	}

	return false;
}

void CState_Spider::Free()
{
}

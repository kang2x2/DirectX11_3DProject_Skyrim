#include "framework.h"
#include "State_BossSpider.h"

#include "GameInstance.h"
#include "BossSpider.h"

CState_BossSpider::CState_BossSpider()
{
}

HRESULT CState_BossSpider::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	if (_pPlayer == nullptr)
		return E_FAIL;

	m_pMonster = dynamic_cast<CBossSpider*>(_pMonster);
	m_pMouthCollider = dynamic_cast<CCollider*>(m_pMonster->Get_Part(CBossSpider::PART_MOUTH)->Get_Component(TEXT("Com_Collider_OBB")));
	m_pLeftCollider = dynamic_cast<CCollider*>(m_pMonster->Get_Part(CBossSpider::PART_LEFT)->Get_Component(TEXT("Com_Collider_OBB")));
	m_pRightCollider = dynamic_cast<CCollider*>(m_pMonster->Get_Part(CBossSpider::PART_RIGHT)->Get_Component(TEXT("Com_Collider_OBB")));

	if (FAILED(__super::Initialize(_pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
		return E_FAIL;

	return S_OK;
}

void CState_BossSpider::Update(_float _fTimeDelta)
{
	if (m_pMonster->GetHp() <= 0.f)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_death_01.wav"), CHANNEL_MONSTER4, 1.f);
		pGameInstance->StopSound(CHANNEL_BGM);

		Safe_Release(pGameInstance);

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_DEAD);
		m_pMonster->Play_Animation(false, "death");
	}

}

void CState_BossSpider::Late_Update(_float _fTimeDelta)
{
}

_bool CState_BossSpider::State_Waiting(_float _fWaitingTime, _bool _bIsLookAt, _float _fTimeDelta)
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

void CState_BossSpider::Free()
{
}

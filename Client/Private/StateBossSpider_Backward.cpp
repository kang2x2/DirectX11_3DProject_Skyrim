#include "framework.h"
#include "StateBossSpider_Backward.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Backward::CStateBossSpider_Backward()
{
}

HRESULT CStateBossSpider_Backward::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Backward::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_fBackwardTime += _fTimeDelta;

	m_pMonsterTransform->Go_Backward(_fTimeDelta, m_pMonsterNavigation);
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	if (m_fBackwardTime >= 0.75f)
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_breathe_lp.wav"), CHANNEL_MONSTER1_RUN, 0.5f);
		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_TURN);
		m_pMonster->Play_Animation(true, "left");

		m_fBackwardTime = 0.f;

		Safe_Release(pGameInstance);
	}

}

void CStateBossSpider_Backward::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Backward* CStateBossSpider_Backward::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Backward* pInstance = new CStateBossSpider_Backward();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Backward");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Backward::Free()
{
}

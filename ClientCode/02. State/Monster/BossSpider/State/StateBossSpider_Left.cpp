#include "framework.h"
#include "StateBossSpider_Left.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Left::CStateBossSpider_Left()
{
}

HRESULT CStateBossSpider_Left::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Left::Update(_float _fTimeDelta)
{
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	__super::Update(_fTimeDelta);
	// m_fBackTime 상태 전환할 때 0으로 초기화
	if (m_fBackTime < 0.5f)
	{
		m_pMonsterTransform->Go_Backward(_fTimeDelta, m_pMonsterNavigation);
		m_fBackTime += _fTimeDelta;
	}
	else if (m_fBackTime >= 0.5f)
	{
		m_fLeftTime += _fTimeDelta;

		if (m_fLeftTime <= 1.5f)
		{
			m_pMonsterTransform->Go_Left(_fTimeDelta, m_pMonsterNavigation);
		}
		else
		{
			m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);
		}

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Collision_Enter(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_ATKROUND], m_pPlayerBodyCollider))
		{
			m_fLeftTime = 0.f;
			m_fBackTime = 0.f;

			pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_attack_b_01.wav"), CHANNEL_MONSTER1, 0.35f);

			m_pMonster->Set_State(CBossSpider::BOSSSPIDER_DOUBLECHOP);
			m_pMonster->Play_Animation(false, "attack_combochop");
		}

		Safe_Release(pGameInstance);
	}
}

void CStateBossSpider_Left::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Left* CStateBossSpider_Left::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Left* pInstance = new CStateBossSpider_Left();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Left");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Left::Free()
{
}

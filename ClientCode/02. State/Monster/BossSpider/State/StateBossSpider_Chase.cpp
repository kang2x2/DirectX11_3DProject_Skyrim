#include "framework.h"
#include "StateBossSpider_Chase.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Chase::CStateBossSpider_Chase()
{
}

HRESULT CStateBossSpider_Chase::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Chase::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* RunPowerAtk 범위에 들어왔을 때 */
	if (pGameInstance->Collision_Enter(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_ATKROUND], m_pPlayerBodyCollider))
	{
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_attack_b_01.wav"), CHANNEL_MONSTER1, 0.35f);
		
		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_ONECHOP);
		m_pMonster->Play_Animation(false, "attack_rightchop");
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_Chase::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Chase* CStateBossSpider_Chase::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Chase* pInstance = new CStateBossSpider_Chase();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Chase::Free()
{
}

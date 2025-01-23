#include "framework.h"
#include "StateBossSpider_BerserkSprint.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_BerserkSprint::CStateBossSpider_BerserkSprint()
{
}

HRESULT CStateBossSpider_BerserkSprint::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_BerserkSprint::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);
	m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_CurFrameIndex() == 10 || m_pMonster->Get_CurFrameIndex() == 20 ||
		m_pMonster->Get_CurFrameIndex() == 30)
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_breathe_lp.wav"), CHANNEL_MONSTER1, 0.35f);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_CHARGEROUND], m_pPlayerBodyCollider)
		|| pGameInstance->Collision_Stay(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_CHARGEROUND], m_pPlayerBodyCollider))
	{
		m_pMonster->Set_AnimationSpeed(1.f);

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_BERSERKCHARGE);
		m_pMonster->Play_Animation(false, "attack_forwardjump");
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_BerserkSprint::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_BerserkSprint* CStateBossSpider_BerserkSprint::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_BerserkSprint* pInstance = new CStateBossSpider_BerserkSprint();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_BerserkSprint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_BerserkSprint::Free()
{
}

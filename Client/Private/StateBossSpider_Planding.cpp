#include "framework.h"
#include "StateBossSpider_Planding.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Planding::CStateBossSpider_Planding()
{
}

HRESULT CStateBossSpider_Planding::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Planding::Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "fspidertraplanding"))
	{
		m_pMonster->Set_BarShow();

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlayBGM(TEXT("BossBattle.mp3"), 0.3f);
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbitegiant_breathe_lp.wav"), CHANNEL_MONSTER1, 1.f);

		Safe_Release(pGameInstance);

		m_pMonster->Set_AnimationSpeed(0.6f);
		m_pMonsterTransform->Set_Speed(m_pMonster->GetRunSpeed());

		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_WARNING);
		m_pMonster->Set_Active();
		m_pMonster->Play_Animation(false, "attack_bitehigh");
	}
}

void CStateBossSpider_Planding::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Planding* CStateBossSpider_Planding::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Planding* pInstance = new CStateBossSpider_Planding();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Planding");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Planding::Free()
{
}

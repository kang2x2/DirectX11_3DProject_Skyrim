#include "framework.h"
#include "StateBossSpider_Idle.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Idle::CStateBossSpider_Idle()
{
}

HRESULT CStateBossSpider_Idle::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Idle::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	_float4 vIdlePos;
	XMStoreFloat4(&vIdlePos, m_pMonsterTransform->Get_State(CTransform::STATE_POSITION));
	vIdlePos.y = -4.5f;
	m_pMonsterTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vIdlePos));

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CBossSpider::BOSSSPIDER_COL_DETECTION], m_pPlayerBodyCollider))
	{
		pGameInstance->PlaySoundFile(TEXT("npc_spiderfrostbite_holeexit.wav"), CHANNEL_MONSTER1, 1.f);
		
		m_pMonster->Play_Animation(true, "fspidertrapfall");
		m_pMonster->Set_State(CBossSpider::BOSSSPIDER_FALL);
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_Idle::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Idle* CStateBossSpider_Idle::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Idle* pInstance = new CStateBossSpider_Idle();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Idle::Free()
{
}

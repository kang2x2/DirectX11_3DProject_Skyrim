#include "framework.h"
#include "StateBossSpider_Release.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Release::CStateBossSpider_Release()
{
}

HRESULT CStateBossSpider_Release::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateBossSpider_Release::Update(_float _fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (m_pMonster->Get_CurFrameIndex() > 10 &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "bleedout_idle"))
	{
		pGameInstance->StopSoundAll();
		m_pMonster->Set_IsDissloving(true);
	}

	Safe_Release(pGameInstance);
}

void CStateBossSpider_Release::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Release* CStateBossSpider_Release::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Release* pInstance = new CStateBossSpider_Release();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Release");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Release::Free()
{
}

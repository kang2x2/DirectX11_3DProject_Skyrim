#include "framework.h"
#include "StateFalmerOH_Detection.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"

CStateFalmerOH_Detection::CStateFalmerOH_Detection()
{
}

HRESULT CStateFalmerOH_Detection::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_Detection::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);
}

void CStateFalmerOH_Detection::Late_Update(_float _fTimeDelta)
{
	if (m_pMonster->Get_IsAnimationFin() &&
		!strcmp(m_pMonster->Get_CurAnimationName().c_str(), "idledetection"))
	{
		m_pMonsterTransform->Set_Speed(m_pMonster->GetWalkSpeed());
		
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		pGameInstance->PlaySoundFile(TEXT("wpn_axe1hand_sheathe.wav"), CHANNEL_MONSTER2, 1.f);
		
		Safe_Release(pGameInstance);

		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_UNEQUIP);
		m_pMonster->Play_Animation(false, "1hmunequip");
	}
}

CStateFalmerOH_Detection* CStateFalmerOH_Detection::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_Detection* pInstance = new CStateFalmerOH_Detection();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_Detection");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_Detection::Free()
{
}


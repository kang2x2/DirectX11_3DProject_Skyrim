#include "framework.h"
#include "StateFalmerOH_Return.h"

#include "GameInstance.h"

#include "Falmer_OneHand.h"
#include "Player.h"

CStateFalmerOH_Return::CStateFalmerOH_Return()
{
}

HRESULT CStateFalmerOH_Return::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	return S_OK;
}

void CStateFalmerOH_Return::Update(_float _fTimeDelta)
{
	__super::Update(_fTimeDelta);

	m_pMonsterTransform->Go_Foward(_fTimeDelta, m_pMonsterNavigation);
	// m_pMonsterTransform->Chase(dynamic_cast<CMonster*>(m_pMonster)->Get_OriginPos(), _fTimeDelta, 1.5f);
	m_pMonsterTransform->LookAt(m_pMonster->Get_OriginPos());

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Collision_Enter(m_pVecCollider[CFalmer_OneHand::FALMEROH_COL_DETECTION], dynamic_cast<CCollider*>(m_pPlayer->Get_Part(CPlayer::PART_BODY)->Get_Component(TEXT("Com_Collider_AABB")))))
	{
		m_pMonsterTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_EQUIP);
		m_pMonster->Play_Animation(false, "1hmequip");
	}

	Safe_Release(pGameInstance);

}

void CStateFalmerOH_Return::Late_Update(_float _fTimeDelta)
{
	_float4 vPos = {};
	_float4 vOriginPos = {};

	XMStoreFloat4(&vPos, m_pMonsterTransform->Get_State(CTransform::STATE_POSITION));
	XMStoreFloat4(&vOriginPos, m_pMonster->Get_OriginPos());

	if (vPos.x >= vOriginPos.x - 1.f && vPos.x <= vOriginPos.x + 1.f &&
		vPos.z >= vOriginPos.z - 2.f && vPos.z <= vOriginPos.z + 2.f)
	{
		m_pMonster->Set_State(CFalmer_OneHand::FALMEROH_IDLE);
		m_pMonster->Play_Animation(false, "idlescratch1");
	}
}

CStateFalmerOH_Return* CStateFalmerOH_Return::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateFalmerOH_Return* pInstance = new CStateFalmerOH_Return();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateFalmerOH_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateFalmerOH_Return::Free()
{
}


#include "framework.h"
#include "StateBossSpider_Fall.h"

#include "GameInstance.h"

#include "BossSpider.h"
#include "Player.h"

CStateBossSpider_Fall::CStateBossSpider_Fall()
{
}

HRESULT CStateBossSpider_Fall::Initialize(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	__super::Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom);

	m_fDropSpeed = 25.f;

	return S_OK;
}

void CStateBossSpider_Fall::Update(_float _fTimeDelta)
{
	_float4 vIdlePos;
	XMStoreFloat4(&vIdlePos, m_pMonsterTransform->Get_State(CTransform::STATE_POSITION));
	
	if (!m_bIsFall && vIdlePos.y >= -12.6f)
	{
		m_fDropSpeed -= 20.f * _fTimeDelta;
		vIdlePos.y -= m_fDropSpeed * _fTimeDelta;
		m_pMonsterTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vIdlePos));
	}
	else
	{
		m_bIsFall = true;
	}

	if (m_bIsFall)
	{
		m_fDropIdleTime += _fTimeDelta;
		if (m_fDropIdleTime >= 0.2f)
		{
			m_pMonster->Play_Animation(false, "fspidertraplanding");
			m_pMonster->Set_State(CBossSpider::BOSSSPIDER_PLANDING);
		}
	}

	m_pMonsterTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vIdlePos));
}

void CStateBossSpider_Fall::Late_Update(_float _fTimeDelta)
{
}

CStateBossSpider_Fall* CStateBossSpider_Fall::Create(CGameObject* _pMonster, CGameObject* _pPlayer, CTransform* _pMonsterTransform, CNavigation* _pMonsterNavigation, vector<CCollider*> _pVecColCom)
{
	CStateBossSpider_Fall* pInstance = new CStateBossSpider_Fall();

	if (FAILED(pInstance->Initialize(_pMonster, _pPlayer, _pMonsterTransform, _pMonsterNavigation, _pVecColCom)))
	{
		MSG_BOX("Fail Create : CStateBossSpider_Fall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateBossSpider_Fall::Free()
{
}

#include "framework.h"
#include "FalmerFence.h"

#include "GameInstance.h"

CFalmerFence::CFalmerFence(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CFalmerFence::CFalmerFence(const CFalmerFence& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CFalmerFence::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CFalmerFence::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CFalmerFence::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("FalmerFence");

	return S_OK;

}

void CFalmerFence::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CFalmerFence::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CFalmerFence::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CFalmerFence::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	/* 추후 aabb 추가. */

	return S_OK;
}

CFalmerFence* CFalmerFence::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CFalmerFence* pInstance = new CFalmerFence(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CFalmerFence");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFalmerFence::Clone(void* _pArg)
{
	CFalmerFence* pInstance = new CFalmerFence(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CFalmerFence");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFalmerFence::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CFalmerFence* pInstance = new CFalmerFence(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CFalmerFence");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFalmerFence::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}

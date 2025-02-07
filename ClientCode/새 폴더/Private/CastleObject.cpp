#include "framework.h"
#include "CastleObject.h"

#include "GameInstance.h"

CCastleObject::CCastleObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CPlaceableObject(_pDevice, _pContext)
{
}

CCastleObject::CCastleObject(const CCastleObject& rhs)
	: CPlaceableObject(rhs)
{
}

HRESULT CCastleObject::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCastleObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CCastleObject::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	_matrix* pMatPivot = (_matrix*)pArg;

	m_strModelComTag = _strModelComTag;

	if (FAILED(Ready_Component(_iLevel)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*pMatPivot);
	// 메시의 정점 정보도 같이 업데이트 해준다.
	m_pModelCom->Update_VI(*pMatPivot);

	m_bHasMesh = true;
	m_strName = TEXT("CastleObject");

	return S_OK;
}

void CCastleObject::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CCastleObject::LateTick(_float _fTimeDelta)
{
	__super::LateTick(_fTimeDelta);
}

HRESULT CCastleObject::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCastleObject::Ready_Component(_uint _iLevel)
{
	__super::Ready_Component(_iLevel);

	return S_OK;
}

CCastleObject* CCastleObject::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCastleObject* pInstance = new CCastleObject(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CCastleObject ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCastleObject::Clone(void* _pArg)
{
	CCastleObject* pInstance = new CCastleObject(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CCastleObject Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCastleObject::Clone(_uint _iLevel, const wstring& _strModelComTag, void* _pArg)
{
	CCastleObject* pInstance = new CCastleObject(*this);

	if (FAILED(pInstance->Initialize_Clone(_iLevel, _strModelComTag, _pArg)))
	{
		MSG_BOX("Fail Clone : CCastleObject Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCastleObject::Free()
{
	__super::Free();
}

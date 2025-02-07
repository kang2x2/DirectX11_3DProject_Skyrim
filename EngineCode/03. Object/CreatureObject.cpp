#include "CreatureObject.h"

CCreatureObject::CCreatureObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CCreatureObject::CCreatureObject(const CCreatureObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCreatureObject::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCreatureObject::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CCreatureObject::Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg)
{
	return S_OK;
}

void CCreatureObject::PriorityTick(_float _fTimeDelta)
{
}

void CCreatureObject::Tick(_float _fTimeDelta)
{
}

void CCreatureObject::LateTick(_float _fTimeDelta)
{
}

HRESULT CCreatureObject::Render()
{
	return S_OK;
}

CGameObject* CCreatureObject::Clone(void* _pArg)
{
	return nullptr;
}

void CCreatureObject::Free()
{
	__super::Free();


}

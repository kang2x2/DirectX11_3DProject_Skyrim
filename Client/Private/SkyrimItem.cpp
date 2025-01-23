#include "framework.h"
#include "SkyrimItem.h"

CSkyrimItem::CSkyrimItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CSkyrimItem::CSkyrimItem(const CSkyrimItem& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyrimItem::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimItem::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

void CSkyrimItem::Tick(_float _fTimeDelta)
{
}

void CSkyrimItem::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkyrimItem::Render()
{
	return S_OK;
}

void CSkyrimItem::Free()
{
	__super::Free();
}

#include "framework.h"
#include "SkyrimHelmet.h"

CSkyrimHelmet::CSkyrimHelmet(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CSkyrimHelmet::CSkyrimHelmet(const CSkyrimHelmet& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyrimHelmet::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimHelmet::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

void CSkyrimHelmet::Tick(_float _fTimeDelta)
{
}

void CSkyrimHelmet::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkyrimHelmet::Render()
{
	return S_OK;
}

void CSkyrimHelmet::Free()
{
	__super::Free();
}

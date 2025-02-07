#include "framework.h"
#include "SkyrimClothes.h"

CSkyrimClothes::CSkyrimClothes(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
{
}

CSkyrimClothes::CSkyrimClothes(const CSkyrimClothes& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyrimClothes::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimClothes::Initialize_Clone(void* _pArg)
{
	return S_OK;
}

void CSkyrimClothes::Tick(_float _fTimeDelta)
{
}

void CSkyrimClothes::LateTick(_float _fTimeDelta)
{
}

HRESULT CSkyrimClothes::Render()
{
	return S_OK;
}

void CSkyrimClothes::Free()
{
	__super::Free();
}

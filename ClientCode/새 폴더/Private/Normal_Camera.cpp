#include "framework.h"
#include "Normal_Camera.h"

#include "GameInstance.h"

CNormal_Camera::CNormal_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCamera(_pDevice, _pContext)
{
}

CNormal_Camera::CNormal_Camera(const CNormal_Camera& rhs)
	: CCamera(rhs)
{
}

HRESULT CNormal_Camera::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CNormal_Camera::Initialize_Clone(void* pArg)
{
	return S_OK;
}

void CNormal_Camera::Tick(_float _fTimeDelta)
{
}

void CNormal_Camera::LateTick(_float _fTimeDelta)
{
}

HRESULT CNormal_Camera::Ready_Components()
{
	return E_NOTIMPL;
}

CNormal_Camera* CNormal_Camera::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return nullptr;
}

CCamera* CNormal_Camera::Clone(void* pArg)
{
	return nullptr;
}

void CNormal_Camera::Free()
{
}

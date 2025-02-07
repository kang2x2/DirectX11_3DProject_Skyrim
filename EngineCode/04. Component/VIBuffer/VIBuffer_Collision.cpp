
#include "VIBuffer_Collision.h"

CVIBuffer_Collision::CVIBuffer_Collision(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CVIBuffer(_pDevice, _pContext)
{
}

CVIBuffer_Collision::CVIBuffer_Collision(const CVIBuffer_Collision& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Collision::Initialize_ProtoType(const _float3* _Points)
{
	return E_NOTIMPL;
}

HRESULT CVIBuffer_Collision::Initialize_Clone(void* pArg)
{
	return E_NOTIMPL;
}

CVIBuffer_Collision* CVIBuffer_Collision::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return nullptr;
}

CComponent* CVIBuffer_Collision::Clone(void* _pArg)
{
	return nullptr;
}

void CVIBuffer_Collision::Free()
{
}

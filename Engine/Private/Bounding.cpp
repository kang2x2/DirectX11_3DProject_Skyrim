#include "Bounding.h"

CBounding::CBounding()
{
}

HRESULT CBounding::Initialize(const BOUNDING_DESC* _pDesc)
{
	m_vCenter = _pDesc->vCenter;

	return S_OK;
}

void CBounding::Set_ColliderDesc(_float _fExtents)
{

}

void CBounding::Free()
{
}

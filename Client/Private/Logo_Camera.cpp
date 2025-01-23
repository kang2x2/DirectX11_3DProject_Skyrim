#include "framework.h"
#include "Logo_Camera.h"

#include "GameInstance.h"

CLogo_Camera::CLogo_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCamera(_pDevice, _pContext)
{
}

CLogo_Camera::CLogo_Camera(const CLogo_Camera& rhs)
	: CCamera(rhs)
{
}

HRESULT CLogo_Camera::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CLogo_Camera::Initialize_Clone(void* pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	m_bHasMesh = false;
	m_strName = TEXT("Logo_Camera");

	return S_OK;
}

void CLogo_Camera::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);
}

void CLogo_Camera::LateTick(_float _fTimeDelta)
{
}

CLogo_Camera* CLogo_Camera::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLogo_Camera* pInstnace = new CLogo_Camera(_pDevice, _pContext);

	if (FAILED(pInstnace->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CLogo_Camera");
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

CGameObject* CLogo_Camera::Clone(void* pArg)
{
	CLogo_Camera* pInstnace = new CLogo_Camera(*this);

	if (FAILED(pInstnace->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CLogo_Camera");
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

void CLogo_Camera::Free()
{
	__super::Free();
}

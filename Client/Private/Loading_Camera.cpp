#include "framework.h"
#include "Loading_Camera.h"

#include "GameInstance.h"

CLoading_Camera::CLoading_Camera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CCamera(_pDevice, _pContext)
{
}

CLoading_Camera::CLoading_Camera(const CLoading_Camera& rhs)
	: CCamera(rhs)
{
}

HRESULT CLoading_Camera::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CLoading_Camera::Initialize_Clone(void* pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	m_bHasMesh = false;
	m_strName = TEXT("Loading_Camera");

	return S_OK;
}

void CLoading_Camera::Tick(_float _fTimeDelta)
{
	m_pTransformCom->Go_Foward(_fTimeDelta);

	__super::Tick(_fTimeDelta);
}

void CLoading_Camera::LateTick(_float _fTimeDelta)
{
}

CLoading_Camera* CLoading_Camera::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CLoading_Camera* pInstnace = new CLoading_Camera(_pDevice, _pContext);

	if (FAILED(pInstnace->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CLoading_Camera");
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

CGameObject* CLoading_Camera::Clone(void* pArg)
{
	CLoading_Camera* pInstnace = new CLoading_Camera(*this);

	if (FAILED(pInstnace->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CLoading_Camera");
		Safe_Release(pInstnace);
	}

	return pInstnace;
}

void CLoading_Camera::Free()
{
	__super::Free();
}

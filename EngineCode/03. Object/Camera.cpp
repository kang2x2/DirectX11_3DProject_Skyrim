#include "Camera.h"
#include "PipeLine.h"

CCamera::CCamera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDevice, _pContext)
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CCamera::Initialize_Clone(void* pArg)
{
	// 구조체 생성 : 매개 번수로 받은 Arg 대입.
	CAMERA_DESC* pCameraDesc = (CAMERA_DESC*)pArg;

	// 멤버 변수들 값 채우기(카메라 상태 설정)
	vEye    = pCameraDesc->vEye; // 카메라의 위치
	vAt     = pCameraDesc->vAt;  // 카메라가 바라보는 지점 혹은 대상
	fFovY   = pCameraDesc->fFovY;
	fAspect = pCameraDesc->fAspect;
	fNear   = pCameraDesc->fNear;
	fFar    = pCameraDesc->fFar;

	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (m_pTransformCom == nullptr)
		return E_FAIL;

	if (FAILED(m_pTransformCom->Initialize_Clone(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vEye)); // 위치
	m_pTransformCom->LookAt(XMLoadFloat4(&vAt)); // 바라볼 지점 혹은 대상

	matView = m_pTransformCom->Get_WorldMatrix_Inverse();

	return S_OK;
}

void CCamera::Tick(_float _fTimeDelta)
{
	if (m_pPipeLine == nullptr)
		return;

	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(fFovY, fAspect, fNear, fFar));
}

void CCamera::LateTick(_float _fTimeDelta)
{
}

const _vector CCamera::Get_CamLook()
{
	return m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}

void CCamera::Zoom_In(_float _fZoomSpeed, _float _fTimeDelta)
{
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * _fZoomSpeed * _fTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CCamera::Zoom_Out(_float _fZoomSpeed, _float _fTimeDelta)
{
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * -_fZoomSpeed * _fTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pPipeLine);
}

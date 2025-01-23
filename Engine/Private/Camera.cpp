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

	// 트랜스 폼 생성.
	/* 카메라의 상태를 제어한다. */
	/* 원점에 항등상태로 존재한다. */
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (m_pTransformCom == nullptr)
		return E_FAIL;

	/* 월드공간에서의 이동, 회전 속도를 셋팅(pArg 매개에 다 포함되어 있음). */
	if (FAILED(m_pTransformCom->Initialize_Clone(pArg)))
		return E_FAIL;

	/* 내가 원하는 카메라의 상태를 CTransform 에게 동기화한다. */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vEye)); // 위치
	m_pTransformCom->LookAt(XMLoadFloat4(&vAt)); // 바라볼 지점 혹은 대상

	matView = m_pTransformCom->Get_WorldMatrix_Inverse();

	return S_OK;
}

void CCamera::Tick(_float _fTimeDelta)
{
	if (m_pPipeLine == nullptr)
		return;
	/* 자식클래스에 필요한 상태 변환을 마쳐놓은 상태다. */


	/* 갱신된 정보를 파이프라인에 저장하자. */

	/* 카메라 월드행렬의 역행렬 == 뷰스페이스 변환행렬. */
	// m_pPipeLine->Set_Transform(CPipeLine::D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
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

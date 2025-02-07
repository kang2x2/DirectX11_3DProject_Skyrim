#include "Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_float3 CTransform::Get_Scaled()
{
	return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
}

_float3 CTransform::Get_Rotated()
{
	_matrix matRotation = XMMatrixRotationQuaternion(XMQuaternionRotationMatrix(XMLoadFloat4x4(&m_WorldMatrix)));

	_float3 vRotationAngles;
	_vector vScale, vRotation, vTranslation;
	XMMatrixDecompose(&vScale, &vRotation, &vTranslation, matRotation);
	XMStoreFloat3(&vRotationAngles, vRotation);

	return vRotationAngles;
}

void CTransform::Set_State(STATE _eState, _fvector _vState)
{
	_matrix StateMatrix;
	StateMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	StateMatrix.r[_eState] = _vState;

	XMStoreFloat4x4(&m_WorldMatrix, StateMatrix);
}

void CTransform::Set_Scaling(const _float3& _vScale)
{
	_vector vRight = Get_State(STATE_RIGHT);
	_vector vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * _vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * _vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * _vScale.z);
}

void CTransform::Set_WorldMatrix(const _matrix& _matrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, _matrix);
}

HRESULT CTransform::Initialize_ProtoType()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize_Clone(void* pArg)
{
	if (pArg != nullptr)
	{
		TRANSFORM_DESC TransformDesc;

		memmove(&TransformDesc, pArg, sizeof(TransformDesc));

		m_fSpeedPerSec = TransformDesc.fSpeedPerSec;
		m_fRotationRadianPerSec = TransformDesc.fRotationRadianPerSec;
		m_fZoomPerSec = TransformDesc.fZoomPerSec;
	}

	return S_OK;
}

HRESULT CTransform::Bind_ShaderResources(CShader* _pShader, const char* _pConstantName)
{
	return _pShader->Bind_Matrix(_pConstantName, &m_WorldMatrix);
}

void CTransform::Go_Foward(_float _fTimeDelta, CNavigation* _pNavigation)
{
	_vector vLook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * _fTimeDelta;

	if (nullptr == _pNavigation ||
		true == _pNavigation->IsMove(vPosition))
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float _fTimeDelta, CNavigation* _pNavigation)
{
	_vector vLook = Get_State(STATE_LOOK);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * _fTimeDelta;

	if (nullptr == _pNavigation ||
		true == _pNavigation->IsMove(vPosition))
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float _fTimeDelta, CNavigation* _pNavigation)
{
	_vector vRight = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * _fTimeDelta;

	if (nullptr == _pNavigation ||
		true == _pNavigation->IsMove(vPosition))
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float _fTimeDelta, CNavigation* _pNavigation)
{
	_vector vRight = Get_State(STATE_RIGHT);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * _fTimeDelta;

	if (nullptr == _pNavigation ||
		true == _pNavigation->IsMove(vPosition))
		Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Up(_float _fTimeDelta)
{
	_vector vUp = Get_State(STATE_UP);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * _fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Down(_float _fTimeDelta)
{
	_vector vUp = Get_State(STATE_UP);
	_vector vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vUp) * m_fSpeedPerSec * _fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Fix_Rotation(FXMVECTOR _vAxis, _float _fRadian)
{
	_float3 vScaled = Get_Scaled();
	_vector vRight  =  XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector vUp		=  XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector vLook	=  XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix RotationMatrix = XMMatrixRotationAxis(_vAxis, _fRadian);

	vRight  = XMVector4Transform(vRight, RotationMatrix);
	vUp		= XMVector4Transform(vUp, RotationMatrix);
	vLook	= XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Turn(FXMVECTOR _vAxis, _float _fTimeDelta, _float _fRotationRadianPerSec)
{
	_vector vRight = Get_State(STATE_RIGHT);
	_vector vUp = Get_State(STATE_UP);
	_vector vLook = Get_State(STATE_LOOK);

	_matrix RotationMatrix = XMMatrixRotationAxis(_vAxis, (m_fRotationRadianPerSec + _fRotationRadianPerSec) * _fTimeDelta);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::LookAt(_fvector _vPoint)
{
	_float3 vScaled = Get_Scaled();

	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = XMVector3Normalize(_vPoint - vPosition) * vScaled.z; // 방향 구하기
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScaled.x;
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScaled.y;
	
	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::LookAt_Fade(_fvector _vPoint, _float _fTimeDelta, _float _fRotationRadianPerSec)
{	
	_float3 vScaled = Get_Scaled();
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vCurrentLook = Get_State(STATE_LOOK);

	// 목표 방향과 현재 방향 간의 각도 계산
	_vector vTargetLook = XMVector3Normalize(_vPoint - vPosition);
	float angle = acos(XMVectorGetX(XMVector3Dot(vCurrentLook, vTargetLook)));

	// 일정 비율로 회전 각도 계산
	float rotationAmount = _fRotationRadianPerSec * _fTimeDelta; 
	float rotationFactor = min(rotationAmount / angle, 1.0f);

	if (angle > 0.1f) {
		// 쿼터니언으로 회전 계산
		_vector vRotationAxis = XMVector3Normalize(XMVector3Cross(vCurrentLook, vTargetLook));
		_vector vRotation = XMQuaternionRotationAxis(vRotationAxis, rotationFactor * angle);

		// 현재 방향을 회전한 결과를 얻음
		_vector vNewLook = XMVector3Rotate(vCurrentLook, vRotation);
		_vector vNewRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vNewLook));
		_vector vNewUp = XMVector3Normalize(XMVector3Cross(vNewLook, vNewRight));

		// 부드러운 회전 결과를 설정
		Set_State(STATE_RIGHT, vNewRight);
		Set_State(STATE_UP, vNewUp);
		Set_State(STATE_LOOK, vNewLook);
	}
	else {
		// 목표 방향에 거의 도달한 경우, 정확한 위치에서 바라보도록 설정
		Set_State(STATE_LOOK, vTargetLook);
		Set_State(STATE_RIGHT, XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vTargetLook)));
		Set_State(STATE_UP, XMVector3Normalize(XMVector3Cross(vTargetLook, Get_State(STATE_RIGHT))));
	}
}

void CTransform::SetLook(_fvector _vPoint)
{
	// 크기 받아옴. (길이를 1로 초기화 하고 연산하기 때문에 크기가 망가질 수 있어 보관.)
	_float3 vScaled = Get_Scaled();

	_float4 vPoint = {0.f, 0.f, 0.f, 0.f};
	XMStoreFloat4(&vPoint, _vPoint);
	vPoint.y = 0;


	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = XMVector3Normalize(XMLoadFloat4(&vPoint)) * vScaled.z; // 방향 구하기
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScaled.x;
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScaled.y;

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Chase(_fvector _vPoint, _float _fTimeDelta, _float _fMargin)
{
	// 포지션 가져오기
	_vector vPosition = Get_State(STATE_POSITION);
	// 방향 구하기 (대상 포지션 - 내 포지션)
	_vector vDir = _vPoint - vPosition;
	// 부들부들 떨리는 거 방지하기 위해 margin을 설정해 주었음.
	if (XMVectorGetX(XMVector3Length(vDir)) > _fMargin)
		vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * _fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

CTransform* CTransform::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CTransform* pInstance = new CTransform(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}

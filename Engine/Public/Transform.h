#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float fSpeedPerSec = 0.f;
		_float fRotationRadianPerSec = 0.f;
		_float fZoomPerSec = 0.f;
	}TRANSFORM_DESC;
	
private:
	CTransform(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	/* 월드 공간상에서의 행렬의 각 행 정보(라, 업, 룩, 포)를 리턴한다. */
	_vector Get_State(STATE _eState)
	{
		//  XMLoadFloat4x4 : float4x4를 XMMATRIX로 치환.
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		return worldMatrix.r[_eState];
	}
	_float3 Get_Scaled();
	// _vector가 아닌 _fvector와 같이 알파벳이 붙은 vector const 형이다.
	/* 월드행렬에 각 행에 필요한 정보를 넣어준다. */
	_float3 Get_Rotated();

	void Set_State(STATE _eState, _fvector _vState);
	/* 행렬의 스케일 정보를 vScale에 담겨있는 각각 값으로 셋한다. */
	void Set_Scaling(const _float3& _vScale);

	void Set_WorldMatrix(const _matrix& _matrix);

	/* 속도 제어 */
	void Set_Speed(_float _fSpeed) { m_fSpeedPerSec = _fSpeed; }

	// 월드 행렬 반환
	_float4x4 Get_WorldMatrix_Float4x4() const
	{
		return m_WorldMatrix;
	}
	_matrix Get_WorldMatrix() const
	{
		return XMLoadFloat4x4(&m_WorldMatrix);
	}
	// 월드 역행렬 반환
	_float4x4 Get_WorldMatrix_Float4x4_Inverse() const
	{
		_float4x4 tempMatrix;
		XMStoreFloat4x4(&tempMatrix, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
		return tempMatrix;
	}
	_matrix Get_WorldMatrix_Inverse() const
	{
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

public:
	virtual HRESULT Initialize_ProtoType() override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	HRESULT Bind_ShaderResources(class CShader* _pShader, const char* _pConstantName);

public:
	void Go_Foward(_float _fTimeDelta, class CNavigation* _pNavigation = nullptr);
	void Go_Backward(_float _fTimeDelta, class CNavigation* _pNavigation = nullptr);
	void Go_Left(_float _fTimeDelta, class CNavigation* _pNavigation = nullptr);
	void Go_Right(_float _fTimeDelta, class CNavigation* _pNavigation = nullptr);
	void Go_Up(_float _fTimeDelta);
	void Go_Down(_float _fTimeDelta);

	void Fix_Rotation(FXMVECTOR _vAxis, _float _fRadian);
	void Turn(FXMVECTOR _vAxis, _float _fTimeDelta, _float _fRotationRadianPerSec = 0.f);
	void LookAt(_fvector _vPoint);
	void LookAt_Fade(_fvector _vPoint, _float _fTimeDelta, _float _fRotationRadianPerSec);
	void SetLook(_fvector _vPoint);
	void Chase(_fvector _vPoint, _float _fTimeDelta, _float _fMargin = 0.1f);

private:
	/* XMFLOAT2, XMFLOAT3, XMFLOAT4 : 저장용벡터. */
	/* XMFLOAT4X4 : 단순 float 16개를 저장하고 있는 구조체. */
	/* 실제 연산의 기능은 가지고 있지 않다. */
	/* 행렬의 정보를 보관하는 기능을 위해 선언하는 타입. */

	/* XMVECTOR : 연산용벡터. */
	/* XMMATRIX : 행렬의 연산을 위해 제공되는 타입. */

	_float4x4			m_WorldMatrix; // 저장용

	_float				m_fSpeedPerSec = { 10.0f }; // 실시간 이동 속도.
	_float				m_fRotationRadianPerSec = { 0.0f }; // 실시간 회전 속도.
	_float				m_fZoomPerSec = { 0.0f }; // 실시간 줌 속도.

public:
	static CTransform* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END


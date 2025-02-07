#pragma once

/* 다수의 Instance를 그려내는 클래스들의 부모 */
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	typedef struct tagInstanceDesc
	{
		_float3 vCenter; // 어딜 중점으로
		_float3 vRange; // 얼만큼 범위를 지정할건지
		_float  fSpeedMin, fSpeedMax = 0.f; // 스피드 난수
		_float  fScaleMin, fScaleMax = 0.f; // 스케일 난수
		_float  fLifeTimeMin, fLifeTimeMax = 0.f; // 생명주기 난수
		_uint   iNumInstance = 0; // 생성 할 인스턴스 수.
	}INSTANCE_DESC;
protected:
	CVIBuffer_Instancing(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_ProtoType(const INSTANCE_DESC& _InstanceDesc);
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual HRESULT Update(_float _fTimeDelta);
	virtual HRESULT Render() override;

protected:
	_uint			m_iStrideInstance = 0; // 크기
	_uint			m_iNumInstance = 0; // 그려낼 개수
	_uint			m_iNumIndicesPerInstance = 0; 
	ID3D11Buffer*	m_pVBInstance = nullptr;
	VTXINSTANCE*	m_pVertices = nullptr;

	_float*			m_pSpeedAry = nullptr;
	_float*			m_pLifeTimeAry = nullptr;
	_float*			m_pTimeAccAry = nullptr;
	_bool*			m_bIsDrop = nullptr;

public:
	virtual CComponent* Clone(void* _pArg) = 0;
	virtual void Free() override;
};

END


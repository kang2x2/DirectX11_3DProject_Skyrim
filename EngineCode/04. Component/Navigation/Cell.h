#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum CELLPOINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum CELLLINE  { LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum CELLRENDER_TYPE  { CELL_OUTLINE, CELL_FILL, CELL_END };
private:
	CCell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCell() = default;

public:
	const _float3* Get_LocalPoints(_uint _ePoint) const
	{ return &m_localPoints[_ePoint]; }

	void Set_Neighbor(CELLLINE _eLine, CCell* _pCell) 
	{ m_iNeighborIndices[_eLine] = _pCell->m_iIndex; }

public:
	HRESULT Initialize(const _float3* _pPoints, _uint _iIndex);
	void Update(_fmatrix _matWorld);
	_bool Compare_Points(const _float3* _pSourPoints, const _float3* _pDestPoints);
	_bool IsOut(_fvector _vPoints, _fmatrix _matWorld, _int* _pINeighborIndex);

#ifdef _DEBUG
public:
	HRESULT Render(CELLRENDER_TYPE _eType);
#endif

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;
#ifdef _DEBUG
	class CVIBuffer_Cell_Outline*	m_pVIBufferOutCom = nullptr;
	class CVIBuffer_Cell_Fill*		m_pVIBufferInCom = nullptr;
#endif

	_uint					m_iIndex = 0; // 자기 번호(인덱스)
	_float3					m_localPoints[POINT_END]; // 로컬 상 위치
	_float3					m_worldPoints[POINT_END]; // 월드 상 위치
	_float3					m_normals[LINE_END]; // 선분 당 법선 
	_int 					m_iNeighborIndices[LINE_END] = { -1, -1, -1 }; // 선분 당 이웃
	
public:
	static CCell* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _float3* _pPoints, _uint _iIndex);
	virtual void Free() override;
};

END


#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Grid final : public CVIBuffer
{
private:
	CVIBuffer_Grid(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Grid(const CVIBuffer_Grid& rhs);
	virtual ~CVIBuffer_Grid() = default;

public:
	virtual HRESULT Initialize_ProtoType(_uint _iTerrainWidth, _uint _iTerrainHeight);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	const _float3* Get_VtxPos() { return m_pPos; }

private:
	_ulong			m_iNumVerticesX = { 0 };
	_ulong			m_iNumVerticesZ = { 0 };
	_float3*		m_pPos = nullptr;

public:
	static CVIBuffer_Grid* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	virtual CVIBuffer* Clone(void* pArg) override;
	virtual void Free() override;

};

END


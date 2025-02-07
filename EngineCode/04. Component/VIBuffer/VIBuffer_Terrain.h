#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{ 
private:
	CVIBuffer_Terrain(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_ProtoType(const wstring & _strHeightMapFilePath);
	virtual HRESULT Initialize_Clone(void* pArg) override;

private:
	_ulong			m_iNumVerticesX = { 0 };
	_ulong			m_iNumVerticesZ = { 0 };

public:
	static CVIBuffer_Terrain* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext, const wstring& _strHeightMapFilePath);
	virtual CVIBuffer* Clone(void* pArg) override;
	virtual void Free() override;

};

END


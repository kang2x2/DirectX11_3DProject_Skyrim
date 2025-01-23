#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_ProtoType()		 override;
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual HRESULT Render();

protected:
	ID3D11Buffer*				m_pVB = nullptr;
	ID3D11Buffer*				m_pIB = nullptr;

	D3D11_BUFFER_DESC			m_BufferDesc;
	D3D11_SUBRESOURCE_DATA		m_InitialData;
	DXGI_FORMAT					m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;
	_uint						m_iStride = 0; // 정점 하나의 크기
	_uint						m_iNumVertices = 0;
	_uint						m_iIndexStride = 0;
	_uint						m_iNumIndices = 0;
	_uint						m_iNumVBs = 0;


protected:
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer** ppOut);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END


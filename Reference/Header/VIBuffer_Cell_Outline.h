#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Cell_Outline final : public CVIBuffer
{
private:
	CVIBuffer_Cell_Outline(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Cell_Outline(const CVIBuffer_Cell_Outline& rhs);
	virtual ~CVIBuffer_Cell_Outline() = default;

public:
	virtual HRESULT Initialize_ProtoType(const _float3* _Points);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	static CVIBuffer_Cell_Outline* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext, const _float3* _Points);
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


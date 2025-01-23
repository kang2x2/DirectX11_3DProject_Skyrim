#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	CVIBuffer_Rect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_Rect() = default;

public:
	virtual HRESULT Initialize_ProtoType()		 override;
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	static CVIBuffer_Rect* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END


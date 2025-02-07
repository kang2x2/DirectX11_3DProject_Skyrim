#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Collision final : public CVIBuffer
{
private:
	CVIBuffer_Collision(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Collision(const CVIBuffer_Collision& rhs);
	virtual ~CVIBuffer_Collision() = default;

public:
	virtual HRESULT Initialize_ProtoType(const _float3 * _Points);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	static CVIBuffer_Collision* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext);
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


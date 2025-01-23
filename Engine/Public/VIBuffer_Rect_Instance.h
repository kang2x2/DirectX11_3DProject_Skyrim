#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer_Instancing
{
private:
	CVIBuffer_Rect_Instance(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_ProtoType(const INSTANCE_DESC & _InstanceDesc);
	virtual HRESULT Initialize_Clone(void* pArg) override;
	virtual HRESULT Update(_float _fTimeDelta) override;
	virtual HRESULT Update_Spark(_float _fTimeDelta);

public:

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext, const INSTANCE_DESC& _InstanceDesc);
	virtual CComponent* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


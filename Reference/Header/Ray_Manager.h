#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRay_Manager : public CBase
{
	DECLARE_SINGLETON(CRay_Manager)
private:
	CRay_Manager();
	virtual ~CRay_Manager() = default;

public:
	_float3 Return_WorldMousePos(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const POINT& _WinMousePos) const;

private:


public:
	virtual void Free() override;
};

END


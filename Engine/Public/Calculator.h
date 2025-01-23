#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCalculator : public CBase
{
	DECLARE_SINGLETON(CCalculator)
private:
	CCalculator();
	virtual ~CCalculator() = default;

public:
	_float3 Picking_Position(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext,
		const POINT& _WinMousePos, class CGameObject* _pTerrain, const _float3* _vec, _uint _iLevel);
	_float3 Picking_Grid(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext,
		const POINT& _WinMousePos, class CGameObject* _pTerrain, const _float3* _vec) const;

	CGameObject* Picking_Object(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext,
		const POINT& _WinMousePos, _uint _iLevel);

	CGameObject* Picking_Light(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const POINT& _WinMousePos, _uint _iLevel);


private:

public:
	virtual void Free() override;
};

END


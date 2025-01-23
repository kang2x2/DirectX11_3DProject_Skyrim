#pragma once

#include "Client_Defines.h"
#include "Level.h"


BEGIN(Client)

class CLevel_Castle final : public CLevel
{
private:
	CLevel_Castle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Castle() = default;

public:
	virtual HRESULT Initialize()override;
	virtual HRESULT	Tick(_float _fTimeDelta)override;
	virtual HRESULT	LateTick(_float _fTimeDelta)override;

public:
	virtual void AfterRender();

public:
	static CLevel_Castle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END


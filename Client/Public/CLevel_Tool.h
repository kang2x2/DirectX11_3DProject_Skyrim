#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Tool final : public CLevel
{
private:
	CLevel_Tool(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CLevel_Tool(const CLevel_Tool& rhs);
	virtual ~CLevel_Tool() = default;


public:
	virtual HRESULT Initialize()override;
	virtual HRESULT	Tick(_float _fTimeDelta)override;
	virtual HRESULT	LateTick(_float _fTimeDelta)override;

public:
	virtual void AfterRender();

public:
	HRESULT Ready_Light();
	HRESULT Ready_Layer_Equip(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Camera(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Terrain(const wstring& _strLayerTag);

private:

public:
	static CLevel_Tool* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END


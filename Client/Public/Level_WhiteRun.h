#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_WhiteRun final : public CLevel
{
private:
	CLevel_WhiteRun(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_WhiteRun() = default;

public:
	virtual HRESULT Initialize()override;
	virtual HRESULT	Tick(_float _fTimeDelta)override;
	virtual HRESULT	LateTick(_float _fTimeDelta)override;

public:
	virtual void AfterRender();

private:
	HRESULT Ready_Cursor(const wstring& _strLayerTag);
	HRESULT Ready_Level();
	HRESULT Ready_Light();

	HRESULT Ready_Layer_Equip(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Player(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Inventory(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Navigation_WhiteRun(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Sky(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Terrain(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Particle(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Effect(const wstring& _strLayerTag);

private:
	static _uint m_iVisitCount;

public:
	static CLevel_WhiteRun* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END


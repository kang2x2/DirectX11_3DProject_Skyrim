#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT	Tick(_float _fTimeDelta)override;
	virtual HRESULT	LateTick(_float _fTimeDelta)override;

public:
	virtual void AfterRender() {};
private:
	HRESULT Ready_Cursor(const wstring& _strLayerTag);
	HRESULT Ready_Layer_Camera(const wstring& _strLayerTag);
	HRESULT Ready_Light();
	HRESULT Ready_Layer_BackGround(const wstring& _strLayerTag);
	HRESULT Ready_Layer_LogoObj(const wstring& _strLayerTag);
	HRESULT Ready_Layer_LogoText(const wstring& _strLayerTag);

public:
	static class CLevel_Logo* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;
};

END

#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVELID _eNextLevel);
	virtual HRESULT	Tick(_float _fTimeDelta)override;
	virtual HRESULT	LateTick(_float _fTimeDelta)override;

private:
	HRESULT Ready_StaticInit();
	HRESULT Ready_LoadingClone(const wstring& _strLayerTag);
	HRESULT Ready_Light(const wstring& _strLayerTag);
private:
	// 로딩 후 이동 할 레벨
	LEVELID			m_eNextLevel = LEVEL_END;
	class CLoader*  m_pLoader = nullptr;

public:
	static CLevel_Loading* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, LEVELID _eNextLevel);
	virtual void Free() override;
};

END


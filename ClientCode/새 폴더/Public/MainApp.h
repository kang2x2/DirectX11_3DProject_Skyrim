#pragma once

#include "Client_Defines.h"

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CRenderer;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void	Tick(_float _fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device*		 m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	CGameInstance*		 m_pGameInstance = nullptr;
	CRenderer*			 m_pRenderer = nullptr; 

//#ifdef _DEBUG
private:
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_uint					m_iRenderCount = { 0 };
	_float					m_fTimeAcc = { 0 };
//#endif


private:
	HRESULT Open_Level(LEVELID _eLevelID);

	HRESULT Ready_ProtoType_Components();

public:
	static		 CMainApp* Create();
	virtual void		   Free() override;
};

END

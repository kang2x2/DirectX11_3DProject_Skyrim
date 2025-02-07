#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
	class CRenderer;
	class CNavigation;
END

BEGIN(Client)

class CNavigation_Client final : public CGameObject
{
private:
	CNavigation_Client(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CNavigation_Client(const CNavigation_Client& rhs);
	virtual ~CNavigation_Client() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual HRESULT Initialize_Clone(_uint _iLevel, const wstring& _strModelComTag, void* pArg); // 사본
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*		 m_pRendererCom = nullptr;
	CNavigation*	 m_pNavigationCom[STAGE_END];

private:
	HRESULT Ready_Component();

public:
	static CNavigation_Client* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


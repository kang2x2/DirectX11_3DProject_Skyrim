#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

END

BEGIN(Client)

class CSkyrimItem abstract : public CGameObject
{
public:
	enum ITEMTYPE { ITEM_WEAPON, ITEM_ARMOR, ITEM_POTION, ITEM_END };

protected:
	CSkyrimItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimItem(const CSkyrimItem& rhs);
	virtual ~CSkyrimItem() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* _pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	ITEMTYPE Get_ItemType() { return m_eItemType; }
	_bool	 Get_PocketItem() { return m_bIsPocket; }
	void	 Set_PocketITem(_bool _bIsSocket) { m_bIsPocket = _bIsSocket; }

protected:
	ITEMTYPE	m_eItemType = ITEM_END;
	_bool		m_bIsPocket = false;
public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;

};

END


#pragma once

#include "SkyrimUI.h"
#include "SkyrimItem.h"

BEGIN(Client)

class CInventory final : public CSkyrimUI
{
public:
	enum INVEN_PART { PART_CATEGORY, PART_LIST, PART_UNDERBAR, PART_END };
private:
	CInventory(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void  Set_IsInvenShow(_bool _bIsShow);
	_bool Get_IsInvenShow() { return m_bIsShowInven; }

	void  Inven_AddItem(class CGameObject* _pItem);

	vector<class CGameObject*> GetItemList(CSkyrimItem::ITEMTYPE _eType)
	{
		return m_vecItemAry[_eType];
	}
private:
	HRESULT	Ready_Part();
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	_bool						m_bIsShowInven = false;

	vector<class CGameObject*>	  m_vecInvenPart;
	vector<class CGameObject*> m_vecItemAry[CSkyrimItem::ITEM_END];

public:
	static CInventory* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* pArg) override;
	virtual void Free() override;
};

END


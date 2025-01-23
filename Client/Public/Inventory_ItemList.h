#pragma once

#include "Inventory_PartBase.h"

BEGIN(Client)

class CInventory;
class CInventory_ItemList final : public CInventory_PartBase
{
public:
	typedef struct tagInvenItemListDesc : public INVENPART_DESC
	{
		class CPlayer*	  m_pPlayer = nullptr;
		class CInventory* pInven = nullptr;
		class CInventory_ItemCategory* pCategory = nullptr;
	}INVEN_ITEMLIST_DESC;

	typedef struct tagItemListFontDesc
	{
		class CGameObject* pItem = nullptr;
		wstring strItemName = TEXT("");
		RECT	rc = {};
		_float2 vPos = {};
		_float2 vSize = {};
		_vector vColor = {};
	}ITEMLISTFONT_DESC;

private:
	CInventory_ItemList(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CInventory_ItemList(const CInventory_ItemList& rhs);
	virtual ~CInventory_ItemList() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* _pArg);
	virtual void    PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void Init_ItemListFont(CInventory_ItemCategory::INVEN_CATEGORY _eType);
	void Set_IsShow(_bool _bIsShow) { m_bIsShow = _bIsShow; }

public:
	HRESULT	Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	_bool			m_bIsShow = false;
	class CPlayer* m_pPlayer = nullptr;
	class CInventory_ItemCategory* m_pCategory = nullptr;
	class CInventory* m_pInven = nullptr;

	vector<ITEMLISTFONT_DESC> m_vecFontDesc;
	

public:
	static CInventory_ItemList* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END


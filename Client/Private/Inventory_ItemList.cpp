#include "framework.h"
#include "Inventory_ItemList.h"

#include "GameInstance.h"

#include "Inventory.h"
#include "Inventory_ItemCategory.h"
#include "Player.h"

CInventory_ItemList::CInventory_ItemList(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CInventory_PartBase(_pDevice, _pContext)
{
}

CInventory_ItemList::CInventory_ItemList(const CInventory_ItemList& rhs)
	: CInventory_PartBase(rhs)
{
}

HRESULT CInventory_ItemList::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CInventory_ItemList::Initialize_Clone(void* _pArg)
{
	INVEN_ITEMLIST_DESC* pPartDesc = (INVEN_ITEMLIST_DESC*)_pArg;
	m_pPlayer = pPartDesc->m_pPlayer;
	m_pCategory = pPartDesc->pCategory;
	m_pInven = pPartDesc->pInven;

	if (FAILED(__super::Initialize_Clone(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_strName = TEXT("Inventory_ItemList");

	return S_OK;
}

void CInventory_ItemList::PriorityTick(_float _fTimeDelta)
{
}

void CInventory_ItemList::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	if (m_pCategory->Get_IsChangeCategory())
		Init_ItemListFont(m_pCategory->Get_CurCategory());

	POINT MousePos = {};
	GetCursorPos(&MousePos);
	ScreenToClient(g_hWnd, &MousePos);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < m_vecFontDesc.size(); ++i)
	{
		if (MousePos.x >= m_vecFontDesc[i].rc.left &&
			MousePos.x <= m_vecFontDesc[i].rc.right &&
			MousePos.y >= m_vecFontDesc[i].rc.top &&
			MousePos.y <= m_vecFontDesc[i].rc.bottom)
		{
			m_vecFontDesc[i].vColor = { 1.f, 1.f, 1.f };

			if (pGameInstance->Get_DIKeyDown(MK_LBUTTON))
			{
				pGameInstance->PlaySoundFile(TEXT("itm_clothing_up_02.wav"), CHANNEL_NATURAL, 1.f);
				if (dynamic_cast<CSkyrimItem*>(m_vecFontDesc[i].pItem)->Get_ItemType() == CSkyrimItem::ITEM_WEAPON)
				{
					m_pPlayer->Use_Item(dynamic_cast<CSkyrimItem*>(m_vecFontDesc[i].pItem));
				}
				else if (dynamic_cast<CSkyrimItem*>(m_vecFontDesc[i].pItem)->Get_ItemType() == CSkyrimItem::ITEM_ARMOR)
				{
					m_pPlayer->Use_Item(dynamic_cast<CSkyrimItem*>(m_vecFontDesc[i].pItem));
				}
				else if (dynamic_cast<CSkyrimItem*>(m_vecFontDesc[i].pItem)->Get_ItemType() == CSkyrimItem::ITEM_POTION)
				{
					m_pPlayer->Use_Item(dynamic_cast<CSkyrimItem*>(m_vecFontDesc[i].pItem));
				}
			}
		}
		else
		{
			m_vecFontDesc[i].vColor = { 0.7f, 0.7f, 0.7f };
		}
	}

	Safe_Release(pGameInstance);

}

void CInventory_ItemList::LateTick(_float _fTimeDelta)
{
	if(m_bIsShow)
		m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
}

HRESULT CInventory_ItemList::Render()
{
	if (m_bIsShow)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		__super::Render();

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		for (_uint i = 0; i < m_vecFontDesc.size(); ++i)
		{
			pGameInstance->Render_Font(TEXT("Font_Bold"), m_vecFontDesc[i].strItemName,
				_float2(m_vecFontDesc[i].vPos.x, m_vecFontDesc[i].vPos.y),
				m_vecFontDesc[i].vColor
				, 0.f, _float2(180.f, 30.f), 0.5f);
		}

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

void CInventory_ItemList::Init_ItemListFont(CInventory_ItemCategory::INVEN_CATEGORY _eType)
{
	m_pCategory->Set_IsChangeCategory(false);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_vecFontDesc.clear();

	_float2 fSize = {180.f, 30.f};
	_float fMarginY = 50.f;

	if (_eType == CInventory_ItemCategory::CATEGORY_WEAPON)
	{
		for (_uint i = 0; i < m_pInven->GetItemList(CSkyrimItem::ITEM_WEAPON).size(); ++i)
		{
			ITEMLISTFONT_DESC tempFontDesc;
			tempFontDesc.pItem = m_pInven->GetItemList(CSkyrimItem::ITEM_WEAPON)[i];
			tempFontDesc.strItemName = m_pInven->GetItemList(CSkyrimItem::ITEM_WEAPON)[i]->Get_Name();
			tempFontDesc.vPos = { 400.f , 200.f + ((_float)i * fMarginY) };
			tempFontDesc.vSize = { fSize.x, fSize.y };
			tempFontDesc.vColor = { 0.7f, 0.7f, 0.7f };
			tempFontDesc.rc = {
				(_long)(tempFontDesc.vPos.x - (200.f / 2.f)),
				(_long)(tempFontDesc.vPos.y - (30.f / 2.f)),
				(_long)(tempFontDesc.vPos.x + (200.f / 2.f)),
				(_long)(tempFontDesc.vPos.y + (30.f / 2.f)) };

			m_vecFontDesc.push_back(tempFontDesc);
		}
	}
	else if (_eType == CInventory_ItemCategory::CATEGORY_ARMOR)
	{
		for (_uint i = 0; i < m_pInven->GetItemList(CSkyrimItem::ITEM_ARMOR).size(); ++i)
		{
			ITEMLISTFONT_DESC tempFontDesc;
			tempFontDesc.pItem = m_pInven->GetItemList(CSkyrimItem::ITEM_ARMOR)[i];
			tempFontDesc.strItemName = m_pInven->GetItemList(CSkyrimItem::ITEM_ARMOR)[i]->Get_Name();
			tempFontDesc.vPos = { 400.f , 200.f + ((_float)i * fMarginY) };
			tempFontDesc.vSize = { fSize.x, fSize.y };
			tempFontDesc.vColor = { 0.7f, 0.7f, 0.7f };
			tempFontDesc.rc = {
				(_long)(tempFontDesc.vPos.x - (200.f / 2.f)),
				(_long)(tempFontDesc.vPos.y - (30.f / 2.f)),
				(_long)(tempFontDesc.vPos.x + (200.f / 2.f)),
				(_long)(tempFontDesc.vPos.y + (30.f / 2.f)) };

			m_vecFontDesc.push_back(tempFontDesc);
		}
	}
	else if (_eType == CInventory_ItemCategory::CATEGORY_POTION)
	{
		for (_uint i = 0; i < m_pInven->GetItemList(CSkyrimItem::ITEM_POTION).size(); ++i)
		{
			ITEMLISTFONT_DESC tempFontDesc;
			tempFontDesc.pItem = m_pInven->GetItemList(CSkyrimItem::ITEM_POTION)[i];
			tempFontDesc.strItemName = m_pInven->GetItemList(CSkyrimItem::ITEM_POTION)[i]->Get_Name();
			tempFontDesc.vPos = { 400.f , 200.f + ((_float)i * fMarginY) };
			tempFontDesc.vSize = { fSize.x, fSize.y };
			tempFontDesc.vColor = { 0.7f, 0.7f, 0.7f };
			tempFontDesc.rc = {
				(_long)(tempFontDesc.vPos.x - (200.f / 2.f)),
				(_long)(tempFontDesc.vPos.y - (30.f / 2.f)),
				(_long)(tempFontDesc.vPos.x + (200.f / 2.f)),
				(_long)(tempFontDesc.vPos.y + (30.f / 2.f)) };

			m_vecFontDesc.push_back(tempFontDesc);
		}
	}

	m_bIsShow = true;

	Safe_Release(pGameInstance);

}

HRESULT CInventory_ItemList::Ready_Component()
{
	return S_OK;
}

HRESULT CInventory_ItemList::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

CInventory_ItemList* CInventory_ItemList::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CInventory_ItemList* pInstance = new CInventory_ItemList(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CInventory_ItemList");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInventory_ItemList::Clone(void* _pArg)
{
	CInventory_ItemList* pInstance = new CInventory_ItemList(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone : CInventory_ItemList");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory_ItemList::Free()
{
	__super::Free();
}

#include "framework.h"
#include "Inventory.h"

#include "GameInstance.h"

#include "Inventory_ItemCategory.h"
#include "Inventory_ItemList.h"
#include "Inventory_UnderBar.h"

#include "Player.h"

CInventory::CInventory(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CSkyrimUI(_pDevice, _pContext)
{
}

CInventory::CInventory(const CInventory& rhs)
	: CSkyrimUI(rhs)
{
}

HRESULT CInventory::Initialize_ProtoType()
{
	m_vecInvenPart.resize(PART_END);

	for (_int i = 0; i < m_vecInvenPart.size(); ++i)
	{
		m_vecInvenPart[i] = nullptr;
	}

	return S_OK;
}

HRESULT CInventory::Initialize_Clone(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(Ready_Part()))
		return E_FAIL;

	m_strName = TEXT("UI_Inventory");

	m_bIsMaintain = true;

	return S_OK;
}

void CInventory::Tick(_float _fTimeDelta)
{
	if (m_bIsShowInven)
	{
		for (_uint i = 0; i < m_vecInvenPart.size(); ++i)
		{
			m_vecInvenPart[i]->Tick(_fTimeDelta);
		}
	}

}

void CInventory::LateTick(_float _fTimeDelta)
{
	if (m_bIsShowInven)
	{
		for (_uint i = 0; i < m_vecInvenPart.size(); ++i)
		{
			m_vecInvenPart[i]->LateTick(_fTimeDelta);
		}
	}
}

HRESULT CInventory::Render()
{
	if (m_bIsShowInven)
	{
		for (_uint i = 0; i < m_vecInvenPart.size(); ++i)
		{
			m_vecInvenPart[i]->Render();
		}
	}
	return S_OK;
}

void CInventory::Set_IsInvenShow(_bool _bIsShow)
{
	if (!_bIsShow)
	{
		m_bIsShowInven = false;
		dynamic_cast<CInventory_ItemList*>(m_vecInvenPart[PART_LIST])->Set_IsShow(false);
	}
	else
	{
		m_bIsShowInven = true;
	}
}

void CInventory::Inven_AddItem(class CGameObject* _pItem)
{
	if (_pItem != nullptr)
	{
		switch (dynamic_cast<CSkyrimItem*>(_pItem)->Get_ItemType())
		{
		case CSkyrimItem::ITEM_WEAPON:
			m_vecItemAry[CSkyrimItem::ITEM_WEAPON].push_back(_pItem);
			break;
		case CSkyrimItem::ITEM_ARMOR:
			m_vecItemAry[CSkyrimItem::ITEM_ARMOR].push_back(_pItem);
			break;
		case CSkyrimItem::ITEM_POTION:
			m_vecItemAry[CSkyrimItem::ITEM_POTION].push_back(_pItem);
			break;
		default:
			break;
		}

		dynamic_cast<CSkyrimItem*>(_pItem)->Set_PocketITem(true);
	}
}

HRESULT CInventory::Ready_Part()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPart = nullptr;

	Safe_Release(pGameInstance);

	/* For. Category */
	CInventory_ItemCategory::INVENPART_DESC categoryPartDesc;
	categoryPartDesc.vBlackPos = { 150.f, g_iWinSizeY * 0.5f };
	categoryPartDesc.vBlackSize = { 200.f, g_iWinSizeY };
	categoryPartDesc.vLine1Pos = { 50.f, g_iWinSizeY * 0.5f };
	categoryPartDesc.vLine1Size = { 2.f, g_iWinSizeY };
	categoryPartDesc.vLine2Pos = { 250.f, g_iWinSizeY * 0.5f };
	categoryPartDesc.vLine2Size = { 2.f, g_iWinSizeY };

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Inventory_CategoryPart"), &categoryPartDesc);
	if (pPart == nullptr)
		return E_FAIL;
	m_vecInvenPart.push_back(pPart);

	/* For. List */
	CInventory_ItemList::INVEN_ITEMLIST_DESC itemListDesc;
	itemListDesc.m_pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_CloneObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Player")));
	itemListDesc.pInven = this;
	itemListDesc.pCategory = dynamic_cast<CInventory_ItemCategory*>(m_vecInvenPart[PART_CATEGORY]);
	itemListDesc.vBlackPos = { 400.f, g_iWinSizeY * 0.5f };
	itemListDesc.vBlackSize = { 200.f, g_iWinSizeY };
	itemListDesc.vLine1Pos = { 300.f, g_iWinSizeY * 0.5f };
	itemListDesc.vLine1Size = { 2.f, g_iWinSizeY };
	itemListDesc.vLine2Pos = { 500.f, g_iWinSizeY * 0.5f };
	itemListDesc.vLine2Size = { 2.f, g_iWinSizeY };

	pPart = pGameInstance->Add_ClonePartObject(TEXT("ProtoType_GameObject_Inventory_ItemListPart"), &itemListDesc);
	if (pPart == nullptr)
		return E_FAIL;

	m_vecInvenPart.push_back(pPart);

	/* For. UnderBar */

	return S_OK;
}

HRESULT CInventory::Ready_Component()
{
	// Renderer
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Shader
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// VIBuffer
	if (FAILED(__super::Add_CloneComponent(g_curLevel, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	// Texture
	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"),
		TEXT("Com_TextureBlack"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	// Transform
	CTransform::TRANSFORM_DESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof TransformDesc);

	TransformDesc.fSpeedPerSec = 5.f;

	if (FAILED(__super::Add_CloneComponent(LEVEL_STATIC, TEXT("ProtoType_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CInventory::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 전달. */
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Textures")))
		return E_FAIL;

	return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CInventory* pInstance = new CInventory(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSkyrimUI* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_vecInvenPart.size(); ++i)
	{
		if (m_vecInvenPart[i] != nullptr)
			Safe_Release(m_vecInvenPart[i]);
	}

	for (_uint i = 0; i < CSkyrimItem::ITEM_END; ++i)
	{
		for (_uint j = 0; j < m_vecItemAry[i].size(); ++j)
		{
			if (m_vecItemAry[i][j] != nullptr)
			{
				Safe_Release(m_vecItemAry[i][j]);
			}
		}
	}
}

#include "framework.h"
#include "Inventory_ItemCategory.h"

#include "GameInstance.h"

#include "Inventory.h"

CInventory_ItemCategory::CInventory_ItemCategory(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CInventory_PartBase(_pDevice, _pContext)
{
}

CInventory_ItemCategory::CInventory_ItemCategory(const CInventory_ItemCategory& rhs)
	: CInventory_PartBase(rhs)
{
}

HRESULT CInventory_ItemCategory::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CInventory_ItemCategory::Initialize_Clone(void* _pArg)
{
	if (FAILED(__super::Initialize_Clone(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	Ready_CategoryFont();

	m_strName = TEXT("Inventory_Category");

	return S_OK;
}

void CInventory_ItemCategory::PriorityTick(_float _fTimeDelta)
{
}

void CInventory_ItemCategory::Tick(_float _fTimeDelta)
{
	__super::Tick(_fTimeDelta);

	POINT MousePos = {};
	GetCursorPos(&MousePos);
	ScreenToClient(g_hWnd, &MousePos);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < m_vecCategory.size(); ++i)
	{
		if (MousePos.x >= m_vecCategory[i].rc.left &&
			MousePos.x <= m_vecCategory[i].rc.right &&
			MousePos.y >= m_vecCategory[i].rc.top &&
			MousePos.y <= m_vecCategory[i].rc.bottom)
		{
			m_vecCategory[i].vColor = { 1.f, 1.f, 1.f };

			if (pGameInstance->Get_DIKeyDown(MK_LBUTTON))
			{
				m_eCurCategory = (INVEN_CATEGORY)i;
				m_bIsChangeCategory = true;
			}
		}
		else
		{
			m_vecCategory[i].vColor = { 0.7f, 0.7f, 0.7f };
		}
	}

	Safe_Release(pGameInstance);

}

void CInventory_ItemCategory::LateTick(_float _fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI_0, this);
}

HRESULT CInventory_ItemCategory::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	__super::Render();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < m_vecCategory.size(); ++i)
	{
		pGameInstance->Render_Font(TEXT("Font_Bold"), m_vecCategory[i].strText,
			_float2(m_vecCategory[i].vPos.x, m_vecCategory[i].vPos.y), 
			m_vecCategory[i].vColor
			, 0.f, _float2(m_fSizeX, m_fSizeY), 0.8f);
	}

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CInventory_ItemCategory::Ready_Component()
{
	return S_OK;
}

void CInventory_ItemCategory::Ready_CategoryFont()
{
	m_fSizeX = 140.f;
	m_fSizeY = 30.f;

	_float fMargin = 60.f;
	_float fX = 200.f;

	CATEGORYFONT_DESC categoryDesc;
	/* 즐겨 찾기 */
	categoryDesc.strText = TEXT("즐겨찾기");
	categoryDesc.vPos = { fX, 200.f };
	categoryDesc.rc = {
	(_long)(categoryDesc.vPos.x - (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y - (m_fSizeY / 2)),
	(_long)(categoryDesc.vPos.x + (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y + (m_fSizeY / 2))};
	m_vecCategory.push_back(categoryDesc);

	/* 전체 */
	categoryDesc.strText = TEXT("전체");
	categoryDesc.vPos = { fX,  m_vecCategory[CATEGORY_FAVORITES].vPos.y + fMargin };
	categoryDesc.rc = {
	(_long)(categoryDesc.vPos.x - (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y - (m_fSizeY / 2)),
	(_long)(categoryDesc.vPos.x + (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y + (m_fSizeY / 2)) };
	m_vecCategory.push_back(categoryDesc);

	/* 무기 */
	categoryDesc.strText = TEXT("무기");
	categoryDesc.vPos = { fX,  m_vecCategory[CATEGORY_ALL].vPos.y + fMargin };
	categoryDesc.rc = {
	(_long)(categoryDesc.vPos.x - (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y - (m_fSizeY / 2)),
	(_long)(categoryDesc.vPos.x + (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y + (m_fSizeY / 2)) };
	m_vecCategory.push_back(categoryDesc);

	/* 방어구 */
	categoryDesc.strText = TEXT("방어구");
	categoryDesc.vPos = { fX,  m_vecCategory[CATEGORY_WEAPON].vPos.y + fMargin };
	categoryDesc.rc = {
	(_long)(categoryDesc.vPos.x - (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y - (m_fSizeY / 2)),
	(_long)(categoryDesc.vPos.x + (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y + (m_fSizeY / 2)) };
	m_vecCategory.push_back(categoryDesc);

	/* 포션 */
	categoryDesc.strText = TEXT("포션");
	categoryDesc.vPos = { fX,  m_vecCategory[CATEGORY_ARMOR].vPos.y + fMargin };
	categoryDesc.rc = {
	(_long)(categoryDesc.vPos.x - (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y - (m_fSizeY / 2)),
	(_long)(categoryDesc.vPos.x + (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y + (m_fSizeY / 2)) };
	m_vecCategory.push_back(categoryDesc);

	/* 음식 */
	categoryDesc.strText = TEXT("음식");
	categoryDesc.vPos = { fX,  m_vecCategory[CATEGORY_POTION].vPos.y + fMargin };
	categoryDesc.rc = {
	(_long)(categoryDesc.vPos.x - (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y - (m_fSizeY / 2)),
	(_long)(categoryDesc.vPos.x + (m_fSizeX / 2)),
	(_long)(categoryDesc.vPos.y + (m_fSizeY / 2)) };
	m_vecCategory.push_back(categoryDesc);

}

HRESULT CInventory_ItemCategory::Bind_ShaderResources()
{
	if(FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

CInventory_ItemCategory* CInventory_ItemCategory::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CInventory_ItemCategory* pInstance = new CInventory_ItemCategory(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_ProtoType()))
	{
		MSG_BOX("Fail Create : CInventory_ItemCategory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInventory_ItemCategory::Clone(void* _pArg)
{
	CInventory_ItemCategory* pInstance = new CInventory_ItemCategory(*this);

	if (FAILED(pInstance->Initialize_Clone(_pArg)))
	{
		MSG_BOX("Fail Clone: CInventory_ItemCategory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory_ItemCategory::Free()
{
	__super::Free();
}

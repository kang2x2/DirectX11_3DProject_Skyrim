#include "framework.h"
#include "SkyrimUI.h"

CSkyrimUI::CSkyrimUI(ID3D11Device* _pDeviec, ID3D11DeviceContext* _pContext)
	: CGameObject(_pDeviec, _pContext)
{
}

CSkyrimUI::CSkyrimUI(const CSkyrimUI& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyrimUI::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CSkyrimUI::Initialize_Clone(void* pArg)
{
	m_fX = 0.f - g_iWinSizeX / 2.f;
	m_fY = 0.f + g_iWinSizeY / 2.f;

	return S_OK;
}

void CSkyrimUI::Tick(_float _fTimeDelta)
{
}

void CSkyrimUI::LateTick(_float _fTimeDelta)
{
	// 자식에서 알아서.(ui는 알파, 논알파가 다양하게 있기 때문)
}

HRESULT CSkyrimUI::Render()
{
	return S_OK;
}

void CSkyrimUI::UI_InitPos(_float _fX, _float _fY)
{
	m_fX = _fX - (g_iWinSizeX / 2.f);
	m_fY = 0.f + (g_iWinSizeY / 2.f) - _fY;
}

HRESULT CSkyrimUI::Ready_Component()
{
	return S_OK;
}

HRESULT CSkyrimUI::Bind_ShaderResources()
{
	return S_OK;
}

void CSkyrimUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
}

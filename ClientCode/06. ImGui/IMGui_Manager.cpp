#include "framework.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "IMGui_Manager.h"
#include "ImGui_Tool.h"
#include "ImGui_GamePlay.h"


IMPLEMENT_SINGLETON(CIMGui_Manager)

CIMGui_Manager::CIMGui_Manager()
{
}

CIMGui_Manager::~CIMGui_Manager()
{
}

HRESULT CIMGui_Manager::Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, LEVELID _eLevelID)
{
	if (_eLevelID == LEVEL_TOOL)
	{
		m_pCurImGui = CImGui_Tool::Create(_pDevice, _pContext);
	}
	else if (_eLevelID == LEVEL_GAMEPLAY)
	{
		m_pCurImGui = CImGui_GamePlay::Create(_pDevice, _pContext);
	}

	 
	return S_OK;
}

void CIMGui_Manager::Frame()
{
	m_pCurImGui->Frame();

	__super::Free();
}

void CIMGui_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pCurImGui);
}

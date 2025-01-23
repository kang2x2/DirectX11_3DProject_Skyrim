#include "framework.h"
#include <filesystem>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGui_GamePlay.h"

CImGui_GamePlay::CImGui_GamePlay()
{
}

HRESULT CImGui_GamePlay::Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	Safe_AddRef(_pDevice);
	Safe_AddRef(_pContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(_pDevice, _pContext);

	Safe_Release(_pDevice);
	Safe_Release(_pContext);

	return S_OK;
}

void CImGui_GamePlay::Frame()
{
	if (m_bActive)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();

		LayOut_Terrain();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

}

void CImGui_GamePlay::LayOut_Terrain()
{
	ImGui::Begin("Terrain");

	ImGui::End();
}

CImGui_GamePlay* CImGui_GamePlay::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CImGui_GamePlay* pInstance = new CImGui_GamePlay();

	if (FAILED(pInstance->Initialize(_pDevice, _pContext)))
	{
		MSG_BOX("Fail Create : CImGui_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_GamePlay::Free()
{
	__super::Free();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

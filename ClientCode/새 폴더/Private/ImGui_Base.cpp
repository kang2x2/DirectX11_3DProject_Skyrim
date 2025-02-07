#include "ImGui_Base.h"

CImGui_Base::CImGui_Base()
{
}

HRESULT CImGui_Base::Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	return S_OK;
}

void CImGui_Base::Frame()
{
}

void CImGui_Base::Free()
{
	__super::Free();
}

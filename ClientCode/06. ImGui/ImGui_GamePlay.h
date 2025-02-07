#pragma once

#include "Client_Defines.h"
#include "ImGui_Base.h"

BEGIN(Client)

class CImGui_GamePlay final : public CImGui_Base
{
private:
	CImGui_GamePlay();
	virtual ~CImGui_GamePlay() = default;

public:
public:
	virtual HRESULT Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void	Frame();

private:
	void	LayOut_Terrain();

private:
	_bool m_bActive = true;

public:
	static CImGui_GamePlay* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;

};

END


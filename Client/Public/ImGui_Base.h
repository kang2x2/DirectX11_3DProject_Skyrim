#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CImGui_Base abstract : public CBase
{
protected:
	CImGui_Base();
	virtual ~CImGui_Base() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void	Frame();

public:
	virtual void Free() override;
};

END


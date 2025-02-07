#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CIMGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CIMGui_Manager)
private:
	CIMGui_Manager();
	virtual ~CIMGui_Manager();

public:
	HRESULT Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, LEVELID _eLevelID);
	void	Frame();

private:
	class CImGui_Base* m_pCurImGui = nullptr;

public:
	virtual void Free() override;
};

END


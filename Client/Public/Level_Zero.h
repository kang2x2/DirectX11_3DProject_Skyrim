#pragma once

#include <filesystem>
#include <set>
#include <fstream>
#include <commdlg.h>

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Zero final : public CLevel
{
private:
	CLevel_Zero(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel_Zero() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT	Tick(_float _fTimeDelta)override;
	virtual HRESULT	LateTick(_float _fTimeDelta)override;

private:
	HRESULT FindFBX(const wstring& _wStrFolderPath);
	HRESULT ThrowFBX(const wstring& _wStrFolderPath);

private:
	_bool		m_bIsBinarying = false;

	wstring fileExtension = { TEXT(".FBX") };
	wstring fileExtension2 = { TEXT(".fbx") };
	set<filesystem::path> visitedDirectories;

public:
	static class CLevel_Zero* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;

};

END

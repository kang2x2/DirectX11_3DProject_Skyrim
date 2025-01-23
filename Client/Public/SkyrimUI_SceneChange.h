#pragma once

#include "SkyrimUI.h"

BEGIN(Client)

class CSkyrimUI_SceneChange final : public CSkyrimUI
{
public:
	typedef struct tagTextUIDesc
	{
		wstring strText = TEXT("");
		
		wstring strFontTag = TEXT("");
		_float	fSizeX = 0.f;
		_float	fSizeY = 0.f;
		_float	fX = 0.f;
		_float	fY = 0.f;
		_vector vColor = {};
		_float  fFontSize = 0.f;
		LEVELID iLevelID = LEVEL_END;
		STAGEID iStageID = STAGE_END;
	}UI_TEXT_DESC;

private:
	CSkyrimUI_SceneChange(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSkyrimUI_SceneChange(const CSkyrimUI_SceneChange& rhs);
	virtual ~CSkyrimUI_SceneChange() = default;

public:
	virtual HRESULT Initialize_ProtoType(); // 원본
	virtual HRESULT Initialize_Clone(void* pArg); // 사본
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Change_Event();

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private:
	wstring				m_strText = TEXT("");
	wstring				m_strFontTag = TEXT("");
	_vector				m_vColor = {};
	_float				m_fFontSize = 0.f;
	LEVELID				m_iLevelID = LEVEL_END;
	STAGEID				m_iStageID = STAGE_END;

	RECT				m_Rc = {};
	_bool				m_bIsChangeScene = false;

	_float				m_fChangeWaitTime = 0.f;
public:
	static CSkyrimUI_SceneChange* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CSkyrimUI* Clone(void* pArg) override;
	virtual void Free() override;
};

END


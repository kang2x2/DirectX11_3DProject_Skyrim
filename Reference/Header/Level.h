#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CLevel() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT	Tick(_float _fTimeDelta);
	virtual HRESULT	LateTick(_float _fTimeDelta);

public:
	virtual void AfterRender();

public:
	virtual void Set_IsChange(_bool _isChange, _uint _iLevelIdx, _uint _iStageIdx)
	{ 
		m_bIsChangeScene = _isChange;
		m_iChangeLevelIdx = _iLevelIdx;
		m_iChangeStageIdx = _iStageIdx;
	}

protected:
	ID3D11Device*		 m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	_bool				 m_bIsChangeScene = false;
	_uint				 m_iChangeLevelIdx = 0;
	_uint				 m_iChangeStageIdx = 0;
public:
	virtual void Free() override;
};

END


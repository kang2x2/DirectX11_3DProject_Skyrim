#pragma once

#include "Base.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	void Tick(_float _fTimeDelta);
	void LateTick(_float _fTimeDelta);

public:
	void	AfterRender();
	
public:
	void	Set_IsChange(_bool _isChange, _uint _iLevelIdx, _uint _iStageIdx);
	HRESULT Open_Level(_uint _iLevelIndex, class CLevel* _pLevel);

private:
	_uint			m_iCurLevelIdx = 0;
	class CLevel*	m_pCurLevel = nullptr;

public:
	virtual void Free() override;
};

END


#pragma once

#include "Base.h"

BEGIN(Engine)

class CTimer_Manager final : public CBase
{
	DECLARE_SINGLETON(CTimer_Manager);
private:
	CTimer_Manager();
	virtual ~CTimer_Manager() = default;

public:
	// 타임 델타 얻어와서 계산하기
	_float Compute_TimeDelta(const wstring& strTimerTag);
	//
	HRESULT Add_Timer(const wstring& strTimerTag);

private:
	map<const wstring, class CTimer*> m_mapTimer;

private:
	class CTimer* Find_Timer(const wstring& strTimerTag) const;

public:
	virtual void	Free(void);
};

END


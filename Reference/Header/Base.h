#pragma once

#include "Engine_Defines.h"

class ENGINE_DLL CBase abstract
{
protected:
	CBase();
	virtual ~CBase() = default;

public:
	/* RefCnt 증가. 전위 연산 후 Cnt 반환. */
	unsigned int AddRef();
	/* RefCnt 감소. 후위 연산 후 Cnt 반환. */
	unsigned int Release();

private:
	unsigned int m_iRefCnt = 0;

public:
	virtual void Free();
};


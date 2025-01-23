#include "Base.h"

CBase::CBase()
{
}

unsigned int CBase::AddRef()
{
	return ++m_iRefCnt;
}

unsigned int CBase::Release()
{
	if (m_iRefCnt == 0)
	{
		Free();

		delete this;

		return 0;
	}
	else
	{
		return m_iRefCnt--;
	}
}

void CBase::Free()
{
}

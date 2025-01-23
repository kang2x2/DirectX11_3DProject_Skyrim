#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* _pDeivce, ID3D11DeviceContext* _pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_ProtoType();
	virtual HRESULT Initialize_Clone(void* pArg);

#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; }
#endif

protected:
	ID3D11Device*				 m_pDevice = nullptr;
	ID3D11DeviceContext*		 m_pContext = nullptr;

	_bool						 m_isCloned = { false };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END


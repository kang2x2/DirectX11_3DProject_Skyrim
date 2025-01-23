#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT Initialize_ProtoType(const wstring& _strTextureFilePath, _uint _iNumTextures);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	// 텍스처 한 장 쉐이더로 전달
	HRESULT Bind_ShaderResource(const class CShader* _pShader, const char* _pConstantName, _uint _iNumTextures);
	// 배열에 저장한 모든 텍스처 쉐이더로 전달
	HRESULT Bind_ShaderResources(const class CShader* _pShader, const char* _pConstantName);

private:
	// 셰이더 단계에서 리소스를 바인딩 하기 위해 필요.
	ID3D11ShaderResourceView** m_ppSRVs = nullptr; // 동적 배열
	_uint					   m_iNumTextures = 0;

public:
	static CTexture* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext,
		const wstring& _strTextureFilePath, _uint _iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END


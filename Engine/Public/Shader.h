#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_ProtoType(const wstring& _strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint iNumElements);
	virtual HRESULT Initialize_Clone(void* pArg) override;

public:
	HRESULT Begin(_uint _iPassIndex);
	/* 셰이더에 던져 줌 */
	HRESULT Bind_RawValue(const char* _pConstantName, const void* _pData, _uint _iLength);
	HRESULT Bind_Matrix(const char* _pConstantName, const _float4x4 * _pMatrix) const;
	HRESULT Bind_Matrices(const char* _pConstantName, const _float4x4 * _pMatrices, _uint _iNumMatrices) const;
	HRESULT Bind_Texture(const char* _pConstantName, ID3D11ShaderResourceView * _pSRV) const;
	HRESULT Bind_Textures(const char* _pConstantName, ID3D11ShaderResourceView ** _ppSRV, _uint _iNumTextures) const;

private:
	ID3DX11Effect*			   m_pEffect = nullptr;
	vector<ID3D11InputLayout*> m_vecInputLayOut; // 각 패스마다 InputLayout을 만들어서 추가.

public:
	static  CShader* Create(ID3D11Device * _pDevice, ID3D11DeviceContext * _pContext, const wstring & _strShaderFilePath
		, const D3D11_INPUT_ELEMENT_DESC * _pElements, _uint _iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END


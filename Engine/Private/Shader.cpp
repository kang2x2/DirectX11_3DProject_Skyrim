#include "Shader.h"

CShader::CShader(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: CComponent(_pDevice, _pContext)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_vecInputLayOut(rhs.m_vecInputLayOut)
{
	for (auto& iter : m_vecInputLayOut)
		Safe_AddRef(iter);

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_ProtoType(const wstring& _strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint iNumElements)
{
	_uint iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	/* 해당 경로에 셰이더 파일을 컴파일하여 객체화한다. */
	if (FAILED(D3DX11CompileEffectFromFile(_strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	/* 이 셰이더안에 있는 모든 패스가 내가 그릴려고하는 정점을 잘 받아줄 수 있는가?! 확인. 이게 확인되면 */
	/* ID3D11InputLayout을 생성해 준다. */
	/* 결론적으로 ID3D11InputLayout을 생성해주기위해 밑의 작업을 수행 한다 .*/

	/* 패스를 얻기위한 작업을 선행한다. 패스는 테크니커 안에 선언되어있다. */
	/* 이 셰이더 안에 정의된 테크니커의 정볼르 얻어온다. (나는 테크니커를 하나만 정의했기때문에 0번째 인덱스로 받아온거여. )*/
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (pTechnique == nullptr)
		return E_FAIL;

	/* 테크니커의 정보를 얻어온다. */
	D3DX11_TECHNIQUE_DESC	TechniqueDesc;
	pTechnique->GetDesc(&TechniqueDesc);

	/* TechniqueDesc.Passes : 패스의 갯수. */
	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		/* pTechnique->GetPassByIndex(i) : i번째 패스객체를 얻어온다.
		ID3DX11EffectPass : 기술 내에서 상태 할당을 캡슐화*/
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);

		/* 패스의 정보를 얻어온다. */
		D3DX11_PASS_DESC		PassDesc;
		pPass->GetDesc(&PassDesc);

		ID3D11InputLayout* pInputLayout = nullptr;

		/* 패스안에 선언된 정점정보와 내가 던져준 정점정보가 일치한다면 ID3D11InputLayout를 생성해준다. */
		if (FAILED(m_pDevice->CreateInputLayout(_pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_vecInputLayOut.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize_Clone(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint _iPassIndex)
{
	if (_iPassIndex >= m_vecInputLayOut.size())
		return E_FAIL;

	// 입력 레이아웃 개체를 입력. 어셈블러 단계에 바인딩.
	m_pContext->IASetInputLayout(m_vecInputLayOut[_iPassIndex]);

	// 기술(Technique)를 테스트하여 유효한 구문이 포함되어 있는지 확인.(하나만 쓸거라 무조건 0번 인덱스)
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (pTechnique == nullptr)
		return E_FAIL;

	/* iPassIndex 번 째 패스로 그리기를 시작. 
	   반드시 그리기 전에 텍스쳐에게 전달할 정보를 모두 전달하고 그려야 한다. */
	pTechnique->GetPassByIndex(_iPassIndex)->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Bind_RawValue(const char* _pConstantName, const void* _pData, _uint _iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(_pData, 0, _iLength);
}

HRESULT CShader::Bind_Matrix(const char* _pConstantName, const _float4x4* _pMatrix) const
{
	/* pConstantName이름에 해당하는 타입을 고려하지않은 전역변수를 컨트롤하는 객체를 얻어온다. 
	   ID3DX11EffectVariable : 모든 효과 변수 클래스의 Base 클래스. */
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	// 행렬에 접근.
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (pMatrixVariable == nullptr)
		return E_FAIL;
	
	return pMatrixVariable->SetMatrix((const _float*)_pMatrix);
}

HRESULT CShader::Bind_Matrices(const char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrix = pVariable->AsMatrix();
	if (pMatrix == nullptr)
		return E_FAIL;

	return pMatrix->SetMatrixArray((_float*)_pMatrices, 0, _iNumMatrices);
}

HRESULT CShader::Bind_Texture(const char* _pConstantName, ID3D11ShaderResourceView* _pSRV) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	// ID3DX11EffectShaderResourceVariable : 셰이더 리소스에 관련된 ID3DX11EffectVariable에서 상속된 인터페이스.
	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (pSRVariable == nullptr)
		return E_FAIL;

	return pSRVariable->SetResource(_pSRV);
}

HRESULT CShader::Bind_Textures(const char* _pConstantName, ID3D11ShaderResourceView** _ppSRV, _uint _iNumTextures) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (pVariable == nullptr)
		return E_FAIL;

	// ID3DX11EffectShaderResourceVariable : 셰이더 리소스에 관련된 ID3DX11EffectVariable에서 상속된 인터페이스.
	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (pSRVariable == nullptr)
		return E_FAIL;

	return pSRVariable->SetResourceArray(_ppSRV, 0, _iNumTextures);
}

CShader* CShader::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const wstring& _strShaderFilePath
	, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements)
{
	CShader* pInstance = new CShader(_pDevice, _pContext);

 	if (FAILED(pInstance->Initialize_ProtoType(_strShaderFilePath, _pElements, _iNumElements)))
	{
		MSG_BOX("Fail Create : CShader_ProtoType");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Fail Clone : CShader_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CShader::Free()
{
	__super::Free();

	for (auto& iter : m_vecInputLayOut)
	{
		Safe_Release(iter);
	}

	Safe_Release(m_pEffect);
}

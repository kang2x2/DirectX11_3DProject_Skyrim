#include "OcTree.h"
#include "PipeLine.h"

COcTree::COcTree(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice), m_pContext(_pContext),
	  m_pBatch(nullptr), m_pRoot(nullptr), m_iDepthCount(4)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT COcTree::Initialize(_float3 _vCenter, _float3 _vRootRange)
{
#ifdef _DEBUG
	/* 디버깅에 대한 준비 */
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);

	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_DESC	DSStateDesc = {};
	DSStateDesc.DepthEnable = true;
	DSStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSStateDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	DSStateDesc.StencilEnable = false;

	if (FAILED(m_pDevice->CreateDepthStencilState(&DSStateDesc, &m_pDSState)))
		return E_FAIL;

#endif

	m_pRoot = SetTree(_vCenter, _vRootRange, 0);

	return S_OK;
}

COcTreeNode* COcTree::SetTree(_float3 _vCenter, _float3 _vRange, _uint _iDepth)
{
	if (_iDepth >= m_iDepthCount)
		return nullptr;

	COcTreeNode* pNode = new COcTreeNode();
	pNode->Initialize(_vCenter, _vRange);

	//_float3 vChildCenter = { 0.f, 0.f, 0.f };
	//_float3 vChildRange = { _vRange.x * 0.5f, _vRange.y * 0.5f, _vRange.z * 0.5f };
	//
	//_float fCenterX = _vCenter.x * 0.5f;
	//_float fCenterY = _vCenter.y * 0.5f;
	//_float fCenterZ = _vCenter.z * 0.5f;
	//
	//for (int i = 0; i < 8; ++i)
	//{
	//	// 범위와 센터 지정
	//	// FLB -> FRB -> FLT -> FRT -> BLB -> BRB -> BLT -> BRT 순서
	//	// x : - + - + - + - +
	//	// y : - - + + - - + +
	//	// z : - - - - + + + +
	//	vChildCenter.x = (i % 2 == 0) ? -fCenterX : fCenterX;
	//	vChildCenter.z = (i < 4) ? -fCenterZ : fCenterZ;
	//
	//	if (vChildCenter.y == 0 || vChildCenter.y == 1 || vChildCenter.y == 4 || vChildCenter.y == 5)
	//	{
	//		vChildCenter.y = -fCenterY;
	//	}
	//	else
	//	{
	//		vChildCenter.y = fCenterY;
	//	}
	//
	//	pNode->AddChild(SetTree(vChildCenter, vChildRange, ++_iDepth));
	//}

	return pNode;
}

HRESULT COcTree::Update()
{
	m_pRoot->Update();

	return S_OK;
}

#ifdef _DEBUG
HRESULT COcTree::Render()
{
	m_pContext->OMSetDepthStencilState(m_pDSState, 0);

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

	Safe_Release(pPipeLine);

	m_pContext->IASetInputLayout(m_pInputLayout);
	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();

	m_pRoot->Render(m_pBatch);

	m_pBatch->End();
	
	return S_OK;
}
#endif

void COcTree::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pDSState);
	Safe_Release(m_pInputLayout);
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
#endif

	Safe_Release(m_pRoot);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

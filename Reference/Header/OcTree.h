#pragma once
#include "Base.h"
#include "OcTreeNode.h"

BEGIN(Engine)

class COcTree : public CBase
{
public:
	COcTree(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~COcTree() = default;

public:
	HRESULT Initialize(_float3 _vCenter, _float3 _vRootRange);
	COcTreeNode* SetTree(_float3 _vCenter, _float3 _vRange, _uint _iDepth);

	HRESULT Update();

#ifdef _DEBUG
	HRESULT Render();
#endif

private:
	/* Vertex, Index Buffer를 생성하고 그리는 기능을 제공하는 객체.  */
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3D11DepthStencilState* m_pDSState = nullptr;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	COcTreeNode* m_pRoot;
	_uint m_iDepthCount;

public:
	virtual void Free() override;
};

END
